#pragma once
#include "common.h"

//
// A.G.E. (Angel Game Engine) common includes
//

#ifdef AGE_API_EXPORT
# define AGE_API NOINLINE __declspec(dllexport)
# define AGE_EXT
#else
# define AGE_API
# define AGE_EXT extern
#endif

//
// DirectX includes
//

// use DIRECTX_VERSION to set some/all of them
// or define them individually, etc.
// otherwise they'll default via the includes
#ifdef DIRECTX_VERSION
# ifndef DIRECT3D_VERSION
#  define DIRECT3D_VERSION DIRECTX_VERSION
# endif
# ifndef DIRECTDRAW_VERSION
#  define DIRECTDRAW_VERSION DIRECTX_VERSION
# endif
# ifndef DIRECTINPUT_VERSION
#  define DIRECTINPUT_VERSION DIRECTX_VERSION
# endif
# ifndef DIRECTSOUND_VERSION
#  define DIRECTSOUND_VERSION DIRECTX_VERSION
# endif
#endif

#include <d3d.h>
#include <ddraw.h>
#include <dinput.h>

#include <mmsystem.h>
#include <dsound.h>

typedef HRESULT(WINAPI *LPFNDIRECTINPUTCREATE)(HINSTANCE, DWORD, LPVOID *, LPUNKNOWN);
typedef HRESULT(WINAPI *LPDIRECTDRAWCREATEEX)(GUID *, LPVOID *, REFIID, IUnknown *);

struct ageInfo
{
    short gameVersion;
    short engineVersion;
    LPCSTR versionString;
};

struct ageInfoLookup {
    DWORD offset;
    bool isSupported;

    ageInfo info;
};

class ageGame {
protected:
    ageInfo m_info;
public:
    ageGame(const ageInfo &gameInfo) : m_info(gameInfo) {};

    ageGame(short gameVersion, short engineVersion)
        : m_info { gameVersion, engineVersion } {};
    ageGame(short gameVersion, short engineVersion, LPCSTR versionString)
        : m_info { gameVersion, engineVersion, versionString } {};

    static bool GetAGEInfo(const ageInfoLookup *infoLookup, ageInfoLookup &outInfo) {
        const ageInfoLookup *l = NULL;
        for (l = infoLookup; l->offset != 0; l++) {
            if (strcmp((LPCSTR)l->offset, l->info.versionString) == 0)
            {
                outInfo = *l;
                return true;
            }
        }
        return false;
    };

    virtual int GetGameVersion() const {
        return m_info.gameVersion;
    };

    virtual int GetEngineVersion() const {
        return m_info.engineVersion;
    };

    virtual void Initialize() {};
};

class ageHook {
private:
    template <typename TRet, typename... TArgs>
    using MethodCall = TRet(*)(TArgs...);

    template <typename TRet, class TThis, typename... TArgs>
    using MemberCall = TRet(__thiscall *)(const TThis, TArgs...);

    template <typename TRet, typename... TArgs>
    using StdMethodCall = TRet(__stdcall *)(TArgs...);
public:
    template <typename TType>
    class Type {
    protected:
        TType *lpValue;
    public:
        constexpr Type(int address) : lpValue(reinterpret_cast<TType *>(address)) {};

        inline TType* get() const {
            return this->lpValue;
        }

        inline void set(TType value) {
            *lpValue = value;
        }

        /*
        TType*->TMember - Pointer to struct of TType
        */
        inline TType& operator->() const {
            return *this->get();
        };

        /*
        &TType - Instance of TType
        */
        inline TType* operator&() const {
            return this->get();
        };

        /*
        *TType - Pointer to TType
        */
        inline TType& operator*() const {
            return *this->get();
        };


        /*
        TType*(this) - Convert this to TType pointer
        */
        explicit inline operator TType*() const {
            return this->get();
        }

        /*
        TType&(this) - Convert this to TType reference
        */
        inline operator TType&() const {
            return *this->get();
        }

        /*
        TType[0] - Pointer is TType array
        */
        inline TType& operator[](int index) const {
            return this->get()[index];
        }

        /*
        this() - TType is function pointer
        */
        template <typename... TArgs>
        inline auto operator()(TArgs... args) {
            return (*this->get())(args...);
        }

        inline TType& operator=(TType value) {
            return (*lpValue = value);
        }
    };

