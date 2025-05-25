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

        char * GetBaseName() {
            return baseName;
        }

        char * GetDescription() {
            return description;
        }

        char * GetColors() {
            return colors;
        }

        int GetFlags() const {
            return flags;
        }

        unsigned int GetTuningCRC() const {
            return tuningCrc;
        }

        bool GetLocked() const {
            return isLocked == TRUE;
        }

        void SetLocked(bool lock) {
            isLocked = (lock) ? TRUE : FALSE;
        }

        bool IsValid() const
        {
            return isValid;
        }

        int GetHorsepower() const
        {
            return horsepower;
        }

        int GetMass() const
        {
            return mass;
        }

        int GetDurability() const
        {
            return durability;
        }
        
        int GetTopSpeed() const
        {
            return topSpeed;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmVehInfo>("mmVehInfo")
                .addPropertyReadOnly("Colors", &GetColors)
                .addPropertyReadOnly("Basename", &GetBaseName)
                .addPropertyReadOnly("Description", &GetDescription)
                .addProperty("Locked", &GetLocked, &SetLocked)
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