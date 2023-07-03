#include "print_handler.h"
#include <modules\core\output.h>

using namespace MM2;

/*
    PrintHandler
*/
void PrintHandler::FatalError(LPCSTR message) {
    // TODO: implement fatal message handler?
}

void PrintHandler::Print(int level, const char* format, va_list args)
{
    static char* Prefixes[6] = {
        "",                 // print
        "",                 // message
        "",                 // display
        "Warning: ",        // warning
        "Error: ",          // error
        "Fatal Error: ",    // quit/abort
    };

    static char* Suffixes[6] = {
        "",                 // print
        "\n",               // message
        "\n",               // display
        "\n",               // warning
        "\n",               // error
        "\n",               // quit/abort
    };

    static short printer_types[] = {
        TEXTCOLOR_DARKGRAY, // print
        TEXTCOLOR_DARKGRAY, // message
        TEXTCOLOR_DARKGRAY, // display
        TEXTCOLOR_YELLOW,   // warning
        TEXTCOLOR_LIGHTRED, // error
        TEXTCOLOR_LIGHTRED, // quit/abort
    };

    static char FormatBuffer[4096]{ NULL };
    static char PrintBuffer[4096]{ NULL };

    vsprintf_s(FormatBuffer, format, args);

    bool showPopupError = datOutput::ShowPopupErrors && (level == 4);
    bool showPopupQuit = datOutput::ShowPopupQuits && (level == 5);

    if (showPopupError || showPopupQuit)
    {
        datOutput::CallBeforeMsgBoxFunction();

        MessageBoxA(NULL, FormatBuffer, Prefixes[level], MB_ICONERROR);

        if (level == 4)
            datOutput::CallAfterMsgBoxFunction();
    }

    sprintf_s(PrintBuffer, "%s%s%s", Prefixes[level], FormatBuffer, Suffixes[level]);

    if (datOutput::sm_Stream)
    {
        fprintf(datOutput::sm_Stream, "%s", PrintBuffer);
        if (level >= 5) datOutput::sm_Stream->Flush();
    }

    OutputDebugStringA(PrintBuffer);

    if (ConsoleLog::IsEnabled()) {
        HANDLE hConsole = ConsoleLog::GetOutputHandle();

        SetConsoleTextAttribute(hConsole, printer_types[level]);
        ConsoleLog::Write(PrintBuffer);
        SetConsoleTextAttribute(hConsole, TEXTCOLOR_LIGHTGRAY);
    }

    if (level == 5)
        FatalError(FormatBuffer);

    datOutput::OutputSent = 1;
}

void PrintHandler::Install() {
    Printer = &Print;
}