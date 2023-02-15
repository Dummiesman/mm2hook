#include "TextureVariantHandler.h"
#include <events\dispatcher.h>
#include <vector>
using namespace MM2;

/*
    TextureVariantHandler
*/
struct variant_info {
    const char *suffix;
    bool canDesaturate;
};
std::vector<variant_info> variant_infos;
bool desaturateDefaultTextures = false;

static gfxImage * (*DefaultLoadImage)(const char *, bool);
static gfxImage * (*DefaultPrepareImage)(gfxImage*, const char *, bool);

hook::Type<bool> EnableTextureVariantHandler(0x6276EC);
hook::Type<bool> AllowDesaturatedTextureVariants(0x6276ED);

std::vector<std::string> split(std::string str, std::string token) {
    std::vector<std::string>result;
    while (str.size()) {
        int index = str.find(token);
        if (index != std::string::npos) {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)result.push_back(str);
        }
        else {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

void TextureVariantHandler::Reset() {
    variant_infos.clear();
    desaturateDefaultTextures = false;
}

void TextureVariantHandler::InitVariantData() {
    variant_infos.clear();
    auto parser = datParser("OwO");

    //records
    char textureVariants[100];
    char textureLuminances[100];
    int defaultSaturated = (dgStatePack::Instance->TimeOfDay != 3) ? 1 : 0;
    parser.AddString("TextureVariants", &textureVariants);
    parser.AddString("TextureLuminances", &textureLuminances);
    parser.AddValue("DefaultLuminance", &defaultSaturated);

    //load file
    int fileId = (dgStatePack::Instance->TimeOfDay * 4) + dgStatePack::Instance->WeatherType;
    string_buf<16> buffer("td%02d", fileId);
    LPCSTR tdExtension = buffer;

    Displayf("Attempting to load texture definition file %s", tdExtension);
    bool loadResult = parser.Load("city", MMSTATE->CityName, tdExtension);

    //deal with loaded values
    desaturateDefaultTextures = defaultSaturated == 0;

    auto tVarStd = std::string(textureVariants);
    auto tLumStd = std::string(textureLuminances);
    auto tVarVec = split(tVarStd, "|");
    auto tLumVec = split(tLumStd, "|");

    if (loadResult && tVarStd.length() > 0) {
        for (size_t i = 0; i < tVarVec.size(); i++) {
            auto vInfo = variant_info();

            auto suffix = tVarVec[i];
            auto suffix_copy = new char[suffix.length() + 1];
            memcpy(suffix_copy, suffix.c_str(), suffix.length() + 1);
            vInfo.suffix = suffix_copy;

            //the luminence array may be a different size or missing altogether
            if (i < tLumVec.size()) {
                auto canDesat = tLumVec[i];
                vInfo.canDesaturate = atof(canDesat.c_str()) < 0.5;
            }
            else {
                vInfo.canDesaturate = false;
            }

            Warningf("Pushing a new variant with suffix %s and desaturate %s", vInfo.suffix, vInfo.canDesaturate ? "true" : "false");
            variant_infos.push_back(vInfo);
        }
    }

    //add defaults
    if (dgStatePack::Instance->WeatherType == 3) {
        auto rVariant = variant_info();
        rVariant.suffix = "fa";
        rVariant.canDesaturate = dgStatePack::Instance->TimeOfDay == 3;
        variant_infos.push_back(rVariant);
    }
    if (dgStatePack::Instance->TimeOfDay == 3) {
        auto nVariant = variant_info();
        nVariant.suffix = "ni";
        nVariant.canDesaturate = false;
        variant_infos.push_back(nVariant);
    }

    //call vehCarAudioContainer::InitStatics, which we hooked
    hook::StaticThunk<0x4D0FF0>::Call<void>();
}

static bool TryLoadTexVariant(const char* textureName, const char* variant, bool mipmaps, gfxImage** pgfxImage)
{
    string_buf<64> textureVariant("%s_%s", textureName, variant);
    gfxImage* variantTex = DefaultLoadImage(textureVariant, mipmaps);

    if (variantTex != nullptr) {
        //Warningf("[LoadTextureVariant]: Using '%s' variant for texture '%s' (buffer is %s)", variant, textureName, (LPCSTR)textureVariant);
        *pgfxImage = variantTex;
        return true;
    }

    return false;
}

static bool TextureVariantExists(const char* textureName, const char* variant) {
    string_buf<128> textureVariant("%s_%s", textureName, variant);
    bool exists = datAssetManager::Exists("texture", textureVariant, "tex");
    if (exists)
        return true;
    exists = datAssetManager::Exists("texture", textureVariant, "tga");
    if (exists)
        return true;
    exists = datAssetManager::Exists("texture", textureVariant, "bmp");
    return exists;
}

static void Desaturate(gfxImage* result) {
    for (gfxImage* image = result; image != nullptr; image = image->Next) {
        // DesaturateTextureVariant
        hook::StaticThunk<0x442FB0>::Call<void>(image);
    }
}

gfxImage * TextureVariantHandler::LoadTextureVariant(const char *textureName, bool mipmaps)
{
    if (EnableTextureVariantHandler)
    {
        gfxImage *variantTex = nullptr;
        for (size_t i = 0; i < variant_infos.size(); i++) {
            if (TryLoadTexVariant(textureName, variant_infos[i].suffix, mipmaps, &variantTex))
                return variantTex;
        }
    }

    auto defaultTex = DefaultLoadImage(textureName, mipmaps);
    return defaultTex;
}

gfxImage * TextureVariantHandler::PrepareTextureVariant(gfxImage* image, const char *textureName, bool mipmaps)
{
    if (EnableTextureVariantHandler)
    {
        //check if this variant is handled manually
        for (size_t i = 0; i < variant_infos.size(); i++) {
            if (TextureVariantExists(textureName, variant_infos[i].suffix)) {
                if (variant_infos[i].canDesaturate)
                    Desaturate(image);

                return image;
            }
        }
    }

    //no variant
    if (desaturateDefaultTextures) {
        Desaturate(image);
    }
    return image;
}

void TextureVariantHandler::InstallTextureVariantHandler()
{
    if (DefaultLoadImage == nullptr) {
        DefaultLoadImage = gfxLoadImage;
        gfxLoadImage = LoadTextureVariant;
    }

    if (DefaultPrepareImage == nullptr) {
        DefaultPrepareImage = gfxPrepareImage;
        gfxPrepareImage = PrepareTextureVariant;
    }
}

void TextureVariantHandler::Install()
{
    InstallCallback(InitVariantData, {
        cb::call(0x412746),
        }, "Installs the texture variant init function."
    );

    InstallCallback(InstallTextureVariantHandler, {
            cb::call(0x401599),
        }, "Installs new texture variant handler."
    );

    GameEventDispatcher::RegisterStateEndCallback(Reset);

    // handled by cityTimeWeatherLightingHandler
    // leaving this here in case something goes wrong
    //--if (UseNightTexturesInEvening)
    //--{
    //--    LogFile::WriteLine("Installing evening patches...");
    //--
    //--    // aiTrafficLightInstance::DrawGlow
    //--    InstallPatch({ 1 }, {
    //--        0x53CABC + 3
    //--    });
    //--
    //--    /*
    //--        mmGame::InitWeather patches
    //--    */
    //--
    //--    // minimum time of day for night checks
    //--    InstallPatch({ 2 }, {
    //--        0x41338E + 2,
    //--        0x4133BD + 2,
    //--    });
    //--
    //--    // jnz -> jb
    //--    InstallPatch({ 0x72 }, {
    //--        0x41339D,
    //--    });
    //--
    //--    // jz -> jge
    //--    InstallPatch({ 0x7D }, {
    //--        0x4133CA,
    //--    });
    //--}
}

