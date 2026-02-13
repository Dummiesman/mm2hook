#include "trailer.h"

namespace MM2
{
    /*
        vehTrailer
    */
    vehCarSim* vehTrailer::GetCarSim() const
    {
        return _sim.get(this);
    }

    dgTrailerJoint* vehTrailer::GetJoint() const
    {
        return _joint.ptr(this);
    }

    vehWheel * vehTrailer::GetWheel(int num) const
    {
        switch (num) {
        case 0:
            return _twhl0.ptr(this);
        case 1:
            return _twhl1.ptr(this);
        case 2:
            return _twhl2.ptr(this);
        case 3:
            return _twhl3.ptr(this);
        }
        return nullptr;
    }

    vehTrailerInstance* vehTrailer::GetInstance() const
    {
        return _instance.ptr(this);
    }

    Vector3 vehTrailer::GetCarHitchOffset() const
    {
        return _carHitchOffset.get(this);
    }

    Vector3 vehTrailer::GetTrailerHitchOffset() const
    {
        return _trailerHitchOffset.get(this);
    }

    AGE_API void vehTrailer::Init(const char* basename, const Vector3* a2, vehCarSim* a3, int a4)
    {
        hook::Thunk<0x4D72F0>::Call<void>(this, basename, a2, a3, a4);

        Matrix34 diffMatrix;

        if (GetPivot(diffMatrix, basename, "trailer_twhl4")) {
            GetCarSim()->TrailerBackBackLeftWheelPosDiff = diffMatrix.GetRow(3) - GetWheel(2)->GetCenter();
        }

        if (GetPivot(diffMatrix, basename, "trailer_twhl5")) {
            GetCarSim()->TrailerBackBackLeftWheelPosDiff = diffMatrix.GetRow(3) - GetWheel(3)->GetCenter();
        }
    }

    void vehTrailer::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<vehTrailer, dgPhysEntity>("vehTrailer")
            .addFunction("GetWheel", &GetWheel)
            .addPropertyReadOnly("CarSim", &GetCarSim)
            .addPropertyReadOnly("Joint", &GetJoint)
            .addPropertyReadOnly("Instance", &GetInstance)
            .addPropertyReadOnly("CarHitchOffset", &GetCarHitchOffset)
            .addPropertyReadOnly("TrailerHitchOffset", &GetTrailerHitchOffset)
        .endClass();
    }

    /*
        vehTrailerInstance
    */
    Vector3 vehTrailerInstance::getTrailerHitchOffsetLua() const
    {
        Vector3 vec;
        this->GetTrailerHitch(&vec);
        return vec;
    }

    AGE_API const Vector3& vehTrailerInstance::GetPosition()
    {
        return hook::Thunk<0x4D7810>::Call<const Vector3&>(this);
    }

    AGE_API const Matrix34& vehTrailerInstance::GetMatrix(Matrix34* a1)
    {
        return hook::Thunk<0x4D77F0>::Call<const Matrix34&>(this);
    }

    AGE_API void vehTrailerInstance::SetMatrix(const Matrix34& a1)
    {
        hook::Thunk<0x4D77D0>::Call<void>(this, &a1);
    }

    AGE_API void vehTrailerInstance::Draw(int a1)
    {
        hook::Thunk<0x4D7F20>::Call<void>(this, a1);
    }

    AGE_API unsigned int vehTrailerInstance::SizeOf(void)
    {
        return hook::Thunk<0x4D7850>::Call<unsigned int>(this);
    }

    vehTrailer * vehTrailerInstance::GetTrailer(void) const {
        return _trailer.get(this);
    }

    AGE_API bool vehTrailerInstance::GetTrailerHitch(Vector3* out) const { return hook::Thunk<0x4D8420>::Call<bool>(this, out); }

    void vehTrailerInstance::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<vehTrailerInstance, lvlInstance>("vehTrailerInstance")
            //properties
            .addPropertyReadOnly("Trailer", &GetTrailer)
            .addPropertyReadOnly("TrailerHitchOffset", &getTrailerHitchOffsetLua)
        .endClass();
    }
}