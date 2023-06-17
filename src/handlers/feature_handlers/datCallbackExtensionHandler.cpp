#include "datCallbackExtensionHandler.h"

using namespace MM2;

/*
    datCallbackExtensionHandler
*/

template <typename Class>
struct class_proxy
{
    template <typename... Args>
    Class* ctor(Args... args)
    {
        return new (this) Class(std::forward<Args>(args)...);
    }
};

void datCallbackExtensionHandler::Install() {
    InstallCallback("datCallback", "Fix addresses", &class_proxy<datCallback>::ctor<>, { cb::jmp(0x4C7A40) });

    InstallCallback("datCallback", "Fix addresses", &class_proxy<datCallback>::ctor<datCallback::Static0>, { cb::jmp(0x4C7AC0) });
    InstallCallback("datCallback", "Fix addresses", &class_proxy<datCallback>::ctor<datCallback::Static1, void*>, { cb::jmp(0x4C7AF0) });
    InstallCallback("datCallback", "Fix addresses", &class_proxy<datCallback>::ctor<datCallback::Static2, void*>, { cb::jmp(0x4C7B20) });

    InstallCallback("datCallback", "Fix addresses", &class_proxy<datCallback>::ctor<datCallback::Member0, Base*>, { cb::jmp(0x4C7A50) });
    InstallCallback("datCallback", "Fix addresses", &class_proxy<datCallback>::ctor<datCallback::Member1, Base*, void*>, { cb::jmp(0x4C7A80) });
    InstallCallback("datCallback", "Fix addresses", &class_proxy<datCallback>::ctor<datCallback::Member2, Base*, void*>, { cb::jmp(0x4C7AA0) });

    InstallCallback("datCallback", "Fix addresses", &datCallback::Call, { cb::jmp(0x4C7B50) });
}