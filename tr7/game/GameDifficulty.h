#pragma once

class GameDifficultySettings
{
public:
	struct GameDifficulty
	{
		float enemyAccuracyMult;
		float ememyDamageMult;
		float healthInitMult;
		float healthDecreaseMult;
		float healthPackDropMult;
		float savingGrabTime;
	};

	GameDifficulty m_setting[3];
};