    template <int _address>
    class Thunk {
    private:
        static constexpr LPVOID address = reinterpret_cast<LPVOID>(_address);
    public:
        template<typename TRet, typename ...TArgs>
        static constexpr TRet Call(TArgs ...args) {
            return static_cast<MethodCall<TRet, TArgs...>>(address)(args...);
        };

        template<typename TRet, class TThis, typename ...TArgs>
        static constexpr TRet Call(const TThis &&This, TArgs ...args) {
            return static_cast<MemberCall<TRet, TThis, TArgs...>>(address)(This, args...);
        };
    };

    template <typename TRet>
    class Func {
    protected:
        LPVOID lpFunc;
    public:
        constexpr Func(int address) : lpFunc(reinterpret_cast<LPVOID>(address)) {};

        template<typename ...TArgs>
        inline constexpr TRet operator()(TArgs ...args) const {
            return static_cast<MethodCall<TRet, TArgs...>>(lpFunc)(args...);
        };

        class StdCall : protected Func<TRet> {
        public:
            constexpr StdCall(int address) : Func<TRet>(address) {};

            template<typename ...TArgs>
            inline constexpr TRet operator()(TArgs ...args) const {
                return static_cast<StdMethodCall<TRet, TArgs...>>(lpFunc)(args...);
            };

            template <typename TRet, typename... TArgs>
            inline constexpr operator StdMethodCall<TRet, TArgs...>() const {
                return static_cast<StdMethodCall<TRet, TArgs...>>(lpFunc);
            };
        };

        class ThisCall : protected Func<TRet> {
        public:
            constexpr ThisCall(int address) : Func<TRet>(address) {};

            template<typename ...TArgs, class TThis>
            inline constexpr TRet operator()(const TThis &&This, TArgs ...args) const {
                return static_cast<MemberCall<TRet, TThis, TArgs...>>(lpFunc)(This, args...);
            };

            template <typename TRet, class TThis, typename... TArgs>
            inline constexpr operator MemberCall<TRet, TThis, TArgs...>() const {
                return static_cast<MemberCall<TRet, TThis, TArgs...>>(lpFunc);
            };
        };
    };

    template <typename TRet>
    class MemberFunc : protected Func<TRet> {
    public:
        constexpr MemberFunc(int address) : Func<TRet>(address) {};

        template<typename ...TArgs, class TThis>
        inline constexpr TRet operator()(const TThis &&This, TArgs ...args) const {
            return static_cast<MemberCall<TRet, TThis, TArgs...>>(lpFunc)(This, args...);
        };

        template <typename TRet, class TThis, typename... TArgs>
        inline constexpr operator MemberCall<TRet, TThis, TArgs...>() const {
            return static_cast<MemberCall<TRet, TThis, TArgs...>>(lpFunc);
        };
    };

    /*
    __cdecl function declaration
    */
    template <typename TRet, typename... TArgs>
    class Func<MethodCall<TRet, TArgs...>> : protected Func<TRet> {
        using MethodCall = MethodCall<TRet, TArgs...>;
    public:
        constexpr Func(int address) : Func<TRet>(address) {};

        inline TRet operator()(TArgs ...args) const {
            return static_cast<MethodCall>(lpFunc)(args...);
        };

        inline operator MethodCall() const {
            return static_cast<MethodCall>(lpFunc);
        };
    };

    /*
    __stdcall function declaration
    */
    template <typename TRet, typename... TArgs>
    class Func<StdMethodCall<TRet, TArgs...>> : protected Func<TRet> {
        using StdMethodCall = StdMethodCall<TRet, TArgs...>;
    public:
        constexpr Func(int address) : Func<TRet>(address) {};

        inline TRet operator()(TArgs ...args) const {
            return static_cast<StdMethodCall>(lpFunc)(args...);
        };

        inline operator StdMethodCall() const {
            return static_cast<StdMethodCall>(lpFunc);
        };
    };

    /*
    __thiscall function declaration
    */
    template <typename TRet, class TThis, typename... TArgs>
    class Func<MemberCall<TRet, TThis, TArgs...>> : protected Func<TRet> {
        using MemberCall = MemberCall<TRet, TThis, TArgs...>;
    public:
        constexpr Func(int address) : Func<TRet>(address) {};

        inline TRet operator()(const TThis &&This, TArgs ...args) const {
            return static_cast<MemberCall>(lpFunc)(args...);
        };

        inline operator MemberCall() const {
            return static_cast<MemberCall>(lpFunc);
        };
    };
};