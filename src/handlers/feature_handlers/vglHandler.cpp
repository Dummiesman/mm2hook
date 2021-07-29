#include "vglHandler.h"
#include "sdlPage16Handler.h"

using namespace MM2;

// HACK HACK HACK!
// Doesn't work well for subway stations :(
bool vglHandler::insideTunnel = false;

/*
    vglHandler
*/

unsigned int vglHandler::vglColor = 0;
unsigned int vglHandler::vglCalculatedColor = 0xFFFFFFFF;
ColorARGB vglHandler::vglResultColor;

unsigned int GetAdjustedColor(gfxDrawMode drawMode, unsigned int color) {
    if (sdlPage16Handler::blockPtr != NULL)
    {
        // fullbright
        static const ColorARGB sdlTunnelColor = { 255, 255, 255, 255 };

        // we can finally check for attributes!
        short* attr = (short*)sdlPage16Handler::attributePtr;

        int type = ((*attr >> 3) & 0xF);
        int subtype = (*attr & 0x7);

        /*
        Due to limitations in Angel's SDL format,
        this is the best way we can find "tunnels".
        It's not guaranteed to be 100% accurate,
        but it works pretty well for what we've got.
        */
        if (type == 9) {
            // flag location varies on subtype
            // there's a subtype 2 but it's never (?) used
            int flags = (subtype == 0) ? attr[2]
                : (subtype == 3) ? attr[1] : 0;

            // do we have a roof?
            // note: junctions can't have curved roofs,
            // but this is a quicker way of doing things
            if ((flags & 0x8) || (flags & 0x100)) {
                /* junction */
                if (subtype == 0) {
                    int wallVisInfos = attr[1];
                    int unk = attr[5];

                    for (int i = 0; i < wallVisInfos; i++) {
                        int info = attr[6 + i];

                        if (info > 0) {
                            // found a wall!
                            // we're probably in a tunnel :)
                            vglHandler::insideTunnel = true;
                            break;
                        }
                    }
                }
                /* road */
                else if (subtype == 3) {
                    // does tunnel have walls?
                    vglHandler::insideTunnel = (flags & 0x3) ? true : false;
                }
                /* unknown */
                else {
                    // probably not a tunnel
                    vglHandler::insideTunnel = false;
                }
            }
            else {
                // tunnels usually have roofs...
                // so we're probably not in a tunnel
                vglHandler::insideTunnel = false;
            }
        }

        // use fullbright inside tunnels instead of the calculated colors below
        // will still preserve shading as well
        if (vglHandler::insideTunnel)
            return sdlPage16::GetShadedColor(color, sdlTunnelColor.color);
    }

    return sdlPage16::GetShadedColor(color, vglHandler::vglResultColor.color);
}

void vglHandler::vglBeginShaded(gfxDrawMode drawMode, int p1) {
    // Save current vgl color
    vglColor = *vglCurrentColor;

    vglCalculatedColor = GetAdjustedColor(drawMode, vglColor);
    vglCurrentColor = vglCalculatedColor;

    vglBegin(drawMode, p1);
}

void vglHandler::vglEndShaded(void) {
    // restore color
    vglCurrentColor = vglColor;

    vglEnd();
}

void vglHandler::Install() {
    LogFile::WriteLine(" - Installing shading fix...");

    auto_ptr vglBeginCB = &vglBeginShaded;
    auto_ptr vglEndCB = &vglEndShaded;

    Installf(" - vglBeginCB: %08X\n", vglBeginCB);
    Installf(" - vglEndCB: %08X\n", vglEndCB);

    // use a custom struct to make the process easier
    // this allows us to have an entry representing each "frame" (vglBegin/vglEnd)
    // and cuts down on the amount of time it takes to add a new entry
    struct vgl_pair {
        unsigned int begin; // vglBegin
        unsigned int end;   // vglEnd
    };

    std::initializer_list<vgl_pair> vglCBs = {
        /* ------------ sdlPage16::Draw ------------- */
        { 0x448424, 0x4485D3 },{ 0x448697, 0x448B82 },{ 0x448903, 0x448D8C },{ 0x448BFD, 0x448FB7 },
        { 0x448DE4, 0x449219 },{ 0x44902A, 0x449480 },{ 0x4492A4, 0x44963E },{ 0x4494C3, 0x44983C },
        { 0x4496A5, 0x4499D4 },{ 0x44986B, 0x449BAA },{ 0x449A13, 0x449D42 },{ 0x449BD9, 0x449F5A },
        { 0x449D82, 0x44A146 },{ 0x449F67, 0x44A3F8 },{ 0x44A21C, 0x44A5BF },{ 0x44A444, 0x44A7C0 },
        { 0x44A629, 0x44A958 },{ 0x44A7EF, 0x44AB2E },{ 0x44A997, 0x44ACC6 },{ 0x44AB5D, 0x44AEBC },
        { 0x44AD06, 0x44B083 },{ 0x44AECA, 0x44B23D },{ 0x44B0EC, 0x44B394 },{ 0x44B24B, 0x44B531 },
        { 0x44B3B6, 0x44B6E1 },{ 0x44B557, 0x44B895 },{ 0x44B6F3, 0x44BA7C },{ 0x44B8F1, 0x44BC03 },
        { 0x44BA8A, 0x44BE8E },{ 0x44BC29, 0x44C118 },{ 0x44BE9C, 0x44C3EA },{ 0x44C136, 0x44C638 },
        { 0x44C40C, 0x44C77A },{ 0x44C64A, 0x44C989 },{ 0x44C7C0, 0x44CC44 },{ 0x44CAD6, 0x44CE63 },
        { 0x44CCF5, 0x44D04E },{ 0x44CF6D, 0x44D403 },{ 0x44D0D4, 0x44D780 },{ 0x44D5F7, 0x44D8E9 },
        { 0x44D789, 0x44E014 },{ 0x44DC55, 0x44E131 },{ 0x44E050, 0x44E22C },{ 0x44E14B, 0x44E661 },
        { 0x44E2A3, 0x44E785 },{ 0x44E69D, 0x44E886 },{ 0x44E79E, 0x44EB82 },{ 0x44EAA0, 0x44EDC3 },
        { 0x44EBA5, 0x44F0B9 },{ 0x44EFD0, 0x44F316 },{ 0x44F0DC, 0x44F64C },{ 0x44F588, 0x44FB9D },
        { 0x44F7E2, 0x44FD30 },{ 0x44FC1E, 0x44FE4E },{ 0x44FDD4, 0x44FFB3 },{ 0x44FF10, 0x450162 },
        { 0x450085, 0x450390 },{ 0x450269, 0x45078C },
        /* ------------------------------------------- */
        { 0x443B9D, 0x443DCC }, // dgRoadDecalInstance
        { 0x57AC4A, 0x57AD41 }, // ped LODs
    };

    // mostly copied from InstallCallback
    for (auto pair : vglCBs)
    {
        auto begin = pair.begin;
        auto end = pair.end;

        InstallCallback(vglBeginCB, cb::call(begin));
        InstallCallback(vglEndCB, cb::call(end));

        Installf("   - { vglBegin: %08X, vglEnd: %08X }\n", begin, end);
    }
}

