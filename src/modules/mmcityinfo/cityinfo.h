#pragma once

namespace MM2
{
    // Forward declarations
    class mmCityInfo;

    // External declarations


    // Class definitions
    class mmCityInfo {
    private:
        char localisedName[40];
        char mapName[40];
        char raceDir[40];

        char *blitzNames;
        char *checkpointNames;
        char *circuitNames;
        
        BOOL isValid;

        int blitzCount;
        int checkpointCount;
        int circuitCount;

        int _unk[2]; // ???

        inline char * getRaceNamesPtr(int mode) {
            switch (mode) {
                case 1: return checkpointNames;
                case 3: return circuitNames;
                case 4: return blitzNames;
            }
            return NULL;
        }

        inline int getRaceNamesCount(int mode) {
            switch (mode) {
                case 1: return checkpointCount;
                case 3: return circuitCount;
                case 4: return blitzCount;
            }
            return -1;
        }
    public:
        AGE_API mmCityInfo(void) {
            scoped_vtable x(this);
            hook::Thunk<0x52A540>::Call<void>(this);
        }

        virtual AGE_API ~mmCityInfo(void) {
            scoped_vtable x(this);
            hook::Thunk<0x52A560>::Call<void>(this);
        }

        inline char * GetLocalisedName(void) {
            return localisedName;
        }

        inline char * GetMapName(void) {
            return mapName;
        }

        inline char * GetRaceDir(void) {
            return raceDir;
        }

        inline int GetRaceNames(int mode, char *buffer) {
            char *names = getRaceNamesPtr(mode);

            if (names != NULL) {
                strcpy(buffer, names);
                return getRaceNamesCount(mode);
            }

            return -1;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmCityInfo>("mmCityInfo")
                .endClass();
        }
    };
}