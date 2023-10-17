#pragma once
#include <modules\node\node.h>
#include <modules\parse\parse.h>
#include <modules\gfx\texture.h>
#include <modules\gfx\gettex.h>
#include <modules\gfx\rstate.h>
#include <modules\rgl.h>

namespace MM2
{
    // Forward declarations
    class fxWaterSpout;

    // External declarations


    // Class definitions
    class fxWaterSpout : public asNode {
    private:
        static const int MAX_POINTS = 12;
        static gfxTexture* sm_Texture;
    private:
        int MinPoints;
        int MaxPoints;
        int MinAngularRes;
        int MaxAngularRes;
        float Gravity;
        float VerticalSpeed;
        float HorizontalSpeed;
        float Life;
        float m_Time;
        float StartIntensity;
        float EndIntensity;
        float m_Rotation;
        float RotationSpeed;
        float LODNear;
        float LODFar;
        Vector2 TextureScale;
        Vector3 Color;
        Vector3 StartOffset;
        int dword_7c;
        Matrix34 m_Matrix;
        float IntensityScale;
        float FrontScale;
        float BackScale;
        gfxTexture* m_Texture;
        fxWaterSpout* m_Next;
        int dword_c4;
        int dword_c8;
        int dword_cc;
    public:
        fxWaterSpout()
        {
            Gravity = -10.0;
            VerticalSpeed = 6.0;
            HorizontalSpeed = 1.7;
            MinPoints = 3;
            MaxPoints = 5;
            StartOffset = Vector3(0.0f, 1.0f, 0.0f);
            TextureScale = Vector2(7.0f, 1.0f);
            Color = Vector3(1.0f, 1.0f, 1.0f);
            Life = 2.0;
            MinAngularRes = 3;
            MaxAngularRes = 5;
            StartIntensity = 0.3;
            EndIntensity = 0.05;
            m_Time = 0.0;
            m_Rotation = 0.0;
            RotationSpeed = 0.0;
            LODNear = 50.0;
            LODFar = 100.0;
            IntensityScale = 1.0;
            FrontScale = 1.0;
            BackScale = 1.0;
            m_Next = nullptr;
            auto texture = gfxGetTexture("fx_rain");
            m_Texture = texture;
            fxWaterSpout::sm_Texture = texture;
        }

        void DrawDouble() 
        {
            float rotation = m_Rotation;
            m_Rotation = 0.0f;
            Draw();
            m_Rotation = rotation;
            Draw();
        }

