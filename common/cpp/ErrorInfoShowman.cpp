#include "ErrorInfoShowman.h"

//C++ standart includes
#include <iostream>

//show custom error msg
void ShowEM(const char* fileName, unsigned int lineNum, const char* codePart, const char* pErrorDefinition)
{
	std::cout
		<< "Welcome to hell of errors!"
		<< "\nFile: " << fileName
		<< "\nLine: " << lineNum
		<< "\nError code: " << codePart
		<< "\nReason: " << pErrorDefinition
		<< '\n';
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