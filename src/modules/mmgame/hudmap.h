#pragma once
#include <modules\node.h>
#include "oppiconinfo.h"

namespace MM2
{
    // Forward declarations
    class mmHudMap;

    // External declarations
    extern class mmWaypoints;
    extern class gfxViewport;
    extern class asMeshSetForm;

    // Class definitions
    class mmHudMap : public asNode {
    private:
        mmWaypoints* Waypoints;
        BOOL ShowWaypoints;
        Vector3* GoldLocation;
        Vector3* BankLocation;
        Vector3* HideoutLocation;
        int dword_2c;
        mmPlayer* PlayerPtr;
        OppIconInfo* OpponentIcons;
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
        AGE_API void DrawIcon(int iconType, Matrix34 const & matrix)
                                                            { hook::Thunk<0x42F580>::Call<void>(this, iconType, &matrix); }
        AGE_API void DrawIndicator(int indicatorType, Vector3 const & position)
                                                            { hook::Thunk<0x42F6F0>::Call<void>(this, indicatorType, &position); }
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

        /*
            mmHudMap
        */
        bool GetShowAllCops() const
        {
            return this->ShowAllCops == TRUE;
        }

        void SetShowAllCops(bool show)
        {
            this->ShowAllCops = show ? TRUE : FALSE;
        }

        float GetIconScale() const
        {
            return this->IconScale;
        }

        void SetIconScale(float scale) 
        {
            this->IconScale = scale;
        }

        int GetOpponentCount() const 
        {
            return NumOpponents; 
        }

        OppIconInfo* GetOpponentIcon(int index) {
            if (index < 0 || index >= GetOpponentCount())
                return nullptr;

            if (OpponentIcons == nullptr)
                return nullptr;

            return &OpponentIcons[index];
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmHudMap, asNode>("mmHudMap")
                .addVariable("Position", &mmHudMap::Pos)
                .addVariable("Size", &mmHudMap::Size)
                .addPropertyReadOnly("NumOpponents", &GetOpponentCount)
                .addFunction("GetOpponentIcon", &GetOpponentIcon)
                .addProperty("ShowAllCops", &GetShowAllCops, &SetShowAllCops)
                .addProperty("IconScale", &GetIconScale, &SetIconScale)
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
                .addFunction("DrawIcon", &DrawIcon)
                .addFunction("DrawIndicator", &DrawIndicator)
            .endClass();  
        }
    };
    ASSERT_SIZEOF(mmHudMap, 0xC8);
}