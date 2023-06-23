#pragma once
#include "icons.h"
#include <mm2_rgl.h>

using namespace MM2;

void mmIcons::registerOpponentsLua(LuaOppIconInfo& container, LuaRef fontRef)
{
    LocFont* fontPtr = nullptr;
    if (fontRef.type() != LuaTypeID::NIL)
    {
        fontPtr = fontRef.toValue<LocFont*>();
    }

    RegisterOpponents(container.GetFirst(), container.GetIconCount(), fontPtr);
}

int mmIcons::GetIconCount() const { return this->IconCount;  }
OppIconInfo* mmIcons::GetIcon(int index) {
    if (index < 0 || index >= GetIconCount())
        return nullptr;

    auto icons = this->IconInfo;
    if (icons == nullptr)
        return nullptr;
            
    return &icons[index];
}

AGE_API void mmIcons::RegisterOpponents(OppIconInfo* infos, int numInfos, LocFont* font)
                                            { hook::Thunk<0x4322F0>::Call<void>(this, infos, numInfos, font); }

/*
    asNode virtuals
*/        
AGE_API void mmIcons::Update() 
{ 
    hook::Thunk<0x432390>::Call<void>(this); 
}

AGE_API void mmIcons::Cull() 
{
    // don't render if no icons are registered
    if (IconCount <= 0 || IconInfo == nullptr)
        return;

    bool alphaWasEnabled = gfxRenderState::SetAlphaEnabled(true);
    bool wasDepthTestEnabled = rglEnableDisable(rglToken::RGL_DEPTH_TEST, false);

    // draw the icons
    for (int i = 0; i < IconCount; i++)
    {
        auto icon = IconInfo[i];
        if (icon.Enabled && icon.MatrixPtr != nullptr && icon.Scale > 0.0f)
        {
            // setup render state
            gfxRenderState::SetCard(icon.MatrixPtr->GetRow(3));
            vglBindTexture(nullptr);

            if (DiamondPointers == TRUE)
                vglBegin(gfxDrawMode::DRAWMODE_TRIANGLESTRIP, 0);
            else
                vglBegin(gfxDrawMode::DRAWMODE_TRIANGLELIST, 0);

            vglCurrentColor = icon.Color;
                    
            // draw pointer
            Vector3 offset = Vector3(0.0f, 4.0f, 0.0f);

            vglVertex3f((TriangleVerts[0] * icon.Scale) + offset);
            vglVertex3f((TriangleVerts[1] * icon.Scale) + offset);
            vglVertex3f((TriangleVerts[2] * icon.Scale) + offset);

            if (DiamondPointers == TRUE)
            {
                vglVertex3f((TriangleVerts[3] * icon.Scale) + offset);
            }

            vglEnd();

            // draw icon
            if (icon.IconIndex > 0 && icon.IconIndex < 10)
            {
                vglBindTexture(IconTexture);
                vglCurrentColor = 0xFFFFFFFF;
                
                vglBegin(gfxDrawMode::DRAWMODE_TRIANGLESTRIP, 0);

                int textureTileX = (icon.IconIndex - 1) % 4;
                int textureTileY = (icon.IconIndex - 1) / 4;
                Vector2 uvOffset = Vector2(textureTileX * UVFraction, textureTileY * UVFraction);

                vglTexCoord2f((IconUVs[0] * UVFraction) + uvOffset);
                vglVertex3f((IconVerts[0] * icon.Scale) + offset);

                vglTexCoord2f((IconUVs[1] * UVFraction) + uvOffset);
                vglVertex3f((IconVerts[1] * icon.Scale) + offset);

                vglTexCoord2f((IconUVs[2] * UVFraction) + uvOffset);
                vglVertex3f((IconVerts[2] * icon.Scale) + offset);

                vglTexCoord2f((IconUVs[3] * UVFraction) + uvOffset);
                vglVertex3f((IconVerts[3] * icon.Scale) + offset);

                vglEnd();
            }
        }
    }
    
    if (IconMode == 3)
    {
        gfxRenderState::SetWorldMatrix(Matrix34::I);
        gfxRenderState::Regenerate();

        Vector4 tmp;

        for (int i = 0; i < IconCount; i++)
        {
            auto icon = IconInfo[i];
            if (icon.Enabled && icon.MatrixPtr != nullptr)
            {
                Vector3 camPosDiff = icon.MatrixPtr->GetRow(3) - CameraMatrixPtr->GetRow(3);
                float camDist2 = camPosDiff.Mag2();
                
                if (camDist2 < MaxDistance2)
                {
                    Vector4 textPosition = Vector4(icon.MatrixPtr->m30, icon.MatrixPtr->m31 + 2.0f, icon.MatrixPtr->m32, 1.0f);
                    tmp.Dot(textPosition, gfxRenderState::GetFullComposite());

                    if (tmp.W > 0.0f)
                    {
                        float invDist = 1.0f / tmp.W;
                        tmp.X *= invDist;
                        tmp.Y *= invDist;
                        tmp.Z *= invDist;

                        gfxPipeline::CopyBitmap(tmp.X - (icon.Bitmap->Width / 2), tmp.Y - icon.Bitmap->Height, icon.Bitmap, 0, 0, icon.Bitmap->Width, icon.Bitmap->Height, true);
                        //vglDrawLabelf(icon.MatrixPtr->GetRow(3), "W=%.4f, D=%.4f", tmp.W, dist);
                    }
                }
            }
        }
    }

    rglEnableDisable(rglToken::RGL_DEPTH_TEST, wasDepthTestEnabled);
    gfxRenderState::SetAlphaEnabled(alphaWasEnabled);
}

void mmIcons::BindLua(LuaState L) {
    LuaBinding(L).beginExtendClass<mmIcons, asNode>("mmIcons")
        .addPropertyReadOnly("NumIcons", &GetIconCount)
        .addFunction("RegisterOpponents", &registerOpponentsLua)
        .addFunction("GetIcon", &GetIcon)
        .endClass();
}
