#pragma once
#include "phforcesphere.h"
#include <modules\level.h>

namespace MM2
{
    // Forward declarations
    class dgExplosionEntity;
    class dgExplosionInstance;
    class dgBombInfo;

    // External declarations
    extern class phMaterial;    

    // Class definitions
    class dgBombInfo : public asFileIO
    {
    public:
        float Strength;
        float RadiusMin;
        float RadiusMax;
        float ExplodeTime;
    public:
        virtual AGE_API void FileIO(datParser& parser) 
        {
            parser.AddValue("Strength", &Strength);
            parser.AddValue("RadiusMin", &RadiusMin);
            parser.AddValue("RadiusMax", &RadiusMax);
            parser.AddValue("ExplodeTime", &ExplodeTime);
        };
        
        virtual AGE_API const char* GetDirName(void) 
        {
            return "tune/banger";
        };

        virtual AGE_API char* GetClassName(void) 
        {
            return "dgBombInfo";
        };

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgBombInfo, asFileIO>("dgBombInfo")
                .addConstructor(LUA_ARGS())

                .addVariableRef("Strength", &dgBombInfo::Strength)
                .addVariableRef("RadiusMin", &dgBombInfo::RadiusMin)
                .addVariableRef("RadiusMax", &dgBombInfo::RadiusMax)
                .addVariableRef("ExplodeTime", &dgBombInfo::ExplodeTime)
                .endClass();
        }
    };

    class dgExplosionEntity : public dgPhysEntity
    {
    private:
        lvlInstance* Instance;
    public:
        dgExplosionEntity()
        {

        }

        virtual AGE_API void PostUpdate() override
        {
        }

        virtual AGE_API phInertialCS* GetICS() override
        {
            return this->collider.GetICS();
        }

        virtual AGE_API lvlInstance* GetInst() override
        {
            return this->Instance;
        }

        void SetInstance(lvlInstance * instance)
        {
            this->Instance = instance;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgExplosionEntity, dgPhysEntity>("dgExplosionEntity")
                .addConstructor(LUA_ARGS())

                .addFunction("SetInstance", &SetInstance)

                .endClass();
        }
    };

    class dgExplosionInstance : public lvlInstance
    {
    private:
        Vector3 Position;
        phForceSphere Sphere;
        BOOL Active;
        dgPhysEntity* Entity;
    public:
        dgExplosionInstance::dgExplosionInstance()
        {
            
        }
        
        virtual AGE_API const float GetRadius() override
        {
            return this->Sphere.getCurrentRadius();
        }

        virtual AGE_API dgPhysEntity* GetEntity() override
        {
            return this->Entity;
        }

        virtual AGE_API const Vector3& GetPosition() override 
        {
            return this->Position;
        }

        virtual AGE_API const Matrix34& GetMatrix(Matrix34* a1) override
        {
            Matrix34 mtx;
            mtx.Identity();
            mtx.SetRow(3, this->Position);
            a1->Set(mtx);
            return *a1;
        }
                             
        virtual AGE_API phBound* GetBound(int a1) override
        {
            return &this->Sphere;
        }

        virtual AGE_API void SetMatrix(const Matrix34& a1)
        {
            this->Position = Matrix34(a1).GetRow(3);
        }

        virtual AGE_API void Draw(int)
        {
        }
        
        virtual AGE_API unsigned int SizeOf(void)
        {
            return sizeof(dgExplosionInstance);
        }

        virtual AGE_API void Reset() override
        {
            this->Deactivate();
            lvlInstance::Reset();
        }

        bool IsActive()
        {
            return this->Active == TRUE;
        }

        void Activate() 
        {
            this->Active = TRUE;
        }

        void Deactivate() 
        {
            this->Active = FALSE;
        }

        void Update() 
        {
            if (this->Active == FALSE)
                return;

            if (this->Sphere.getIsRunning())
            {
                auto position = this->GetPosition();
                int room = lvlLevel::Singleton->FindRoomId(position, this->getRoomId());
                if (room != this->getRoomId())
                    lvlLevel::Singleton->MoveToRoom(this, room);

                dgPhysManager::Instance->DeclareMover(this, 3, 0x1A); // Verify flags, they're from Test Drive
                this->Sphere.Update();
            }
            else
            {
                this->Active = FALSE;
                lvlLevel::Singleton->MoveToRoom(this, 0);
            }
        }

        void Detonate(Matrix34 & matrix, const dgBombInfo & bombInfo)
        {
            if (this->Entity == nullptr)
                return;

            auto &sphere = this->Sphere;

            this->SetMatrix(matrix);
            this->Entity->GetCollider()->Init(&matrix, &sphere);
            
            sphere.Reset();
            sphere.SetCenter(matrix.GetRow(3));
            sphere.SetRadiusGrow(bombInfo.RadiusMin, bombInfo.RadiusMax, (bombInfo.RadiusMax - bombInfo.RadiusMin) / bombInfo.ExplodeTime);
            sphere.SetTimeAndFade(bombInfo.ExplodeTime, 1.0f);
            sphere.setForceCoef(bombInfo.Strength);
            sphere.Start();

            this->Activate();
        }

        void Detonate(Vector3& position, const dgBombInfo& bombInfo)
        {
            auto mtx = Matrix34();
            mtx.Identity();
            mtx.SetRow(3, position);
            this->Detonate(mtx, bombInfo);
        }

        void SetEntity(dgExplosionEntity* entity)
        {
            Entity = entity;
        }

        static void BindLua(LuaState L) {
            LuaBinding(L).beginExtendClass<dgExplosionInstance, lvlInstance>("dgExplosionInstance")
                .addConstructor(LUA_ARGS())

                .addPropertyReadOnly("Active", &IsActive)

                .addFunction("Activate", &Activate)
                .addFunction("Deactivate", &Deactivate)
                .addFunction("Detonate", static_cast<void(dgExplosionInstance::*)(Vector3&, const dgBombInfo&)>(&dgExplosionInstance::Detonate))
                .addFunction("SetEntity", &SetEntity)
                .addFunction("Update", &Update)

                .endClass();
        }
    };
    ASSERT_SIZEOF(phBound, 0x4C);

    // Lua initialization

}