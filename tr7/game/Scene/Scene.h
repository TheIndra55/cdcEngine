#pragma once

#include "cdc/runtime/cdcScene/Source/SceneCell.h"
#include "cdc/runtime/cdcScene/Source/SceneManager.h"

class SceneLayer
{
public:
	static bool s_enabled;

	static cdc::IScene* s_pGlobalScene;
	static cdc::SceneId s_globalSceneId;

	static void Init();
	static void Update();
	static void Render(cdc::ISceneCell* pStartCell);
	static void SetEnabled(bool enabled);
};