#pragma once
#include <modules\node.h>

namespace MM2
{
    // Forward declarations
    class mmHudMap;
    struct OppIconInfo;

    // External declarations
    extern class mmWaypoints;
    extern class gfxViewport;
    extern class asMeshSetForm;

    // Class definitions
    struct OppIconInfo
    {
        ColorARGB Color;
        int dword_04;
        Matrix34* MatrixPtr;
        int dword_0c;
        int dword_10;
        int dword_14;
        int dword_18;
        int dword_1c;
        int Bitmap;
        int dword_24;
    };

    class mmHudMap : public asNode {
    private:
        mmWaypoints* Waypoints;
        BOOL ShowWaypoints;
        Vector3* GoldLocation;
        Vector3* BankLocation;
        Vector3* HideoutLocation;
        int dword_2c;
        mmPlayer* PlayerPtr;
        OppIconInfo* OppIconInfo;
        BOOL ShowAllCops;
        bool MapOnLeft;
        bool MapMissing;
        int LastNonFSMapMode;
        int MapMode;
        Matrix34* PlayerMatrixPtr;
        float ApproachRate;
        float ZoomLevel;
        float ZoomInDist;
        float ZoomOutDist;
        float ZoonInDistFS;
        float ZoomOutDistFS;
        float IconScale;
        float IconScaleMin;
        float IconScaleMax;
        float IconScaleMinFS;
        float IconScaleMaxFS;
        Vector3 BackgroundColor;
        Vector2 Size;
        Vector2 Pos;
        gfxViewport* Viewport;
        asMeshSetForm* HudmapModel;
        byte gap[32];
        int NumOpponents;
        asMeshSetForm* HudmapSquareModel;
        asMeshSetForm* HudmapTriModel;
    private:
        bool getShowAllCopsLua()
        {
            return this->ShowAllCops == TRUE;
        }

        void setShowAllCopsLua(bool b)
        {
            this->ShowAllCops = b ? TRUE : FALSE;
        }
    protected:
        AGE_API int GetCurrentMapMode()                     { return hook::Thunk<0x42EF20>::Call<int>(this); }
        AGE_API int GetNextMapMode()                        { return hook::Thunk<0x42EF00>::Call<int>(this); }
        AGE_API void SetMapMode(int a1)                     { hook::Thunk<0x42EF30>::Call<void>(this, a1); }
    public:
        AGE_API void Activate()                             { hook::Thunk<0x42EEE0>::Call<void>(this); }
        AGE_API void Deactivate()                           { hook::Thunk<0x42EEF0>::Call<void>(this); }
        AGE_API void SetOrient(bool a1)                     { hook::Thunk<0x42FA40>::Call<void>(this, a1); }
        AGE_API void SetZoomIn(bool a1)                     { hook::Thunk<0x42FA20>::Call<void>(this, a1); }
        AGE_API void ToggleMapOrient()                      { hook::Thunk<0x42FA10>::Call<void>(this); }
        AGE_API void ToggleMapRes()                         { hook::Thunk<0x42FA00>::Call<void>(this); }
        AGE_API bool GetOrient()                            { return hook::Thunk<0x42FA50>::Call<bool>(this); }
        AGE_API bool GetZoomIn()                            { return hook::Thunk<0x42FA30>::Call<bool>(this); }

        /*
            asNode virtuals
        */

        AGE_API void Cull() override                { hook::Thunk<0x42F1B0>::Call<void>(this); }
        AGE_API void Update() override              { hook::Thunk<0x42F1A0>::Call<void>(this); }
        AGE_API void Reset() override               { hook::Thunk<0x42EE90>::Call<void>(this); }
        AGE_API void FileIO(datParser &parser) override 
                                                    { hook::Thunk<0x42FA60>::Call<void>(this, &parser); }
        AGE_API char * GetClassName() override      { return hook::Thunk<0x42FD60>::Call<char*>(this); }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmHudMap, asNode>("mmHudMap")
                .addProperty("ShowAllCops", &getShowAllCopsLua, &setShowAllCopsLua)
                .addFunction("Activate", &Activate)
                .addFunction("Deactivate", &Deactivate)
                .addFunction("SetOrient", &SetOrient)
                .addFunction("SetZoomIn", &SetZoomIn)
                .addFunction("ToggleMapOrient", &ToggleMapOrient)
                .addFunction("ToggleMapRes", &ToggleMapRes)
                .addFunction("GetOrient", &GetOrient)
                .addFunction("GetZoomIn", &GetZoomIn)
                .addFunction("GetCurrentMapMode", &GetCurrentMapMode)
                .addFunction("GetNextMapMode", &GetNextMapMode)
                .addFunction("SetMapMode", &SetMapMode)
            .endClass();  
        }
    };
    ASSERT_SIZEOF(mmHudMap, 0xC8);
}