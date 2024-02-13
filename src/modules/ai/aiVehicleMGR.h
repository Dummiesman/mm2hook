#pragma once
#include <modules\ai\aiVehicleData.h>
#include <modules\level\inst.h>

namespace MM2
{
    // Forward declarations
    class aiVehicleActive;
    class aiVehicleManager;
    class aiVehicleInstance;

    // External declarations
    extern class asNode;
    extern class ltLight;
    extern class aiVehicleSpline;

    // Class definitions
    class aiVehicleActive 
    {
        byte buffer[0xAFC];
    };

    class aiVehicleManager : public asNode {
    private:
        aiVehicleData vehicleDatas[32];
        int numVehicleDatas;
        aiVehicleActive* activeActives[32];
        aiVehicleActive aiVehicleActives[32];
        __int16 attachedCount;
        ltLight* sharedLight;
    public:
        static hook::Type<aiVehicleManager *> Instance;
        static hook::Type<int> SignalClock;

        AGE_API aiVehicleManager(void);
        AGE_API virtual ~aiVehicleManager(void);

        //members
        AGE_API void Init(char *unused);
        AGE_API int AddVehicleDataEntry(LPCSTR name);
        AGE_API void SaveEntry();
        AGE_API void Attach(aiVehicleInstance* instance);
        AGE_API void Detach(aiVehicleActive* active);

        //asNode overrides
        AGE_API void Reset() override;
        AGE_API void Update() override;

        //helpers
        int GetDataCount();
        aiVehicleData *GetData(int num);

        //lua
        static void BindLua(LuaState L);
    };

    class aiVehicleInstance : public lvlInstance {
    public:
        static const int HLIGHT_GEOM_ID = 2;
        static const int TLIGHT_GEOM_ID = 3;
        static const int SLIGHT0_GEOM_ID = 4;
        static const int SLIGHT1_GEOM_ID = 5;
        static const int BLIGHT_GEOM_ID = 18;
        static const int PLIGHTON_GEOM_ID = 19;
        static const int PLIGHTOFF_GEOM_ID = 20;
        static const int TSLIGHT0_GEOM_ID = 21;
        static const int TSLIGHT1_GEOM_ID = 22;
    public:
        static int AmbientHeadlightStyle;
    protected:
        static hook::Field<0x14, aiVehicleSpline*> _spline;
        static hook::Field<0x1E, short> _variant;
    public:
        aiVehicleInstance(void)             DONOTCALL;

        //properties
        aiVehicleSpline* GetSpline();
        int GetVariant() const;

        //overrides
        AGE_API Vector3 const& GetPosition() override;
        AGE_API Matrix34 const& GetMatrix(Matrix34& a1) override;
        AGE_API void SetMatrix(Matrix34 const& a1) override;
        AGE_API dgPhysEntity* GetEntity() override;
        AGE_API dgPhysEntity* AttachEntity() override;
        AGE_API void Detach() override;
        AGE_API void DrawShadow() override;
        AGE_API void DrawShadowMap() override;

        AGE_API void Draw(int a1) override;

        AGE_API void DrawReflected(float a1) override;
        AGE_API unsigned int SizeOf() override;
        AGE_API phBound* GetBound(int a1) override;

        //members
        aiVehicleData* GetData();
        AGE_API void DrawPart(modStatic* a1, const Matrix34& a2, modShader* a3, int a4);

        //lua
        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiVehicleManager, 0x177A4);
}