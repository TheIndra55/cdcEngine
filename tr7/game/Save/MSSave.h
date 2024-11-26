#pragma once

#include <Windows.h>

#include "cdc/runtime/cdcSys/UTF8String.h"

class MSSave
{
private:
	static MSSave* s_instance;

public:
	static void Create();
};