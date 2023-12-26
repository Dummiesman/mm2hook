#pragma once
#include <modules\creature.h>

namespace MM2
{
    // Forward declarations
    struct crBone;
    class crSkeleton;
    struct crSkeletonData;
    struct crBoneData;
    struct crSkeletonData;

    // External declarations


    // Class definitions
    struct crBone
    {
        Vector3 EulerAngles;
        Matrix34 Matrix;
        Matrix34* dword_3c;
    };


    class crSkeleton 
    {
    private:
        int BoneCount;
        crBone* Bones;
        int dword_08;
        Matrix34* SomeMatrix;
    public:
        AGE_API void Init(crSkeletonData *a1, Matrix34 *a2) { hook::Thunk<0x57C5D0>::Call<void>(this, a1, a2); }
        AGE_API void Update()                               { hook::Thunk<0x57C630>::Call<void>(this); }
        AGE_API void Attach(Matrix34 *a1)                   { hook::Thunk<0x57C650>::Call<void>(this, a1); }
        AGE_API void Attach(Matrix44 *a1)                   { hook::Thunk<0x57C690>::Call<void>(this, a1); }
    };

    struct crBoneData
    {
        char Name[64];
        crBoneData* Next;
        crBoneData* Child;
        crBoneData* Parent;
        Vector3 Offset;
        int dword_58;
        int ID;
        int MirrorBoneID;
        Vector3 RotMin;
        Vector3 RotMax;
    };


    struct crSkeletonData
    {
        int NumBones;
        crBoneData* Bones;
    };

    // Lua initialization

}