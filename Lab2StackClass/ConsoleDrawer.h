#pragma once

//WinAPI includes:
#include <Windows.h>

//C++ standart includes:
#include <cstdint>

namespace console_drawer
{
	extern COORD bufferSize;
	extern int32_t pixelAmountTotal;

	void Initialize(SHORT argFontSize = 12, COORD argBufferSize = { 120, 50 });

	bool DrawPixel(WORD color, COORD coord);

	bool DrawImage(HANDLE hImage, COORD originalSize, COORD positionOfLeftUpperCorner, COORD desiredSize);

	bool DrawImage(HANDLE hImage, COORD originalSize, COORD positionOfLeftUpperCorner);

	void Present(bool cleanAfterPresenting = true);

	void Uninitialize();
}