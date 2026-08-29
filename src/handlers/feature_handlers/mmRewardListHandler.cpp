#include "mmRewardListHandler.h"

using namespace MM2;

/*
    mmRewardListHandler
*/

void mmRewardListHandler::Init(int capacity)
{
    // allow up to 256 rewards
    capacity = 256;
    hook::Thunk<0x523930>::Call<void>(this, capacity);
}

void mmRewardListHandler::Install()
{
    InstallCallback("mmRewardList::Init", "",
        &mmRewardListHandler::Init, {
            cb::call(0x523CD3),
            cb::call(0x523EA9),
        }
    );
}

