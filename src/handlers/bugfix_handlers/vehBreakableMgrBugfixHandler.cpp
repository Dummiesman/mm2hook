#include "vehBreakableMgrBugfixHandler.h"

using namespace MM2;

/*
    vehBreakableMgrBugfixHandler
*/

bool vehBreakableMgrBugfixHandler::GetPivotHook(MM2::Matrix34& mtx, const char* basename, const char* file)
{
    if (GetPivot(mtx, basename, file))
    {
        return true;
    }
    else 
    {
        memset(&mtx, 0x00, sizeof(Matrix34));
        return false;
    }
}

void vehBreakableMgrBugfixHandler::Install()
{
    InstallCallback("vehBreakableMgr::Create", "Fix undefined behavior with uninitialized memory.",
        &vehBreakableMgrBugfixHandler::GetPivotHook, {
            cb::call(0x4D8629)
        }
    );
}