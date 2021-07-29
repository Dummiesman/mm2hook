#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmGameHandler : handler_t {
public:
    void SendChatMessage(char *message);
    void InitWeather(void);
    void UpdateSteeringBrakes(void);
    void UpdateHorn(bool a1);

    static void Install();
};

