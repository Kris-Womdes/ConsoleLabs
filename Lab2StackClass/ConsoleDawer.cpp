#include "ConsoleDrawer.h"

//WinAPI includes:
#include <Windows.h>

//C++ standart includes:
#include <cstdint>

HANDLE hBufferInputDefault, hBufferOutputDefault,
hBufferOutputCustom_1, hBufferOutputCustom_2;
COORD bufferSize;
bool indexBuffer{ 0 };

HWND hwnd;

void initialize(uint16_t spaceWidth, uint16_t spaceHeight)
{
	//если всё, что делает /subsystem:console -- это создаёт буферы по умолчанию, то переключить на другое (но а если он еще и в ответе за окно и ещё что-нибудь?)

	hBufferInputDefault = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hBufferInputDefault, &mode);
	mode &= ~ENABLE_LINE_INPUT & ~ENABLE_ECHO_INPUT & ~ENABLE_PROCESSED_INPUT & ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_INSERT_MODE;
	SetConsoleMode(hBufferInputDefault, mode);

	bufferSize.X = spaceWidth;
	bufferSize.Y = spaceHeight;

	hBufferOutputDefault = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(hBufferOutputDefault, bufferSize);

	hBufferOutputCustom_1 = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
	SetConsoleScreenBufferSize(hBufferOutputCustom_1, bufferSize);
	hBufferOutputCustom_2 = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
	SetConsoleScreenBufferSize(hBufferOutputCustom_2, bufferSize);

	SetConsoleActiveScreenBuffer(hBufferOutputCustom_1);


	hwnd = GetConsoleWindow();
	LONG style = GetWindowLongA(hwnd, GWL_STYLE);
	style &= ~WS_CAPTION & ~WS_THICKFRAME;
	SetWindowLongA(hwnd, GWL_STYLE, style);

	SMALL_RECT windowSize;
	windowSize.Left = 0;
	windowSize.Right = spaceWidth - 1;
	windowSize.Top = 0;
	windowSize.Bottom = spaceHeight - 1;
	SetConsoleWindowInfo(hBufferOutputDefault, TRUE, &windowSize);



	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hBufferOutputDefault, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hBufferOutputDefault, &cursorInfo);
}

void uninitialize()
{
	CloseHandle(hBufferOutputCustom_1);
	CloseHandle(hBufferOutputCustom_2);
}

bool drawPixel()
{
	return true;
}

void swapBuffers()
{
	SetConsoleActiveScreenBuffer(indexBuffer ? hBufferOutputCustom_1 : hBufferOutputCustom_2);
	indexBuffer = !indexBuffer;
}