//WinAPI includes
#include <Windows.h>

#define fl_args __FILE__, __LINE__, "Not supported yet"

//show custom error msg
void ShowEM(const char* fileName, unsigned int lineNum, const char* codePart, const char* pErrorDefinition);

//show error msg by code
void ShowEM(const char* fileName, unsigned int lineNum, const char* codePart, DWORD ec);