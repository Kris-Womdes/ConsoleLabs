#include "ConsoleDrawer.h"

//WinAPI includes:
#include <Windows.h>

//C++ standart includes:
#include <cstdint>

namespace console_drawer
{
	HANDLE hBufferOutputCustom;
	SHORT fontSize;
	COORD bufferSize{ 0,0 };
	int32_t pixelAmountTotal{ 0 };
	WORD* pAttributes;

	HWND hwnd;

	void DisableCursorOfBuffer(HANDLE hBuffer)
	{
		static CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hBuffer, &cursorInfo);
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(hBuffer, &cursorInfo);
	}

	void Initialize(SHORT argFontSize, COORD argBufferSize)
	{
		bufferSize = argBufferSize;

		hBufferOutputCustom = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
		SetConsoleScreenBufferSize(hBufferOutputCustom, bufferSize);

		SetConsoleActiveScreenBuffer(hBufferOutputCustom);

		SetConsoleOutputCP(CP_UTF8);

		CloseHandle(GetStdHandle(STD_INPUT_HANDLE));

		CloseHandle(GetStdHandle(STD_OUTPUT_HANDLE));

		CloseHandle(GetStdHandle(STD_ERROR_HANDLE));



		hwnd = GetConsoleWindow();
		LONG style = GetWindowLongA(hwnd, GWL_STYLE);
		style &= ~WS_CAPTION & ~WS_THICKFRAME;
		SetWindowLongA(hwnd, GWL_STYLE, style);

		SMALL_RECT windowSize;
		windowSize.Left = 0;
		windowSize.Right = bufferSize.X - 1;
		windowSize.Top = 0;
		windowSize.Bottom = bufferSize.Y - 1;
		SetConsoleWindowInfo(hBufferOutputCustom, TRUE, &windowSize);



		fontSize = argFontSize;
		CONSOLE_FONT_INFOEX fontInfo;
		fontInfo.cbSize = sizeof(fontInfo);
		fontInfo.nFont = 0;
		fontInfo.dwFontSize.Y = fontInfo.dwFontSize.X = fontSize;
		fontInfo.FontFamily = FF_DONTCARE;
		fontInfo.FontWeight = FW_NORMAL;
		wcscpy_s(fontInfo.FaceName, L"Lucida Console");
		SetCurrentConsoleFontEx(hBufferOutputCustom, FALSE, &fontInfo);

		DisableCursorOfBuffer(hBufferOutputCustom);

		pixelAmountTotal = bufferSize.X * bufferSize.Y;
		pAttributes = new WORD[pixelAmountTotal]{};
	}

	bool DrawPixel(WORD color, COORD coord)
	{
		if (coord.X >= 0 && coord.Y >= 0 && coord.X < bufferSize.X && coord.Y < bufferSize.Y)
		{
			pAttributes[bufferSize.X * coord.Y + coord.X] = color;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DrawImage(HANDLE hImage, COORD originalSize, COORD positionOfLeftUpperCorner, COORD desiredSize)
	{
		return true;
	}

	bool DrawImage(HANDLE hImage, COORD originalSize, COORD positionOfLeftUpperCorner)
	{
		return DrawImage(hImage, originalSize, positionOfLeftUpperCorner, originalSize);
	}

	void Present(bool cleanAfterPresenting)
	{
		DWORD attrWrittenAmount;
		WriteConsoleOutputAttribute(hBufferOutputCustom, pAttributes, pixelAmountTotal, { 0, 0 }, &attrWrittenAmount);
		if (cleanAfterPresenting)
		{
			for (WORD* pAttribute{ pAttributes }; pAttribute < pAttributes + pixelAmountTotal - 1; ++pAttribute)
			{
				*pAttribute = 0;
			}
		}
	}

	void Uninitialize()
	{
		CloseHandle(hBufferOutputCustom);
	}
}