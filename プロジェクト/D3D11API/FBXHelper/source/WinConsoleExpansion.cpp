#include "Common.h"
#include "WinConsoleExpansion.h"


void ConsoleExpansion::WinConsoleExpansion::SetColor(WORD color)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h,
		color);
}
