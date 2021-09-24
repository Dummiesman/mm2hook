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

        //helpers
        void SetBaseName(LPCSTR basename) {
            strcpy_s(this->baseName, 40, basename);
        }

        void SetDescription(LPCSTR description) {
            strcpy_s(this->description, 80, description);
        }

        void SetColors(LPCSTR colors) {
            strcpy_s(this->colors, 100, colors);
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmVehInfo>("mmVehInfo")
                .addProperty("Colors", &GetColors, &SetColors)
                .addProperty("Basename", &GetBaseName, &SetBaseName)
                .addProperty("Description", &GetDescription, &SetDescription)
                .addVariableRef("Flags", &mmVehInfo::flags)
                .addVariableRef("Order", &mmVehInfo::order)
                .addVariableRef("UnlockFlags", &mmVehInfo::unlockFlags)
                .addVariableRef("UnlockScore", &mmVehInfo::unlockScore)
                .addVariableRef("Horsepower", &mmVehInfo::horsepower)
                .addVariableRef("TopSpeed", &mmVehInfo::topSpeed)
                .addVariableRef("Durability", &mmVehInfo::durability)
                .addVariableRef("Mass", &mmVehInfo::mass)
                .addVariableRef("UIDist", &mmVehInfo::uiDist)
                .addPropertyReadOnly("TuningCRC", &GetTuningCRC)
                .endClass();
        }
    };
}