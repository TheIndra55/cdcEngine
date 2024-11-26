#include "MSSave.h"

MSSave* MSSave::s_instance = nullptr;

void MSSave::Create()
{
	s_instance = new MSSave();
}