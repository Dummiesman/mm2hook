/*
Depending on where we want our symbols to be defined,
this allows us to reuse the references as many times we want.

This will only work for hooks in a namespace, NOT a class!

Once a .cpp file declares this, hook declarations will be created.

Multiple declarations will cause compiler errors!
*/
#ifdef MM2_API_EXPORT
# define HOOK_API
# define declhook(addr, type, name) HOOK_API type name (addr)
#else
# define HOOK_API extern
# define declhook(addr, type, name) HOOK_API type name
#endif
#pragma once

#define declfield(t) decltype(t) t

#define VIRTUAL_THUNK(addr, rtype, ...) { return hook::Thunk<addr>::Call<rtype> WRAP(this, __VA_ARGS__); }

//
// Allocator
// Calls MM2's internal operator new
//
#define ANGEL_ALLOCATOR void* operator new(size_t size)          { return hook::StaticThunk<0x577360>::Call<void*>(size); } \
                        void operator delete(void * pointer)     { hook::StaticThunk<0x577380>::Call<void>(pointer);}       \
                        void operator delete[](void * pointer)   { hook::StaticThunk<0x5773C0>::Call<void>(pointer);}

#define LEVEL_ALLOCATOR void* operator new(size_t size)          { return hook::StaticThunk<0x463110>::Call<void*>(size); } \
                        void operator delete(void * pointer)     { hook::StaticThunk<0x463170>::Call<void>(pointer);}       
//
// MM2 uses DirectX 7
//
#define DIRECTX_VERSION 0x0700

#include "common.h"
#include "AGE.h"
#include "hook.h"

#include "mm2_utils.h"
#include "mm2_vector.h"

enum mm2_module {
    module_ai,
    module_audio,
    module_base,
    module_bound,
    module_breakable,
    module_city,
    module_common,
    module_creature,
    module_data,
    module_game,
    module_gfx,
    module_input,
    module_inst,
    module_level,
    module_model,
    module_network,
    module_phys,
    module_rgl,
    module_stream,
    module_ui,
    module_vehicle,
    module_effects,
    module_camera,

    module_count, // number of Lua modules
};

template<class TNode>
using luaClassBinder = CppBindClass<TNode, LuaBinding>;

namespace MM2 {
    template <class T>
    static inline void luaBind(LuaState L) {
        T::BindLua(L);
    }

    template <class T>
    static inline void luaBind(LuaState L, LPCSTR name) {
        luaClassBinder<T> lc = LuaBinding(L).beginClass<T>(name);
        T::BindLua(&lc);
        lc.endClass();
    }

    template <enum mm2_module>
    static inline void luaAddModule(LuaState L) { /* do nothing */ }

    struct LocString {
        char buffer[512];
    };

    extern void Printf(LPCSTR str, ...);
    extern void Messagef(LPCSTR str, ...);
    extern void Displayf(LPCSTR str, ...);
    extern void Warningf(LPCSTR str, ...);
    extern void Errorf(LPCSTR str, ...);
    extern void Quitf(LPCSTR str, ...);
    extern void Abortf(LPCSTR str, ...);

    class string {
    public:
        AGE_API string(const char *str)                     { hook::Thunk<0x505070>::Call<void>(this, str); }

        AGE_API string(char *str, int len) {
            buffer = str;
            length = len;
        }

        AGE_API void operator+=(char c)                     { hook::Thunk<0x49F4C0>::Call<void>(this, c); }
        AGE_API void operator+=(const char *str)            { hook::Thunk<0x49F3E0>::Call<void>(this, str); }
        AGE_API void operator-=(const char *str)            { hook::Thunk<0x49F6D0>::Call<void>(this, str); }
        AGE_API void operator=(const char *str)             { hook::Thunk<0x4A0A90>::Call<void>(this, str); }

        AGE_API string operator+(const string &str) const   { return hook::Thunk<0x4A0B00>::Call<string>(this, &str); }
        AGE_API string operator+(const char *str) const     { return hook::Thunk<0x49F180>::Call<string>(this, str); }
        AGE_API string operator-(const char *str) const     { return hook::Thunk<0x49F560>::Call<string>(this, str); }

        AGE_API int NumSubStrings(void) const               { return hook::Thunk<0x4A0A20>::Call<int>(this); }
        
        AGE_API string SubString(int index) const           { return hook::Thunk<0x4A0910>::Call<string>(this, index); }

        inline operator char *(void) const {
            return buffer;
        }

        char *buffer;
        int length;
    };

    extern char * AngelReadString(UINT stringId);

    class Timer {
    public:
        static hook::Type<float> TicksToSeconds;
        static hook::Type<float> TicksToMilliseconds;

        DWORD StartTicks;

        AGE_API Timer()                                     { hook::Thunk<0x4C7840>::Call<void>(this); }

        static AGE_API void BeginBenchmark()                       { hook::StaticThunk<0x4C7980>::Call<void>(); }
        static AGE_API void EndBenchmark()                         { hook::StaticThunk<0x4C79F0>::Call<void>(); }
        static AGE_API uint QuickTicks()                           { return hook::StaticThunk<0x4C7810>::Call<uint>(); }
        static AGE_API uint Ticks()                               { return hook::StaticThunk<0x4C77E0>::Call<uint>(); }   
    };

    enum dgGameMode {
        Cruise,
        Checkpoint,
        CnR,
        Circuit,
        Blitz,
        CRoam, // unused
        CrashCourse,
    };

    enum dgSkillLevel {
        Amateur,
        Professional,
    };

    /*
        Make sure all of our ducks are in a row --
        The size of this stuff is EXTREMELY important!
    */
    ASSERT_SIZEOF(dgGameMode, 4);
    ASSERT_SIZEOF(dgSkillLevel, 4);
    

    declhook(0x402180, _Func<void>, $CreateGameMutex);

    declhook(0x4C9510, _Func<void>, $DefaultPrintString);
    declhook(0x4C95F0, _Func<void>, $DefaultPrinter);

    declhook(0x5346B0, _Func<int>::StdCall, $MyLoadStringA);

    declhook(0x5CED24, _Type<void(*)(int, LPCSTR, va_list)>, Printer);

    declhook(0x5E0CC4, _Type<void(*)(void)>, __VtResumeSampling);
    declhook(0x5E0CD8, _Type<void(*)(void)>, __VtPauseSampling);


    declhook(0x6A3AA8, _Type<int>, joyDebug);
    declhook(0x6A3C0C, _Type<int>, assetDebug);
    declhook(0x683104, _Type<int>, gfxDebug);
    declhook(0x6B4C24, _Type<int>, audDebug);

    declhook(0x6A3AC0, _Type<int>, gRandSeed);

    template<>
    void luaAddModule<module_common>(LuaState L) {
        typedef void(__cdecl *printer_type)(LPCSTR);
        LuaBinding(L)
            .addFunction("Printf", (printer_type)&Printf)
            .addFunction("Messagef", (printer_type)&Messagef)
            .addFunction("Displayf", (printer_type)&Displayf)
            .addFunction("Warningf", (printer_type)&Warningf)
            .addFunction("Errorf", (printer_type)&Errorf)
            .addFunction("Quitf", (printer_type)&Quitf)
            .addFunction("Abortf", (printer_type)&Abortf)

            .addFunction("AngelReadString", &AngelReadString);
    }
};