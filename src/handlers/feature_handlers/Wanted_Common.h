/*
    Common stuff that Wanted9811's additions share 
*/

#pragma once

struct OppIconInfo;

class Wanted_Common
{
public:
    static float bustedTimer;
    static int bustedTarget;
    static float bustedMaxSpeed;
    static float bustedTimeout;
    static float oppBustedTimer;
    static float resetTimer;
    static bool enableBustedTimer;
    static bool enableOppBustedTimer;
    static bool enableResetTimer;
};

struct OppIconInfo {
    int color;
    char buffer[0x24];
};

//ASSERT_SIZEOF(OppIconInfo, 0x28);