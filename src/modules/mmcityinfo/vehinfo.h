#pragma once

namespace MM2
{
    // Forward declarations
    class mmVehInfo;

    // External declarations


    // Class definitions
    class mmVehInfo {
    private:
        char baseName[40];
        char description[80];
        char colors[100];
        int flags;
        int order;
        BOOL isValid;
        int unlockScore;
        int unlockFlags;
        BOOL isLocked;
        int horsepower;
        int topSpeed;
        int durability;
        int mass;
        int rewardFlags;
        int unk_10C;
        float scoringBias;
        float uiDist;
        float unk_118;
        float unk_11C;
        unsigned int tuningCrc;
    public:
        AGE_API mmVehInfo(void) {
            scoped_vtable x(this);
            hook::Thunk<0x529720>::Call<void>(this);
        }

        virtual AGE_API ~mmVehInfo(void) {
            scoped_vtable x(this);
            hook::Thunk<0x529770>::Call<void>(this);
        }

        inline char * GetBaseName(void) {
            return baseName;
        }

        inline char * GetDescription(void) {
            return description;
        }

        inline char * GetColors(void) {
            return colors;
        }

        inline int GetFlags(void) {
            return flags;
        }

        inline unsigned int GetTuningCRC(void) {
            return tuningCrc;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmVehInfo>("mmVehInfo")
                .addPropertyReadOnly("Colors", &GetColors)
                .addPropertyReadOnly("Basename", &GetBaseName)
                .addPropertyReadOnly("Description", &GetDescription)
                .addVariable("Flags", &mmVehInfo::flags, false)
                .addVariable("Order", &mmVehInfo::order, false)
                .addVariable("UnlockFlags", &mmVehInfo::unlockFlags, false)
                .addVariable("UnlockScore", &mmVehInfo::unlockScore, false)
                .addVariable("Horsepower", &mmVehInfo::horsepower, false)
                .addVariable("TopSpeed", &mmVehInfo::topSpeed, false)
                .addVariable("Durability", &mmVehInfo::durability, false)
                .addVariable("Mass", &mmVehInfo::mass, false)
                .addVariable("UIDist", &mmVehInfo::uiDist, false)
                .addPropertyReadOnly("TuningCRC", &GetTuningCRC)
                .endClass();
        }
    };
}