#include "mmDirSndHandler.h"

using namespace MM2;

/*
    mmDirSndHandler
*/

mmDirSnd* mmDirSndHandler::Init(ulong sampleRate, bool enableStero, int p3, int volume, char *deviceName, short p6, short p7) {
    if (*deviceName == '\0') {
        deviceName = "Primary Sound Driver";

        LogFile::WriteLine("[mmDirSnd::Init]: Using primary sound driver");
    }

    /*
    TODO:

    - Set sampling rate (see: AudManager::SetBitDepthAndSampleRate(int bitDepth, ulong samplingRate))
    - Redo SetPrimaryBufferFormat to set sampleSize? (see: DirSnd::SetPrimaryBufferFormat(ulong sampleRate, bool allowStero))
    */
    return mmDirSnd::Init(48000, enableStero, p3, volume, deviceName, p6, p7);
}

void mmDirSndHandler::Install() {
    InstallCallback("mmDirSnd::Init", "Fixes no sound issue on startup.",
        &Init, {
            cb::call(0x51941D),
        }
    );
}

