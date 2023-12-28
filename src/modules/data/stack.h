#pragma once
#include <modules\data.h>

namespace MM2
{
    // Forward declarations
    class datStack;
    struct MapSymbol;

    // External declarations


    // Class definitions
    struct MapSymbol
    {
        const char* Name;
        int Address;
    };

    class datStack {
    private:
        static hook::Type<bool> MapInitialized;
        static hook::Type<int> MapSymbolCount;
        static hook::Type<MapSymbol*> MapSymbols;
    private:
        static void TracebackLua(int length);
        static std::string LookupAddressLua(int address);
    public:
        // marked public for hooks
        static int ExceptionFilter(EXCEPTION_POINTERS* eptrs);
        static int ExceptionFilterWithMsgBox(EXCEPTION_POINTERS* eptrs);
        static int ExceptionFilterCombined(EXCEPTION_POINTERS* eptrs);
    public:
        AGE_API static char const* GetTimestamp();
        AGE_API static void LookupAddress(char* buf, int address);
        AGE_API static void DoTraceback(int length, DWORD* contextRecordEbpPtr, FILE* output, char const* lineSeperator);
        AGE_API static void Traceback(int length, FILE* output);

        static const MapSymbol* LookupMapSymbol(int address);
        static void InitDebugSymbols();

        //lua
        static void BindLua(LuaState L);
    };
}