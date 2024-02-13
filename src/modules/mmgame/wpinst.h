#pragma once
#include <modules\level\inst.h>
#include <modules\gfx\rstate.h>
#include <modules\model\static2.h>

namespace MM2
{
    // Forward declarations
    class mmWaypointInstance;
    class mmBillInstance;
    class mmCheckpointInstance;

    // Class definitions
    class mmWaypointInstance : public lvlInstance {
    protected:
        Vector3 Scale;
        Matrix34 Matrix;
        BOOL HasBeenDrawn;
        bool LocalVisibility;
        int ShaderSet;
    public:

        /*
            lvlInstance virtuals
        */

        virtual AGE_API const Vector3 & GetPosition() override              { return hook::Thunk<0x43F520>::Call<Vector3 const &>(this); }
        virtual AGE_API const Matrix34 & GetMatrix(Matrix34 &a1) override   { return hook::Thunk<0x43F530>::Call<Matrix34 const &>(this, &a1); }
        virtual AGE_API void SetMatrix(const Matrix34 & a1) override        { hook::Thunk<0x43F550>::Call<void>(this, &a1); }
        virtual AGE_API void SetVariant(int a1) override                    { hook::Thunk<0x43FC60>::Call<void>(this, a1); }

        /*
            mmWaypointInstance virtuals
        */
        virtual AGE_API void Init(const char* a1, Vector3 scale)             PURE;

        /// <summary>
        /// Test method
        /// </summary>
        virtual AGE_API int IsVisible(const gfxViewport* a1) override      
        { 
            if (!LocalVisibility)
                return 0;
            return lvlInstance::IsVisible(a1);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<mmWaypointInstance, lvlInstance>("mmWaypointInstance")

            .endClass();
        }
    };
    ASSERT_SIZEOF(mmWaypointInstance, 0x5C);

    class mmBillInstance : public mmWaypointInstance {
    public:
        virtual AGE_API void Init(const char* a1, Vector3 scale) override
        {
            hook::Thunk<0x43F820>::Call<void>(this, a1, scale);
        }

        virtual AGE_API unsigned int SizeOf() override 
        {
            return sizeof(mmBillInstance);
        }

        // rewritten to only billboard on Y axis
        virtual AGE_API void Draw(int lod) override
        {
            if (!LocalVisibility)
                return;

            bool prevLighting = gfxRenderState::SetLighting(false);

            this->HasBeenDrawn = TRUE;
            
            // setup matrix
            Vector4 cameraPosition4 = gfxRenderState::GetCameraMatrix().GetRow(3);
            Vector3 camPosition = Vector3(cameraPosition4.X, 0.0f, cameraPosition4.Z);
            Vector3 ourPosition = this->GetPosition();
            ourPosition.Y = 0.0f;

            Matrix34 renderMatrix = Matrix34();
            renderMatrix.LookAt(ourPosition, camPosition);
            renderMatrix.SetRow(3, this->GetPosition());
            renderMatrix.Scale(this->Scale.X, this->Scale.Y, this->Scale.Z);
            gfxRenderState::SetWorldMatrix(renderMatrix);

            // draw
            auto geom = this->GetGeomBase();
            if (geom != nullptr)
            {
                auto mod = geom->GetHighestLOD();
                if (mod != nullptr) 
                {
                    auto shaders = this->GetShader(this->ShaderSet);
                    mod->Draw(shaders);
                }
            }

            gfxRenderState::SetLighting(prevLighting);
        }
    };
    ASSERT_SIZEOF(mmBillInstance, 0x5C);

    class mmCheckpointInstance : public mmWaypointInstance
    {
    public:
        virtual AGE_API void Init(const char* a1, Vector3 scale) override
        {
            hook::Thunk<0x43F600>::Call<void>(this, a1, scale);
        }

        virtual AGE_API unsigned int SizeOf() override
        {
            return sizeof(mmCheckpointInstance);
        }

        virtual AGE_API void Draw(int lod) override
        {
            hook::Thunk<0x43F650>::Call<void>(this, lod);
        }
    };
    ASSERT_SIZEOF(mmCheckpointInstance, 0x5C);
}