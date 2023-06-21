#pragma once
#include "aiPedAudio.h"

namespace MM2
{
    // Forward declarations
    class aiPedestrian;

    // External declarations
    extern class aiPedestrianInstance;

    // Class definitions

    class aiPedestrian {
    private:
        byte _buffer[0x134];
    protected:
        static hook::Field<0x08, short> _id;
        static hook::Field<0x3C, Matrix34> _matrix;
        static hook::Field<0x6C, Vector3> _targetPoint;
        static hook::Field<0xC0, aiPedAudio> _audio;
        static hook::Field<0x9C, aiPedestrianInstance *> _instance;
    public:
        AGE_API void Update();
        void DrawDebug() const;

        aiPedestrianInstance* GetInstance() const;
        aiPedAudio* GetAudio() const;

        static void BindLua(LuaState L);
    };

    ASSERT_SIZEOF(aiPedestrian, 0x134);
}