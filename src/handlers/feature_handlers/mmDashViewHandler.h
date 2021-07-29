#pragma once
#include "common.h"
#include "patch.h"
#include "mm2.h"

class mmDashViewHandler {
public:
    void UpdateCS();
    void FileIO(MM2::datParser* parser);

    static void Install();
};