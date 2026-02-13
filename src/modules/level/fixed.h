#pragma once
#include <modules\level\inst.h>

namespace MM2
{
    // Forward declarations
    class lvlFixedAny;
    class lvlFixedRotY;
    class lvlFixedMatrix;

    // External declarations
    extern class gfxViewport;

    // Class definitions
    class lvlFixedAny : public lvlInstance
    {
    public:
        AGE_API lvlFixedAny();

        /*
            lvlInstance virtuals
        */

        virtual AGE_API void SetVariant(int a1) override;
                                                           
        virtual AGE_API void Draw(int lod) override;
        virtual AGE_API void DrawShadow() override;
        virtual AGE_API void DrawShadowMap() override;
        virtual AGE_API void DrawReflectedParts(int a1) override;
        virtual AGE_API int Init(const char* a1, const Matrix34& a2, int a3) override;


        static void BindLua(LuaState L);
    };
    ASSERT_SIZEOF(lvlFixedAny, 0x14);

    class lvlFixedRotY : public lvlFixedAny
    {
    private:
        Vector3 Position;
        Vector2 XAxis;
    public:
        AGE_API lvlFixedRotY();

        /*
            lvlInstance virtuals
        */

        virtual AGE_API const Vector3& GetPosition() override;
        virtual AGE_API int IsVisible(const gfxViewport& a1) override;
        virtual AGE_API const Matrix34& GetMatrix(Matrix34* a1) override;
        virtual AGE_API void SetMatrix(const Matrix34& a1)  override;
        virtual AGE_API unsigned int SizeOf() override;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(lvlFixedRotY, 0x28);

    class lvlFixedMatrix : public lvlFixedAny
    {
    private:
        Matrix34 Matrix;
    public:
        AGE_API lvlFixedMatrix();

        /*
            lvlInstance virtuals
        */

        virtual AGE_API const Vector3& GetPosition() override;
        virtual AGE_API int IsVisible(gfxViewport const& viewport) override;
        virtual AGE_API const Matrix34& GetMatrix(Matrix34* a1) override;
        virtual AGE_API void SetMatrix(const Matrix34& a1) override;
        virtual AGE_API unsigned int SizeOf() override;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(lvlFixedMatrix, 0x44);
}