        void Draw() 
        {
            Vector3 startPoints[MAX_POINTS];
            Vector3 transformedPoints[MAX_POINTS];
            Vector3 lastTransformedPoints[MAX_POINTS];
            float vCoordinates[MAX_POINTS];
            unsigned int colors[MAX_POINTS];

            // compute lod
            auto camera = gfxRenderState::GetCameraMatrix();
            Vector3 cameraPos = static_cast<Vector3>(camera.GetRow(3));
            float distToCamera = cameraPos.FlatDist(m_Matrix.GetRow(3));
            
            float lod = (distToCamera - LODNear) / (LODFar - LODNear);
            lod = fminf(fmaxf(lod, 0.0f), 1.0f);

            int numPoints = static_cast<int>(MaxPoints + ((MinPoints - MaxPoints) * lod));
            int angularRes = static_cast<int>(MaxAngularRes + ((MinAngularRes - MaxAngularRes) * lod));

            if (numPoints > MAX_POINTS)
            {
                numPoints = MAX_POINTS;
            }

            float lod150 = distToCamera / 150.0f;
            lod150 = fminf(fmaxf(lod150, 0.0f), 1.0f);
            float invLod150 = 1.0f - lod150;

            float invAngularRes = 1.0f / angularRes;
            float invPointCount = 1.0f / (numPoints - 1);

            // calculate start points
            float lifePerPoint = Life / (numPoints - 1);
            for (int i = 0; i < numPoints; i++)
            {
                float halfGrav = Gravity * 0.5f;
                float pointLife = lifePerPoint * i;

                float x = StartOffset.X;
                float y = StartOffset.Y + ((VerticalSpeed * pointLife) + ((halfGrav * pointLife) * pointLife));
                float z = StartOffset.Z + (pointLife * HorizontalSpeed);
                startPoints[i] = Vector3(x, y, z);
            }

            // calculate v coordinates and colors
            for (int i = 0; i < numPoints; i++)
            {
                float vOffset = m_Time / Life;
                float v = ((i * invPointCount) - vOffset) * TextureScale.Y;
                vCoordinates[i] = v;

                float colorScale = (StartIntensity + ((EndIntensity - StartIntensity) * (i * invPointCount))) * IntensityScale * invLod150;
                colors[i] = mkfrgba(Color.X * colorScale, Color.Y * colorScale, Color.Z * colorScale, 1.0f);
            }

            // setup render
            bool oldAlpha = gfxRenderState::SetAlphaEnabled(true);
            D3DCULL oldCull = gfxRenderState::SetCullMode(D3DCULL_NONE);
            gfxRenderState::SetWorldMatrix(m_Matrix);
            gfxRenderState::SetBlendSet(7, 0x80);
            vglBindTexture(m_Texture);

            Matrix34 mtx = Matrix34();
            mtx.Identity();

            // transform the first row of points
            {
                // setup matrix for this row of points
                mtx.Identity();

                mtx.m22 = FrontScale;
                mtx.RotateY(m_Rotation);

                // transform this row of points
                for (int j = 0; j < numPoints; j++)
                {
                    lastTransformedPoints[j] = mtx.Transform(startPoints[j]);
                }
            }

            // draw the fountain
            for (int i = 1; i < angularRes+1; i++)
            {
                int pointPrev = ((i-1) % angularRes);
                int pointCurrent = (i % angularRes);

                // setup matrix for this row of points
                mtx.Identity();
                
                float s = pointCurrent * invAngularRes;
                float scaleBlend = 0.0f;
                if (s > 0.5f)
                    scaleBlend = (1.0f - s) + (1.0f - s);
                else
                    scaleBlend = s + s;
                float scale = FrontScale + ((BackScale - FrontScale) * scaleBlend);
                
                mtx.m22 = scale;
                mtx.RotateY((6.14f * pointCurrent * invAngularRes) + m_Rotation);

                // transform this row of points
                for (int j = 0; j < numPoints; j++)
                {
                    transformedPoints[j] = mtx.Transform(startPoints[j]);
                }

                // draw tri row
                vglBegin(gfxDrawMode::DRAWMODE_TRIANGLELIST, 6 * (numPoints - 1));                
                for (int j = 0; j < numPoints - 1; j++)
                {
                    Vector3 p0 = lastTransformedPoints[j];
                    Vector3 p1 = lastTransformedPoints[j+1];
                    Vector3 p2 = transformedPoints[j];
                    Vector3 p3 = transformedPoints[j + 1];

                    float u0 = pointPrev * invAngularRes * TextureScale.X;
                    float u1 = pointCurrent * invAngularRes * TextureScale.X;
                    
                    float v0 = vCoordinates[j];
                    float v1 = vCoordinates[j + 1];

                    unsigned int c0 = colors[j];
                    unsigned int c1 = colors[j + 1];

                    vglCurrentColor = c0;
                    vglTexCoord2f(u0, v0);
                    vglVertex3f(p0);
                    vglCurrentColor = c0;
                    vglTexCoord2f(u1, v0);
                    vglVertex3f(p2);
                    vglCurrentColor = c1;
                    vglTexCoord2f(u1, v1);
                    vglVertex3f(p3);

                    vglCurrentColor = c0;
                    vglTexCoord2f(u0, v0);
                    vglVertex3f(p0);
                    vglCurrentColor = c1;
                    vglTexCoord2f(u1, v1);
                    vglVertex3f(p3);
                    vglCurrentColor = c1;
                    vglTexCoord2f(u0, v1);
                    vglVertex3f(p1);
                }
                vglEnd();

                // transfer points into other array
                for (int j = 0; j < numPoints; j++)
                {
                    lastTransformedPoints[j] = transformedPoints[j];
                }
            }

            vglBindTexture(nullptr);
            gfxRenderState::SetBlendSet(0, 0x80);
            gfxRenderState::SetAlphaEnabled(oldAlpha);
            gfxRenderState::SetCullMode(oldCull);
        }

