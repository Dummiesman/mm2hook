#pragma once
#include <modules\ai\aiVehicleData.h>

namespace MM2
{
    // Forward declarations
    class aiVehicleActive;
    class aiVehicleManager;

    // External declarations
    extern class asNode;
    extern class ltLight;

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
        /*
        AGE_API aiVehicleActive Attach
        AGE_API aiVehicleActive Detach
        */

        //asNode overrides
        AGE_API void Reset() override;
        AGE_API void Update() override;

        //helpers
        int GetDataCount();
        aiVehicleData *GetData(int num);

        //lua
        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiVehicleManager, 0x177A4);
}