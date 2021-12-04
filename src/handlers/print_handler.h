#pragma once
#include <mm2_common.h>

class PrintHandler {
public:
    static void FatalError(LPCSTR message);
    static void Print(int level, const char* format, va_list args);
    
    static void Install();
};