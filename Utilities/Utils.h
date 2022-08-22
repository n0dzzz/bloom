#include <Windows.h>
#include <iostream>
#include <string>
#include "ColorPrint.h"
#include "Math/Math.h"

namespace Utils
{
	void AttachConsole(const char* name);
	void DetachConsole();
	void ConsolePrint(const char* msg, int msgType);

	/*
	 * msg type 0 - Normal (white txt)
	 * msg type 1 - Warning (yellow txt)
	 * msg type 2 - Critical Warning / Error (red txt)
	 * msg type 3 - Critical Warning / Error (red txt)
	 */

	std::uint8_t* PatternScan(const char* module, const char* signature);
	
}