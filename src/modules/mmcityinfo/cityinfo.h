#pragma once

namespace MM2
{
    // Forward declarations
    class mmCityInfo;

    // External declarations


    // Class definitions
    class mmCityInfo {
    private:
        char localizedName[40];
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

        inline std::vector<std::string> getRaceNames(dgGameMode mode)
        {
            std::vector<std::string> list;
            if (mode == dgGameMode::CrashCourse)
            {
                if (getRaceNamesCount(mode) > 0)
                {
                    int startIndex = 0x214;         // LONDON_CRASHNAME_0
                    int endIndex = startIndex + 12; // LONDON_CRASHNAME_12
                    if (!strcmp(mapName, "sf"))
                    {
                        startIndex += 13;
                        endIndex += 13;
                    }

                    for (int i = startIndex; i <= endIndex; i++)
                    {
                        list.push_back(std::string(AngelReadString(i)));
                    }
                }
            }
            else 
            {
                auto namesPtr = getRaceNamesPtr(mode);
                if (namesPtr != nullptr)
                {
                    std::string names = std::string(namesPtr);
                    std::string curName = "";
                    for (unsigned int i = 0; i < names.length(); i++) 
                    {
                        auto c = names[i];
                        if (c != '|')
                        {
                            curName += c;
                        }
                        if(c == '|' || i == names.length() - 1)
                        {
                            list.push_back(curName);
                            curName = "";
                        }
                    }
                }
            }
            return list;
        }

        inline const char * getRaceNamesPtr(dgGameMode mode) {
            switch (mode) {
                case dgGameMode::Checkpoint: return checkpointNames;
                case dgGameMode::Circuit:    return circuitNames;
                case dgGameMode::Blitz:      return blitzNames;
            }
            return nullptr;
        }

        inline int getRaceNamesCount(dgGameMode mode) {
            switch (mode) {
                case dgGameMode::CrashCourse:
                    if (!strcmp(mapName, "sf") || !strcmp(mapName, "london"))
                        return 12;
                    return 0;
                case dgGameMode::Checkpoint: return checkpointCount;
                case dgGameMode::Circuit:    return circuitCount;
                case dgGameMode::Blitz:      return blitzCount;
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

        inline const char * GetLocalizedName(void) {
            return localizedName;
        }

        inline const char * GetMapName(void) {
            return mapName;
        }

        inline const char * GetRaceDir(void) {
            return raceDir;
        }

        //lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass<mmCityInfo>("mmCityInfo")
                .addPropertyReadOnly("LocalizedName", &GetLocalizedName)
                .addPropertyReadOnly("MapName", &GetMapName)
                .addPropertyReadOnly("RaceDir", &GetRaceDir)
                .addVariable("NumBlitzRaces", &mmCityInfo::blitzCount, false)
                .addVariable("NumCheckpointRaces", &mmCityInfo::checkpointCount, false)
                .addVariable("NumCircuitRaces", &mmCityInfo::circuitCount, false)
                .addFunction("GetRaceNames", &getRaceNames)
                .endClass();
        }
    };
}