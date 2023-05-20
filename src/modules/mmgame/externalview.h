#pragma once
#include <modules\node.h>
#include <modules\gfx\pipeline.h>
#include "gauge.h"

namespace MM2
{
    // Forward declarations
    class mmExternalView;

    // External declarations
    extern class mmPlayer;
    extern class vehCarSim;

    // Class definitions
    class mmExternalView : public asNode {
    private:
        vehCarSim* CarSimPtr;
        mmPlayer* PlayerPtr;
        int XPos;
        int YPos;
        gfxBitmap* MouseBarBitmap;
        gfxBitmap* MouseBarArrowBitmap;
        int MouseBarXPos;
        int MouseBarYPos;
        int MouseBarArrowXPos;
        int MouseBarArrowYPos;
        int MouseBarCenter;
        int MouseArrowCenter;
        mmLinearGauge SpeedGauge;
        mmSlidingGauge DamageGauge;
        mmGearIndicator GearIndicator;
        mmSpeedIndicator SpeedIndicator;
    public:
        /*
            asNode virtuals
        */
        AGE_API void Update() override                      { hook::Thunk<0x4319E0 >::Call<void>(this); }
        AGE_API void Cull() override                        { hook::Thunk<0x4319F0>::Call<void>(this); }
        AGE_API void Reset() override                       { hook::Thunk<0x4319D0>::Call<void>(this); }
        AGE_API void ResChange(int width, int height) override
                                                            { hook::Thunk<0x431880>::Call<void>(this, width, height); }
        AGE_API char* GetClassName() override               { return hook::Thunk<0x431C20>::Call<char*>(this); }

        /*
            mmExternalView
        */
        void Init(mmPlayer* player)                         { hook::Thunk<0x431840>::Call<void>(this, player); }
        
        void DrawMouseBar()
        {
            this->MouseBarCenter = (MouseBarBitmap->Width / 2) - 15;
            this->MouseArrowCenter = MouseBarArrowBitmap->Width / 2;

            this->MouseBarXPos = (gfxPipeline::GetWidth() - MouseBarBitmap->Width) >> 1;
            this->MouseBarYPos = gfxPipeline::GetHeight() - 2 * MouseBarBitmap->Height;
            
            float mouseBarCenterF = (float)this->MouseBarCenter;
            this->MouseBarArrowXPos = (gfxPipeline::GetWidth() / 2) + (int)(mouseBarCenterF * PlayerPtr->GetSteering()) - this->MouseArrowCenter;

            this->MouseBarArrowYPos = this->MouseBarYPos - 16;
            gfxPipeline::CopyBitmap(
                this->MouseBarArrowXPos,
                this->MouseBarArrowYPos,
                MouseBarArrowBitmap,
                0,
                0,
                MouseBarArrowBitmap->Width,
                MouseBarArrowBitmap->Height,
                1);
            gfxPipeline::CopyBitmap(
                this->MouseBarXPos,
                this->MouseBarYPos,
                this->MouseBarBitmap,
                0,
                0,
                this->MouseBarBitmap->Width,
                this->MouseBarBitmap->Height,
                1);
        }

        mmLinearGauge* GetSpeedGauge()
        {
            return &this->SpeedGauge;
        }

        mmSlidingGauge* GetDamageGauge()
        {
            return &this->DamageGauge;
        }

        mmGearIndicator* GetGearIndicator()
        {
            return &this->GearIndicator;
        }

        mmSpeedIndicator* GetSpeedIndicator()
        {
            return &this->SpeedIndicator;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmExternalView, asNode>("mmExternalView")
                .addPropertyReadOnly("SpeedGauge", &GetSpeedGauge)
                .addPropertyReadOnly("DamageGauge", &GetDamageGauge)
                .addPropertyReadOnly("GearIndicator", &GetGearIndicator)
                .addPropertyReadOnly("SpeedIndicator", &GetSpeedIndicator)
                .addFunction("DrawMouseBar", &DrawMouseBar)
                .endClass();
        }                                                      
    };
    ASSERT_SIZEOF(mmExternalView, 0x104);

}