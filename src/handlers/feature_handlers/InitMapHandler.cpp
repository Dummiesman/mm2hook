#include "InitMapHandler.h"
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace MM2;

/*
    InitMapHandler
*/

static bool mapInitialized = false;
static std::vector<std::string> symbolNames;
static std::vector<unsigned int> symbolAddresses;

struct MapSymbol
{
    const char* Name;
    unsigned int Address;
};

void InitMapHandler::CopyToMM2()
{
    MapSymbol** symbols = (MapSymbol**)0x6A3CE4;
    if (*symbols != nullptr)
    {
        GlobalFree(*symbols);
    }
    *symbols = (MapSymbol*)GlobalAlloc(0x40, symbolNames.size() * sizeof(MapSymbol));


    for(unsigned int i=0; i < symbolNames.size(); i++)
    {
        auto symbol = (*symbols) + i;
        symbol->Address = symbolAddresses[i];
        symbol->Name = symbolNames[i].c_str();
    }
}

void InitMapHandler::LoadMapFile(LPCSTR filepath, unsigned int offset, bool main)
{
    //our data
    char** timestamp = (char**)0x6A3C64;
    unsigned int* symbolsCount = (unsigned int*)0x6A3CE8;

    unsigned int temp = 0;
    std::string line;
    std::string symbolName;
    bool foundCodeOffset = false;

    int oldSymbolCount = symbolNames.size();
    
    if (main)
        *timestamp = "NO TIMESTAMP";

    //read file
    std::ifstream stream(filepath);
    if (!stream.is_open()) 
    {
        string_buf<256> buf("Couldn't open '%s'", filepath);
        OutputDebugStringA((LPCSTR)buf);
        return;
    }

    int parsedSymbolCount = 0;
    int parsedStringLength = 0;
    int loadAddress = 0;

    //Get header info
    while (std::getline(stream, line))
    {
        if (main && line.find("Preferred load address") != std::string::npos) 
        {
            offset += std::stoi(line.substr(line.rfind(" ") + 1), 0, 16);
        }
        else if (main && line.find("Timestamp is") != std::string::npos)
        {
            strcpy(*timestamp, line.c_str());
        }
        else if (!foundCodeOffset && line.find("0001:") != std::string::npos)
        {
            line = line.substr(6);
            std::istringstream iss(line);
            
            iss >> std::hex >> temp;
            offset += temp;
            foundCodeOffset = true;
        }
        else if (line.find("Publics by Value") != std::string::npos)
        {
            break;
        }
    }

    //The rest
    while (std::getline(stream, line))
    {
        if (line.find("0001:") != std::string::npos)
        {
            line = line.substr(6);
            std::istringstream iss(line);

            iss >> std::hex >> temp;
            iss >> symbolName;

            symbolNames.push_back(symbolName);
            symbolAddresses.push_back(temp + offset);
        }
        else if (line.find("0002:") != std::string::npos)
        {
            break;
        }
    }

    // format for MM2's structures
    *symbolsCount = symbolNames.size();
    CopyToMM2();
    Errorf("%d symbols parsed from map file.", symbolNames.size() - oldSymbolCount);
    stream.close();
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
    
    LoadMapFile(moduleMapFilename.c_str(), 0x00000000, true);

    //load hook map
    int slashPos = moduleFilename.find_last_of('\\');
    if (slashPos != std::string::npos) 
    {
        std::string hookMapFilename = moduleFilename.substr(0, slashPos + 1) + "MM2Hook.map";
        Warningf("dinput entrypoint %x", GetModuleHandle("dinput.dll"));
        LoadMapFile(hookMapFilename.c_str(), (unsigned int)GetModuleHandle("dinput.dll"), false);
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
