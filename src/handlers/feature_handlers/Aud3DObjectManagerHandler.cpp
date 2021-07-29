#include "Aud3DObjectManagerHandler.h"

using namespace MM2;

/*
    Aud3DObjectManager
*/

void Aud3DObjectManagerHandler::InitAmbObjContainer(LPCSTR name) {
    string_buf<80> buffer("%sambientcontainer", MMSTATE->CityName);

    //don't continue if it doesn't exist
    if (!datAssetManager::Exists("aud\\ambient", buffer, "csv"))
        return;

    LPCSTR szAmbientSFX = buffer;
    LogFile::Format("AmbientContainer: %s\n", szAmbientSFX);

    //call original
    hook::Thunk<0x50F650>::Call<void>(this, szAmbientSFX);
}

void Aud3DObjectManagerHandler::Install() {
    InstallPatch("Allows for custom positional ambient effects in addon cities.", { 0x90, 0x90 }, {
        0x404059,
        });

    InstallCallback("mmPlayer::Init", "Allows for custom positional ambient effects in addon cities.",
        &InitAmbObjContainer, {
            cb::call(0x404082),
        }
    );
}