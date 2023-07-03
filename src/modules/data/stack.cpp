#pragma once
#include "stack.h"
#include <DbgHelp.h>

using namespace MM2;

/*
    datStack
*/
declfield(datStack::MapInitialized)(0x6A3CEC);
declfield(datStack::MapSymbolCount)(0x6A3CE8);
declfield(datStack::MapSymbols)(0x6A3CE4);

static BOOL DbgHelpLoaded = FALSE;

void datStack::TracebackLua(int length)
{
    Traceback(length, nullptr);
}

std::string datStack::LookupAddressLua(int address) {
    char buf[128];
    LookupAddress(buf, address);
    return std::string(buf);
}

AGE_API char const* datStack::GetTimestamp() {
    return hook::StaticThunk<0x4C7120>::Call<char const*>();
};

AGE_API void datStack::LookupAddress(char* buf, int address) {
    InitDebugSymbols();

    if (DbgHelpLoaded)
    {
        DWORD64 dwDisplacement = 0;

        alignas(SYMBOL_INFO) char symbol_buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbol_buffer;

        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = MAX_SYM_NAME;

        IMAGEHLP_MODULE module{};
        module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);

        if (SymFromAddr(GetCurrentProcess(), address, &dwDisplacement, pSymbol) &&
            SymGetModuleInfo(GetCurrentProcess(), address, &module))
        {
            bool hide_module = (module.BaseOfImage == 0x400000) || !_stricmp(module.ModuleName, "DINPUT");

            if (hide_module)
            {
                sprintf(buf, "0x%08zx (%s + 0x%X)", address, pSymbol->Name, static_cast<unsigned int>(dwDisplacement));
            }
            else
            {
                sprintf(buf, "0x%08zx (%s.%s + 0x%X)", address, module.ModuleName, pSymbol->Name, static_cast<unsigned int>(dwDisplacement));
            }
            return;
        }
    }

    // Grab from map instead
    auto symbol = LookupMapSymbol(address);
    if (symbol != nullptr)
    {
        char undec_name[256];

        const char* function_name =
            UnDecorateSymbolName(symbol->Name, undec_name, sizeof(undec_name), UNDNAME_NAME_ONLY) ? undec_name
            : symbol->Name;

        sprintf(buf, "0x%08zx (%s + 0x%zx)", address, function_name, address - symbol->Address);
        return;
    }

    // Not found at all
    sprintf(buf, "0x%08zx (Unknown)", address);
};

AGE_API void datStack::DoTraceback(int length, DWORD* contextRecordEbpPtr, FILE* output, char const* lineSeperator) {
    char addressBuffer[1024];
    DWORD* currentEbpPtr = contextRecordEbpPtr;
    for (int i = 0; i < length; i++)
    {
        auto v6 = currentEbpPtr[1];
        currentEbpPtr = (DWORD*)*currentEbpPtr;
        if (v6 < 0 || currentEbpPtr == nullptr)
            break;
        datStack::LookupAddress(addressBuffer, v6);
        if (output)
            fprintf(output, "%s,%s", addressBuffer, lineSeperator);
        else
            Displayf("%s", addressBuffer);
    }
};

AGE_API void datStack::Traceback(int length, FILE* output) {
    hook::StaticThunk<0x4C75C0>::Call<void>(length, output);
};

const MapSymbol* datStack::LookupMapSymbol(int address)
{
    InitDebugSymbols();

    int symbolCount = MapSymbolCount.get();
    MapSymbol* first = MapSymbols.get();

    for (int i = 0; i < symbolCount; i++)
    {
        int addressRangeMin = (first + i)->Address;
        int addressRangeMax = (i == (symbolCount - 1)) ? addressRangeMin + 64 : (first + i + 1)->Address;

        if (address >= addressRangeMin && address < addressRangeMax)
            return (first + i);
    }

    return nullptr;
}

//custom extension for exception filter
int datStack::ExceptionFilter(EXCEPTION_POINTERS* eptrs) {
    char addressBuf[128];
    auto ctx = eptrs->ContextRecord;
    datStack::LookupAddress(addressBuf, ctx->Eip);
    Displayf(
        "EAX=%08x EBX=%08x ECX=%08x EDX=%08x\nESI=%08x EDI=%08x EBP=%08x ESP=%08x",
        ctx->Eax,
        ctx->Ebx,
        ctx->Ecx,
        ctx->Edx,
        ctx->Esi,
        ctx->Edi,
        ctx->Ebp,
        ctx->Esp);
    Displayf("Exception %x at EIP=%s", eptrs->ExceptionRecord->ExceptionCode, addressBuf);
    datStack::DoTraceback(16, &ctx->Ebp, nullptr, "\n");
    return 1;
}

int datStack::ExceptionFilterWithMsgBox(EXCEPTION_POINTERS* eptrs) {
    auto ctx = eptrs->ContextRecord;

    //create our buffers (yuck!)
    char addressBuf[128], stackBuf[256], tracebackBuf[8192];
    char totalBuf[128 + 256 + 256 + 8192]; //address+stack+exception(below)+trace

    //create a temporary file for the traceback (yuck again, Windows, y u no have fmemopen)
    FILE* tempFile = tmpfile();
    if (tempFile == NULL) {
        Displayf("Couldn't create temp file for stack trace.");
        return 1;
    }
        
    //fill address and stack buf
    datStack::LookupAddress(addressBuf, ctx->Eip);
    sprintf_s(stackBuf, "EAX=%08x EBX=%08x ECX=%08x EDX=%08x\nESI=%08x EDI=%08x EBP=%08x ESP=%08x",
                      ctx->Eax,
                      ctx->Ebx,
                      ctx->Ecx,
                      ctx->Edx,
                      ctx->Esi,
                      ctx->Edi,
                      ctx->Ebp,
                      ctx->Esp);

    //do our traceback into tempFile
    datStack::DoTraceback(16, &ctx->Ebp, tempFile, "\n");

    //read the file to the traceback buf and close it
    long tempFileLen = ftell(tempFile);
    rewind(tempFile);           
    int readLength = fread(tracebackBuf, 1, tempFileLen, tempFile);
    tracebackBuf[readLength] = 0x00;
    fclose(tempFile);

    //finally combine everything and show the error
    sprintf_s(totalBuf, "%s\nException %x at EIP=%s\n%s", stackBuf, eptrs->ExceptionRecord->ExceptionCode, addressBuf, tracebackBuf);
    MessageBoxA(0, totalBuf, "Guru Meditation", MB_OK | MB_ICONSTOP);
    return 1;
}

int datStack::ExceptionFilterCombined(EXCEPTION_POINTERS* eptrs) {
    ExceptionFilter(eptrs);
    ExceptionFilterWithMsgBox(eptrs);
    return 1;
}

void datStack::InitDebugSymbols()
{
    if (!MapInitialized.get()) 
    {
        // Init DbgHelp
        SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
        DbgHelpLoaded = SymInitialize(GetCurrentProcess(), NULL, TRUE);

        if (!DbgHelpLoaded)
            Errorf("Failed to load debug symbols, error: 0x%08X (handle 0x%08X)", GetLastError(), GetCurrentProcess());

        // InitMap
        hook::StaticThunk<0x4C7130>::Call<void>();
    }
}

//lua
void datStack::BindLua(LuaState L) {
    LuaBinding(L).beginClass<datStack>("datStack")
        .addStaticProperty("Timestamp", [] {return GetTimestamp(); })
        .addStaticFunction("Traceback", &TracebackLua)
        .addStaticFunction("LookupAddress", &LookupAddressLua)
        .endClass();
}
