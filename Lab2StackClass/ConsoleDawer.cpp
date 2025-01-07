#include "ConsoleDrawer.h"

//WinAPI includes:
#include <Windows.h>

//C++ standart includes:
#include <cstdint>

HWND hwnd;
HANDLE hConsoleInput, hConsoleOutput;

void initialize(uint16_t spaceWidth, uint16_t spaceHeight)
{
    hwnd = GetConsoleWindow();
    LONG style = GetWindowLongA(hwnd, GWL_STYLE);
    style &= ~WS_CAPTION & ~WS_THICKFRAME;
    SetWindowLongA(hwnd, GWL_STYLE, style);
    //SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);



    hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hConsoleInput, &mode);
    mode &= ~ENABLE_LINE_INPUT & ~ENABLE_ECHO_INPUT & ~ENABLE_PROCESSED_INPUT & ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_INSERT_MODE;
    SetConsoleMode(hConsoleInput, mode);



    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD bufferSize;
    bufferSize.X = spaceWidth;
    bufferSize.Y = spaceHeight;
    SetConsoleScreenBufferSize(hConsoleOutput, bufferSize);

    SMALL_RECT windowSize;
    windowSize.Left = 0;
    windowSize.Right = spaceWidth - 1;
    windowSize.Top = 0;
    windowSize.Bottom = spaceHeight - 1;
    SetConsoleWindowInfo(hConsoleOutput, TRUE, &windowSize);

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}