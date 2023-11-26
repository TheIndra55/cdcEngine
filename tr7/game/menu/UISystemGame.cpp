#include "UISystemGame.h"
#include "game/Stream/stream.h"
#include <game/Main.h>

UISystemGame::UISystemGame() : UISystem(), m_mansionUI(nullptr)
{
}

void UISystemGame::InitInGameUI(int inGameUIMemorySize)
{
	UISystem::InitInGameUI(inGameUIMemorySize);
}

void UISystemGame::InitFrontEndUI(int frontEndUIMemorySize)
{
	UISystem::InitFrontEndUI(frontEndUIMemorySize);
}

UIScreen* UISystemGame::GetScreen(__int16 screenID)
{
	if (screenID < 7000)
	{
		return UISystem::GetScreen(screenID);
	}
	else
	{
		return UISystem::GetSystemScreen(m_mansionUI, screenID, "UIMansion");
	}
}

UIMenu* UISystemGame::GetMenu(__int16 menuID)
{
	if (menuID < 7000)
	{
		return UISystem::GetMenu(menuID);
	}
	else
	{
		return UISystem::GetSystemMenu(m_mansionUI, menuID, "UIMansion");
	}
}