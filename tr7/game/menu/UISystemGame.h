#pragma once

#include "UISystem.h"

class UISystemGame : public UISystem
{
private:
	UISystemDTP* m_mansionUI;

public:
	UISystemGame();

	void InitInGameUI(int inGameUIMemorySize);
	void InitFrontEndUI(int frontEndUIMemorySize);
	UIScreen* GetScreen(__int16 screenID);
	UIMenu* GetMenu(__int16 menuID);
};