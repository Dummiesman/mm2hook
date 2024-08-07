#pragma once
#include <modules\vector.h>
#include <modules\creature\crskeleton.h>

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
    extern class pedActive;

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
        pedAnimation() : SkeletonData()
        {

        }

        modModel* GetModel()                                    { return pModel; }
        modShader* GetShaders(int variant)                      { return ppShaders[variant]; }
        crSkeleton* GetSkeleton()                               { return pSkeleton; }
        pedAnimationSequence* GetSequence(int index)            { return &Animations[index]; }
        int GetSequenceCount() const                            { return AnimationCount; }
        int GetHashTableIndex() const                           { return HashTableIndex; }

        int LookupSequence(const char* name) const              { return hook::Thunk<0x57A7A0>::Call<int>(this, name); }
        void DrawSkeleton(int variant, crSkeleton* skeleton)    { hook::Thunk<0x57AB60>::Call<void>(this, variant, skeleton); }
    };

    class pedAnimationInstance {
    protected:
        pedActive* m_Active;
        pedAnimation* m_Animation;
        int m_Variant;
        __int16 m_Frame;
        unsigned __int8 m_CurrentState;
        unsigned __int8 m_NextState;
    public:
        pedAnimationInstance()
        {
            m_Active = nullptr;
            m_Animation = nullptr;
            m_Variant = 0;
            m_CurrentState = 0;
            m_NextState = 0;
            m_Frame = 0;
        }

        pedActive* GetActive() const         { return m_Active; }
        int GetCurrentState() const          { return m_CurrentState; }
        int GetNextState() const             { return m_NextState; }
        pedAnimation * GetAnimation() const  { return m_Animation; }
        int GetVariant() const               { return m_Variant; }
        int GetCurrentFrame() const          { return m_Frame; }
                                             
        void SetActive(pedActive* active)    { m_Active = active; }
        void SetVariant(int variant)         { m_Variant = variant; }
        void SetCurrentFrame(int frame)      { m_Frame = frame; }
                                             
        void Init(const char* pedName)       { hook::Thunk<0x57ADB0>::Call<void>(this, pedName); }
        void Draw(bool highLod)              { hook::Thunk<0x57B370>::Call<void>(this, highLod); }
        void Start(int nextStateIndex)       { hook::Thunk<0x57B550>::Call<void>(this, nextStateIndex); }
        void Update()                        { hook::Thunk<0x57B2F0>::Call<void>(this); }
        void VerifySequence(int index) const { return hook::Thunk<0x57B520>::Call<void>(this, index); }

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