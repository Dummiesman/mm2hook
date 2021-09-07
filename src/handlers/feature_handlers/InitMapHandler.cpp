#include "InitMapHandler.h"

using namespace MM2;

/*
    InitMapHandler
*/

static bool mapInitialized = false;

struct MapSymbol
{
    char* Name;
    int Address;
};

void InitMapHandler::AllocMemory(int start, int count)
{
    int total = start + count;
    MapSymbol* newMemory = (MapSymbol*)GlobalAlloc(0x40, total * sizeof(MapSymbol));
    MapSymbol** symbols = (MapSymbol**)0x6A3CE4;

    if (*symbols != nullptr) 
    {
        //copy 
        for (int i = 0; i < start; i++) 
            newMemory[i] = MapSymbol(*((*symbols) + i));

        //free
        GlobalFree(*symbols);
    }

    *symbols = newMemory;
}

void InitMapHandler::LoadMapFile(LPCSTR filepath, int base, bool main)
{
    //our data
    char buf[1024];
    char currentLine[1024];
    char symbolName[768];
    char* allNames;
    int symbolAddress = 0;

    char** timestamp = (char**)0x6A3C64;
    int* symbolsCount = (int*)0x6A3CE8;
    MapSymbol** symbols = (MapSymbol**)0x6A3CE4;

    int oldSymbolCount = *symbolsCount;

    if (main)
        *timestamp = "NO TIMESTAMP";

    //read file
    for (int stage = 1; stage <= 2; stage++) 
    {
        auto file = fopen(filepath, "r");
        if (file == nullptr) {
            string_buf<256> buf("Couldn't open '%s'", filepath);
            OutputDebugStringA((LPCSTR)buf);
            return;
        }

        int parsedSymbolCount = 0;
        int parsedStringLength = 0;

        //Get timestamp
        while (true) 
        {
            fgets(buf, sizeof(buf), file);
            auto newLinePos = strchr(buf, '\n');
            if (newLinePos == nullptr)
                break;

            int lineLength = newLinePos - buf;
            memcpy(currentLine, buf, lineLength);
            currentLine[lineLength] = '\0';

            if (strstr(currentLine, "Timestamp is") && main)
                strcpy(*timestamp, currentLine);
            if (strstr(currentLine, "Publics by Value"))
                break;
        }

        //The rest
        while (true)
        {
            fgets(buf, sizeof(buf), file);
            auto newLinePos = strchr(buf, '\n');
            if (newLinePos == nullptr) 
                break;

            int lineLength = newLinePos - buf;
            memcpy(currentLine, buf, lineLength);
            currentLine[lineLength] = '\0';

            if (!strncmp(currentLine, " 0001:", 6u) && sscanf(currentLine, "%*s %s %x", &symbolName, &symbolAddress) == 2)
            {
                if (stage == 2)
                {
                    auto symbolNamePtr = &allNames[parsedStringLength];
                    strcpy(&allNames[parsedStringLength], symbolName);

                    auto symbol = (*symbols) + (parsedSymbolCount + oldSymbolCount);
                    symbol->Name = symbolNamePtr;
                    symbol->Address = symbolAddress + base;
                }
                else
                {
                    *symbolsCount = (*symbolsCount) + 1;
                }
                parsedStringLength += strlen(symbolName) + 1;
                parsedSymbolCount++;
            }
            else if (!strncmp(currentLine, " 0002:", 6u))
            {
                break;
            }
        }

        //allocate memory for symbols
        if (stage == 1) 
        {
            AllocMemory(oldSymbolCount, parsedSymbolCount);
            allNames = (char*)GlobalAlloc(64, parsedStringLength);
            Errorf("%d symbols parsed from map file.", parsedSymbolCount);
        }

        fclose(file);
    }
}

void InitMapHandler::InitMap()
{
    if (mapInitialized)
        return;
    mapInitialized = true;

    //load exe map
    char buf[128];
    GetModuleFileNameA(0, buf, sizeof(buf));
    
    std::string moduleFilename = std::string(buf);
    std::string moduleMapFilename = moduleFilename;
    
    int dotPos = moduleFilename.find_last_of('.');
    if (dotPos != std::string::npos)
        moduleMapFilename = moduleMapFilename.substr(0, dotPos);
    moduleMapFilename += ".map";
    
    LoadMapFile(moduleMapFilename.c_str(), 0x00, true);

    //load hook map
    //base is our handle - our image base
    int slashPos = moduleFilename.find_last_of('\\');
    if (slashPos != std::string::npos) 
    {
        std::string hookMapFilename = moduleFilename.substr(0, slashPos + 1) + "MM2Hook.map";
        LoadMapFile(hookMapFilename.c_str(), (int)(GetModuleHandle("dinput.dll")) - 0x10000000, false);
    }
}

void InitMapHandler::Install()
{
    InstallCallback("InitMap", "Allow loading multiple map files.",
        &InitMap, {
            cb::call(0x4C7120),
            cb::call(0x4C7476),
        }
    );
}
