#include "WindowsErrorInfoShowman.h"

//C++ standart includes
#include <string>

namespace WEISH
{
	HWND hParentWnd;
	//Must be called first.
	//hOutputBuffer is needed only when the BUFFER mode is used.
	void Initialize(HWND arg_hParentWnd)
	{
		hParentWnd = arg_hParentWnd;
	}

	//show custom error msg
	void ShowEM(const char* fileName, unsigned int lineNum, const char* codePart, const char* pErrorDefinition)
	{
		std::string str;
		(str += "File: ") += fileName;
		(str += "\nLine: ") += std::to_string(lineNum);
		(str += "\nError code: ") += codePart;
		(str += "\nReason: ") += pErrorDefinition;
		MessageBoxA(hParentWnd, str.c_str(), "Welcome to hell of errors!", MB_ICONERROR);
	}

	//show error msg by code
	void ShowEM(const char* fileName, unsigned int lineNum, const char* codePart, DWORD ec)
	{
		char* pErrorDefinition;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			ec,
			0,
			(LPSTR)&pErrorDefinition,
			0,
			nullptr
		);
		ShowEM(fileName, lineNum, codePart, pErrorDefinition);
		LocalFree(pErrorDefinition);
	}
}