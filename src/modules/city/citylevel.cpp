#pragma once
#include "citylevel.h"

namespace MM2
{
    declfield(cityLevel::Sky)(0x628728);
    declfield(cityLevel::sm_EnablePVS)(0x62B070);
    declfield(cityLevel::sm_PvsBuffer)(0x62AE68);
    declfield(cityLevel::SDL)(0x629928);

    /*
        cityTimeWeatherLighting
    */
    void cityTimeWeatherLighting::ComputeAmbientLightLevels()
    {
        hook::Thunk<0x443300>::Call<void>(this);
    }

    /*
        cityLevel
    */

    AGE_API cityLevel::cityLevel() {
        scoped_vtable x(this);
        hook::Thunk<0x443860>::Call<void>(this);
    };

    AGE_API cityLevel::~cityLevel() {
        scoped_vtable x(this);
        hook::Thunk<0x443910>::Call<void>(this);
    };

    /*
        lvlLevel virtuals
    */

    AGE_API void cityLevel::Load(char const* a1)          { hook::Thunk<0x443F30>::Call<void>(this, a1); }
    AGE_API void cityLevel::Update()                      { hook::Thunk<0x4452B0>::Call<void>(this); }
    AGE_API void cityLevel::PreDraw()                     { hook::Thunk<0x4452E0>::Call<void>(this); }
    AGE_API void cityLevel::PostDraw()                    { hook::Thunk<0x445300>::Call<void>(this); }
    AGE_API void cityLevel::Draw(const gfxViewport& a1, uint a2) 
                                                                { hook::Thunk<0x445400>::Call<void>(this, &a1, a2); }

    AGE_API int cityLevel::FindRoomId(Vector3 const& a1, int a2) const
                                                        { return hook::Thunk<0x446A60>::Call<int>(this, &a1, a2); }
    AGE_API int cityLevel::GetNeighborCount(int room) const 
                                                        { return hook::Thunk<0x446C20>::Call<int>(this, room); }
    AGE_API int cityLevel::GetNeighbors(int* neighbourRooms, int room) const  
                                                        { return hook::Thunk<0x446C40>::Call<int>(this, neighbourRooms, room); }
    AGE_API int cityLevel::GetTouchedNeighbors(int* a1, int a2, int a3, const Vector4& a4)
                                                        { return hook::Thunk<0x446CD0>::Call<int>(this, a1, a2, a3, a4); }
    AGE_API int cityLevel::GetRoomPerimeter(int roomId, Vector3* out, int outSize) const
                                                        { return hook::Thunk<0x446FA0>::Call<int>(this, roomId, out, outSize); }
    AGE_API int cityLevel::GetVisitList(int* a1, int a2, Vector3 const& a3, Vector3 const& a4, int a5, int a6)
                                                        { return hook::Thunk<0x447030>::Call<int>(this, a1, a2, a3, a4, a5, a6); }
    AGE_API bool cityLevel::Collide(int a1, lvlSegment& a2, lvlIntersection& a3)
                                                        { return false; }
    AGE_API bool cityLevel::GetBoundSphere(Vector4& a1, int a2)
                                                        { return hook::Thunk<0x445310>::Call<bool>(this, a1, a2); }
    AGE_API const class lvlLevelBound* cityLevel::GetBound()
                                                        { return hook::Thunk<0x443930>::Call<class lvlLevelBound*>(this); }
    AGE_API void cityLevel::SetObjectDetail(int a1)       { hook::Thunk<0x443E50>::Call<void>(this, a1); }
    AGE_API float cityLevel::GetWaterLevel(int a1) const  { return hook::Thunk<0x445280>::Call<float>(this, a1); }
    AGE_API float cityLevel::GetLightingIntensity(Vector3 const& a1) const
                                                        { return hook::Thunk<0x445290>::Call<float>(this, a1); }
    AGE_API void cityLevel::SetPtxHeight(asParticles& a1) { hook::Thunk<0x4452A0>::Call<void>(this, &a1); }

    AGE_API gfxTexture* cityLevel::GetEnvMap(int room, Vector3 const& position, float& intensity)
                                                                { return hook::Thunk<0x443940>::Call<gfxTexture*>(this, room, &position, &intensity); }


    /*
        cityLevel functions
    */
    AGE_API void cityLevel::EnablePVS(bool enable) {
        hook::Thunk<0x4472A0>::Call<void>(this, enable);
    }

    AGE_API void cityLevel::EnableSky(bool enable) {
        hook::Thunk<0x447290>::Call<void>(this, enable);
    }

    AGE_API void cityLevel::LoadPath(const char* a1, const Matrix34& a2, bool a3) {
        hook::StaticThunk<0x4451E0>::Call<void>(a1, a2, a3);
    }

    AGE_API void cityLevel::LoadPathSet(const char* a1, const char* a2) {
        hook::StaticThunk<0x445140>::Call<void>(a1, a2);
    }

    AGE_API void cityLevel::LoadProp(int a1, const char* a2, const Matrix34& a3) {
        hook::StaticThunk<0x4450E0>::Call<void>(a1, a2, a3);
    }

    //should be protected but we use this in the hook
    AGE_API void cityLevel::DrawRooms(const gfxViewport* a1, uint a2, LPVOID a3, int a4) {
        hook::Thunk<0x445820>::Call<void>(this, a1, a2, a3, a4);
    }

    AGE_API void cityLevel::SetupLighting(Vector3 const& multiplyColor)
    {
        hook::StaticThunk<0x4436A0>::Call<void>(&multiplyColor);
    }

    /*
        Helpers
    */
    bool cityLevel::IsRoomVisible(int roomId) const
    {
        if (!sm_EnablePVS.get())
            return true;
        if (roomId < 0 || roomId >= this->GetRoomCount())
            return false;

        auto buf = sm_PvsBuffer.ptr();
        return  ((buf[roomId >> 2] >> (2 * (roomId & 3))) & 3) != 0;
    }

    lvlSDL* cityLevel::GetSDL()
    {
        return SDL.ptr();
    }

    //lua
    void cityLevel::BindLua(LuaState L) {
        LuaBinding(L).beginExtendClass<cityLevel, lvlLevel>("cityLevel")
            //functions
            .addFunction("EnablePVS", &EnablePVS)
            .addFunction("EnableSky", &EnableSky)
            .addFunction("IsRoomVisible", &IsRoomVisible)

            .addStaticFunction("LoadPath", &LoadPath)
            .addStaticFunction("LoadPathSet", &LoadPathSet)
            .addStaticFunction("LoadProp", &LoadProp)
                
            //singleton
            .addStaticProperty("Singleton", [] 
            { 
                //MM2 only uses cityLevel so this works
                auto level = lvlLevel::GetSingleton();
                return reinterpret_cast<cityLevel*>(level);
            })

            //sky singleton
            .addStaticProperty("Sky", [] { return &Sky; })
            .addStaticProperty("SDL", &GetSDL)
            .endClass();
    }
}