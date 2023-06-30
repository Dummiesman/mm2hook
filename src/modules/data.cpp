#include <modules\data.h>
#include <mm2_lua.h>

using namespace MM2;

/*
    datAssetManager
*/
declfield(datAssetManager::sm_Path)(0x6A3B8C);

/*
    datCallback
*/
declfield(datCallback::NullCallback)(0x6A3D10);

template <typename To, typename From>
inline To bit_cast(const From& src) noexcept
{
    static_assert(sizeof(To) == sizeof(From), "sizeof(To) != sizeof(From)");
    static_assert(std::is_trivially_copyable<From>::value, "From is not trivially copyable");
    static_assert(std::is_trivially_copyable<To>::value, "To is not trivially copyable");

    typename std::aligned_storage<sizeof(To), alignof(To)>::type dst;
    memcpy(&dst, &src, sizeof(dst));
    return reinterpret_cast<To&>(dst);
}

datCallback* datCallback::CreateParamaterlessLuaCallback(const LuaRef func)
{
    if (func.type() == LuaIntf::LuaTypeID::NIL)
        return datCallback::NullCallback;

    func.checkFunction();
    func.pushToStack();
    int m_ref = luaL_ref(func.state(), LUA_REGISTRYINDEX);

    assert(MM2Lua::GetState() == func.state());

    return new datCallback([m_ref](void* param) {
        auto state = MM2Lua::GetState();
        state.getRef(m_ref);

        auto ref = state.popValue<LuaRef>();
        MM2Lua::TryCallFunction(ref);
    });
}

datCallback::datCallback(Static0 func)
    : datCallback(CB_TYPE_PARAM0, func, nullptr, nullptr)
{
}

datCallback::datCallback(Static1 func, void* param)
    : datCallback(CB_TYPE_PARAM1, func, nullptr, param)
{
}

datCallback::datCallback(Static2 func, void* param)
    : datCallback(CB_TYPE_PARAM2, func, nullptr, param)
{
}

datCallback::datCallback(Member0 func, Base* base)
    : datCallback(CB_TYPE_PARAM0, bit_cast<void*>(func), base, nullptr)
{
    if (!base)
        Quitf("Can't have callback to member function with nil 'this'");
}

datCallback::datCallback(Member1 func, Base* base, void* param)
    : datCallback(CB_TYPE_PARAM1, bit_cast<void*>(func), base, param)
{
    if (!base)
        Quitf("Can't have callback to member function with nil 'this'");
}

datCallback::datCallback(Member2 func, Base* base, void* param)
    : datCallback(CB_TYPE_PARAM2, bit_cast<void*>(func), base, param)
{
    if (!base)
        Quitf("Can't have callback to member function with nil 'this'");
}

datCallback::datCallback(size_t type, void* func, Base* base, void* param)
    : raw_(bit_cast<std::size_t>(func))
    , game_ { base, param }
{
    if (raw_ & type)
        Quitf("Legacy callbacks do not support hook functions");

    raw_ |= type;
}

void datCallback::Call(void* param)
{
    if (raw_ & CB_TYPE_GAME) {
        std::size_t type = raw_ & CB_TYPE_MASK;
        std::size_t func = raw_ & CB_FUNC_MASK;

        if (game_.base_) {
            switch (type) {
            case CB_TYPE_PARAM0:
                (game_.base_->*bit_cast<Member0>(func))();
                break;

            case CB_TYPE_PARAM1:
                (game_.base_->*bit_cast<Member1>(func))(game_.param_);
                break;

            case CB_TYPE_PARAM2:
                (game_.base_->*bit_cast<Member2>(func))(game_.param_, param);
                break;
            }
        } else {
            switch (type) {
            case CB_TYPE_PARAM0:
                bit_cast<Static0>(func)();
                break;

            case CB_TYPE_PARAM1:
                bit_cast<Static1>(func)(game_.param_);
                break;

            case CB_TYPE_PARAM2:
                bit_cast<Static2>(func)(game_.param_, param);
                break;
            }
        }
    } else if (invoke_) {
        invoke_(data_, param);
    }
}