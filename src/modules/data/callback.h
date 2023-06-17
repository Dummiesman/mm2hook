#pragma once
#include <mm2_common.h>
#include <modules\data\base.h>

namespace MM2
{
    // Forward declarations
    class datCallback;

    // External declarations
    extern class Base;

    // Class definitions

    namespace $
    {
        namespace datCallback
        {
            declhook(0x4C7A40, _MemberFunc<void>, $$ctor$void);

            declhook(0x4C7A50, _MemberFunc<void>, $$ctor$ThisCB$1);
            declhook(0x4C7A80, _MemberFunc<void>, $$ctor$ThisCB$2);
            declhook(0x4C7AA0, _MemberFunc<void>, $$ctor$ThisCB$3);

            declhook(0x4C7AC0, _MemberFunc<void>, $$ctor$CB$1);
            declhook(0x4C7AF0, _MemberFunc<void>, $$ctor$CB$2);
            declhook(0x4C7B20, _MemberFunc<void>, $$ctor$CB$3);

            declhook(0x4C7B50, _MemberFunc<void>, Call);
        }
    }
    
    template <typename Func, typename = void>
    struct CallbackInvoker
    {
        static void Invoke(void* context, void* /*param*/)
        {
            (*static_cast<Func*>(context))();
        }
    };

    template <typename Func>
    struct CallbackInvoker<Func, decltype(void(std::declval<Func&>()(std::declval<void*>())))>
    {
        static void Invoke(void* context, void* param)
        {
            (*static_cast<Func*>(context))(param);
        }
    };

    class datCallback
    {
    protected:
        enum : uint32_t {
            // The highest bit of a function should never be set (unless perhaps the main executable is marked as /LARGEADDRESSAWARE)
            CB_TYPE_GAME = 0x80000000,

            // MM2 functions only use the lowest 23 bits (address range 0x400000 - 0x5B0000)
            CB_TYPE_PARAM0 = CB_TYPE_GAME | 0x20000000,
            CB_TYPE_PARAM1 = CB_TYPE_GAME | 0x40000000,
            CB_TYPE_PARAM2 = CB_TYPE_GAME | 0x60000000,

            CB_TYPE_MASK = 0xE0000000,
            CB_FUNC_MASK = 0x1FFFFFFF,
        };

    public:
        using Static0 = void (*)();
        using Static1 = void (*)(void*);
        using Static2 = void (*)(void*, void*);

        using Member0 = void (Base::*)();
        using Member1 = void (Base::*)(void*);
        using Member2 = void (Base::*)(void*, void*);

        static hook::TypeProxy<datCallback> NullCallback;

        datCallback() = default;

        datCallback(std::nullptr_t)
        {
        }

        template <typename Func>
        datCallback(Func func)
            : invoke_(CallbackInvoker<Func>::Invoke)
        {
            new (data_) Func(func);
            static_assert(sizeof(func) <= sizeof(data_));
            static_assert(std::is_trivially_copyable_v<Func>);
        }

        // Deprecated, use lambdas.
        explicit datCallback(Static0 func);
        explicit datCallback(Static1 func, void* param);
        explicit datCallback(Static2 func, void* param);
        explicit datCallback(Member0 func, Base* base);
        explicit datCallback(Member1 func, Base* base, void* param);
        explicit datCallback(Member2 func, Base* base, void* param);

        void Call(void* param = nullptr);
        
        union {
            void(* invoke_)(void*, void*) {};
            size_t raw_;
        };

        union {
            alignas(void*) char data_[8] {};

            struct {
                Base* base_;
                void* param_;
            } game_;
        };

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<datCallback>("datCallback")
                .endClass();
        }

    private:
        explicit datCallback(size_t type, void* func, Base* base, void* param);
    };

    ASSERT_SIZEOF(datCallback, 0xC);
}