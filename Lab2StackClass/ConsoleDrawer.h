#pragma once

//WinAPI includes:
#include <Windows.h>

//C++ standart includes:
#include <cstdint>

namespace console_drawer
{
	extern COORD bufferSize;
	extern int32_t pixelAmountTotal;

	class ConsoleImage
	{
		WORD* pAttributes;



	public:

		ConsoleImage(WORD resourceID_JPEG);

		friend bool DrawImage(ConsoleImage& consoleImage, COORD originalSize, COORD positionOfLeftUpperCorner, COORD desiredSize);

		friend bool DrawImage(ConsoleImage& consoleImage, COORD originalSize, COORD positionOfLeftUpperCorner);

		~ConsoleImage();
	};

	void Initialize(SHORT argFontSize = 12, COORD argBufferSize = { 120, 50 });

	bool DrawPixel(WORD color, COORD coord);

	bool DrawImage(ConsoleImage& consoleImage, COORD originalSize, COORD positionOfLeftUpperCorner, COORD desiredSize);

	bool DrawImage(ConsoleImage& consoleImage, COORD originalSize, COORD positionOfLeftUpperCorner);

	void Present(bool cleanAfterPresenting = true);

	void Uninitialize();
}