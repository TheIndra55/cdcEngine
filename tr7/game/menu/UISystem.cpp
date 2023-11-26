#include "UISystem.h"

UISystem* UISystem::s_system;

void UISystem::Init(UISystem* system)
{
	s_system = system;
}

void UISystem::InitInGameUI(int inGameUIMemorySize)
{
}

void UISystem::InitFrontEndUI(int frontEndUIMemorySize)
{
}

UIScreen* UISystem::GetScreen(__int16 screenID)
{
	return nullptr;
}

UIMenu* UISystem::GetMenu(__int16 menuID)
{
	return nullptr;
}

void UISystem::MarkFrontEndDump(bool dump)
{
}

UIMenu* UISystem::GetSystemMenu(UISystemDTP* systemDTP, __int16 menuID, char* debugText)
{
	return nullptr;
}

UIScreen* UISystem::GetSystemScreen(UISystemDTP* systemDTP, __int16 screenID, char* debugText)
{
	return nullptr;
}