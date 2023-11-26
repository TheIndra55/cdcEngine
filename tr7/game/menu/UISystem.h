#pragma once

#include "UIScreen.h"
#include "UIMenu.h"

struct UISystemDTP
{
	int screenCount;
	UIScreen** screenList;
	int menuCount;
	UIMenu** menuList;
};

class UISystem
{
public:
	virtual void InitInGameUI(int inGameUIMemorySize);
	virtual void InitFrontEndUI(int frontEndUIMemorySize);
	virtual UIScreen* GetScreen(__int16 screenID);
	virtual UIMenu* GetMenu(__int16 menuID);
	virtual void MarkFrontEndDump(bool dump);

	UIMenu* GetSystemMenu(UISystemDTP* systemDTP, __int16 menuID, char* debugText);
	UIScreen* GetSystemScreen(UISystemDTP* systemDTP, __int16 screenID, char* debugText);

public:
	static UISystem* s_system;

	static void Init(UISystem* system);
};