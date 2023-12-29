#pragma once
#include "stream.h"

using namespace MM2;

// Stream
declfield(Stream::coreFileStandard)(0x5CEE28);
declfield(Stream::zipFile)(0x5DA768);

declfield(Stream::sm_DefaultOpenMethods)(0x5CED78);
declfield(Stream::sm_DefaultCreateMethods)(0x5CED7C);

std::string Stream::ReadString(int length)
{
    std::string buf;
    buf.resize(length);

    int read = this->Read((LPVOID)buf.data(), length);
    if (read != length)
        buf.resize(read);
    return buf;
}

void MM2::Stream::WriteString(std::string str)
{
    auto cstr = str.c_str();
    this->Write((void*)cstr, str.size()); // use str.size() here because it's valid for a std::string to contain \0
}

std::string Stream::AsString()
{
    int remainBytes = this->Size() - this->Tell();
    return ReadString(remainBytes);
}

AGE_API void Stream::DumpOpenFiles() { return hook::StaticThunk<0x4C9970>::Call<void>(); };

AGE_API Stream* Stream::Open(LPCSTR filename, bool readOnly) { return hook::StaticThunk<0x4C99C0>::Call<Stream*>(filename, readOnly); };
AGE_API Stream* Stream::Open(LPCSTR filename, const coreFileMethods* fileMethods, bool readOnly)
{
    return hook::StaticThunk<0x4C9A40>::Call<Stream*>(filename, fileMethods, readOnly);
};

AGE_API Stream* Stream::Create(LPCSTR filename) { return hook::StaticThunk<0x4C9A00>::Call<Stream*>(filename); };
AGE_API Stream* Stream::Create(LPCSTR filename, const coreFileMethods* fileMethods)
{
    return hook::StaticThunk<0x4C9A70>::Call<Stream*>(filename, fileMethods);
};

AGE_API int Stream::Read(LPVOID dstBuf, int size) { return hook::Thunk<0x4C9AA0>::Call<int>(this, dstBuf, size); };
AGE_API int Stream::Write(const LPVOID srcBuf, int size) { return hook::Thunk<0x4C9BF0>::Call<int>(this, srcBuf, size); };
AGE_API int Stream::GetCh() { return hook::Thunk<0x4C9D00>::Call<int>(this); };
AGE_API int Stream::PutCh(unsigned char ch) { return hook::Thunk<0x4C9D30>::Call<int>(this, ch); };
AGE_API int Stream::Seek(int offset) { return hook::Thunk<0x4C9D60>::Call<int>(this, offset); };
AGE_API int Stream::Tell() { return hook::Thunk<0x4C9DB0>::Call<int>(this); };
AGE_API int Stream::Close() { return hook::Thunk<0x4C9DC0>::Call<int>(this); };
AGE_API int Stream::Size() { return hook::Thunk<0x4C9E00>::Call<int>(this); };
AGE_API int Stream::Flush() { return hook::Thunk<0x4C9E60>::Call<int>(this); };

int MM2::fseek(Stream* stream, int position, seekWhence whence) { return hook::StaticThunk<0x4C9F20>::Call<int>(stream, position, whence); }
int MM2::fgets(char* buffer, int length, Stream* stream) { return hook::StaticThunk<0x4C9F80>::Call<int>(buffer, length, stream); }

void MM2::fprintf(Stream* stream, char const* format, ...) {
    /* It's much easier to just rewrite the function since it's so simple */
    char buf[512];
    va_list va;
    va_start(va, format);
    _vsnprintf(buf, sizeof(buf), format, va);
    va_end(va);

    stream->Write(buf, strlen(buf));
}

int MM2::fscanf(Stream* stream, char const* format, ...) {
    /*
    I think I need a shower after writing this...
    (Don't blame me, this is how Angel does it when calling sscanf!)
    */
    va_list va;
    va_start(va, format);
    int result = hook::StaticThunk<0x4C9FF0>::Call<int>(stream, format, *va, *(va + 1), *(va + 2), *(va + 3), *(va + 4), *(va + 5), *(va + 6), *(va + 7));
    va_end(va);

    return result;
}

void Stream::BindLua(LuaState L) {
    LuaBinding(L).beginClass<coreFileMethods>("coreFileMethods")
        .endClass();

    LuaBinding(L).beginClass<Stream>("Stream")
        .addFactory([](LPCSTR filename, bool createFile = false) {
        auto stream = (createFile) ? Stream::Create(filename) : Stream::Open(filename, false);
        return stream;
            }, LUA_ARGS(LPCSTR, _opt<bool>))
        .addStaticProperty("ZipFileMethods", []() {return zipFile.ptr(); })
        .addStaticProperty("StandardFileMethods", []() {return coreFileStandard.ptr(); })

        .addStaticFunction("DumpOpenFiles", &Stream::DumpOpenFiles)

        .addStaticFunction("Open", static_cast<Stream * (*)(LPCSTR, bool)>(&Stream::Open))
        .addStaticFunction("OpenWithMethods", static_cast<Stream * (*)(LPCSTR, const coreFileMethods*, bool)>(&Stream::Open))
        .addStaticFunction("Create", static_cast<Stream * (*)(LPCSTR)>(&Stream::Create))
        .addStaticFunction("CreateWithMethods", static_cast<Stream * (*)(LPCSTR, const coreFileMethods*)>(&Stream::Create))

        .addFunction("ReadAll", &Stream::AsString)

        .addFunction("Read", &Stream::ReadString)
        .addFunction("Write", &Stream::WriteString)
        .addFunction("GetCh", &Stream::GetCh)
        .addFunction("PutCh", &Stream::PutCh)
        .addFunction("Seek", &Stream::Seek)
        .addFunction("Tell", &Stream::Tell)
        .addFunction("Close", &Stream::Close)
        .addFunction("Size", &Stream::Size)
        .addFunction("Flush", &Stream::Flush)
        .endClass();
}