        void FileIO(datParser& parser) override
        {
            parser.AddValue("FrontScale", &this->FrontScale);
            parser.AddValue("BackScale", &this->BackScale);
            parser.AddValue("MinPoints", &this->MinPoints);
            parser.AddValue("MaxPoints", &this->MaxPoints);
            parser.AddValue("MinAngularRes", &this->MinAngularRes);
            parser.AddValue("MaxAngularRes", &this->MaxAngularRes);
            parser.AddValue("StartIntensity", &this->StartIntensity);
            parser.AddValue("EndIntensity", &this->EndIntensity);
            parser.AddValue("Gravity", &this->Gravity);
            parser.AddValue("VerticalSpeed", &this->VerticalSpeed);
            parser.AddValue("HorizontalSpeed", &this->HorizontalSpeed);
            parser.AddValue("RotationSpeed", &this->RotationSpeed);
            parser.AddValue("Life", &this->Life);
            parser.AddValue("TextureScale", &this->TextureScale);
            parser.AddValue("StartOffset", &this->StartOffset);
            parser.AddValue("Color", &this->Color);
            parser.AddValue("LODNear", &this->LODNear);
            parser.AddValue("LODFar", &this->LODFar);
        }

        char* GetClassName() override 
        {
            return "fxWaterSpout";
        }

        void SetMatrix(Matrix34 const& mtx)
        {
            m_Matrix.Set(mtx);
        }

        void SetTexture(gfxTexture* texture)
        {
            m_Texture = texture;
        }

        void SetIntensityScale(float scale)
        {
            IntensityScale = scale;
        }

        void SetNext(fxWaterSpout* spout)
        {
            m_Next = spout;
        }

        fxWaterSpout* GetNext()
        {
            return m_Next;
        }

        void Update(float time)
        {
            m_Time += time;
            m_Time = WRAPNUM(m_Time, Life);

            m_Rotation += RotationSpeed * time;
            m_Rotation = WRAPNUM(m_Rotation, 6.28f);
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<fxWaterSpout, asNode>("fxWaterSpout")
                .addFactory([]() { return new fxWaterSpout(); })
                .addVariable("FrontScale", &fxWaterSpout::FrontScale)
                .addVariable("BackScale", &fxWaterSpout::BackScale)
                .addVariable("MinPoints", &fxWaterSpout::MinPoints)
                .addVariable("MaxPoints", &fxWaterSpout::MaxPoints)
                .addVariable("MinAngularRes", &fxWaterSpout::MinAngularRes)
                .addVariable("MaxAngularRes", &fxWaterSpout::MaxAngularRes)
                .addVariable("StartIntensity", &fxWaterSpout::StartIntensity)
                .addVariable("EndIntensity", &fxWaterSpout::EndIntensity)
                .addVariable("Gravity", &fxWaterSpout::Gravity)
                .addVariable("VerticalSpeed", &fxWaterSpout::VerticalSpeed)
                .addVariable("HorizontalSpeed", &fxWaterSpout::HorizontalSpeed)
                .addVariable("RotationSpeed", &fxWaterSpout::RotationSpeed)
                .addVariable("Life", &fxWaterSpout::Life)
                .addVariable("TextureScale", &fxWaterSpout::TextureScale)
                .addVariable("StartOffset", &fxWaterSpout::StartOffset)
                .addVariable("Color", &fxWaterSpout::Color)
                .addVariable("LODNear", &fxWaterSpout::LODNear)
                .addVariable("LODFar", &fxWaterSpout::LODFar)
                .addVariable("IntensityScale", &fxWaterSpout::IntensityScale)
                .addFunction("Draw", &Draw)
                .addFunction("DrawDouble", &DrawDouble)
                .addFunction("Update", &Update)
                .addFunction("SetMatrix", &SetMatrix)
                .addFunction("SetTexture", &SetTexture)
                .endClass();
        }
    };
}