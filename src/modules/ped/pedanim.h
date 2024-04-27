#pragma once
#include <modules\vector.h>

namespace MM2
{
    // Forward declarations
    struct pedAnimationSequence;
    class pedAnimation;
    class pedAnimationInstance;

    // External declarations
    extern class crAnimation;
    extern class crSkeleton;
    extern struct crSkeletonData;
    extern class modModel;
    extern class modShader;

    // Class definitions
    
    struct pedAnimationSequence {
        const char* Name;
        uint32_t FrameCount;
        float FSpeed;
        float LSpeed;
        int32_t Direction;
        crAnimation* pCrAnimation;
        float FrameBias; // Unused?
        const char* NextName;
        uint32_t NextIndex;
    };

    class pedAnimation {
    public:
        int AnimationCount;
        pedAnimationSequence* Animations;
        Matrix34 matrix348;
        modModel *pModel;
        modShader **ppShaders;
        crSkeleton *pSkeleton;
        crSkeletonData SkeletonData;
        float* RaysHeightMod;
        Vector2 *RaysBoneSizes;
        uint8_t *RaysOriginBone;
        uint8_t *RaysUnknown;
        uint8_t ** RaysShaderIndices;
        uint32_t *pColors;
        uint32_t SphereIndex;
        int nColorCount;
        uint16_t HashTableIndex;
    public:
        modModel* GetModel()                                    { return pModel; }
        modShader* GetShaders(int variant)                      { return ppShaders[variant]; }
        crSkeleton* GetSkeleton()                               { return pSkeleton; }
        pedAnimationSequence* GetSequence(int index)            { return &Animations[index]; }
        int GetSequenceCount() const                            { return AnimationCount; }

        void DrawSkeleton(int variant, crSkeleton* skeleton)    { hook::Thunk<0x57AB60>::Call<void>(this, variant, skeleton); }
    };

    class pedAnimationInstance {
    protected:
        void* m_Active; // pedActive*
        pedAnimation* m_Animation;
        int m_Variant;
        __int16 m_Frame;
        unsigned __int8 m_CurrentState;
        unsigned __int8 m_NextState;
    public:
        int GetCurrentState() const         { return m_CurrentState; }
        int GetNextState() const            { return m_NextState; }
        pedAnimation * GetAnimation() const { return m_Animation; }
        int GetVariant() const              { return m_Variant; }
        int GetCurrentFrame() const         { return m_Frame; }

        void SetVariant(int variant)        { m_Variant = variant; }
        void SetCurrentFrame(int frame)     { m_Frame = frame; }
        void Start(int nextStateIndex)      { hook::Thunk<0x57B550>::Call<void>(this, nextStateIndex); }

        // lua
        static void BindLua(LuaState L) {
            LuaBinding(L).beginClass< pedAnimationInstance>("pedAnimationInstance")
                .addPropertyReadOnly("State", &GetCurrentState)
                .addPropertyReadOnly("NextState", &GetNextState)
                .addProperty("Variant", &GetVariant, &SetVariant)
                .addFunction("SetVariant", &SetVariant) // lvlInstance parity
                .endClass();
        }
    };

    // Lua initialization

}