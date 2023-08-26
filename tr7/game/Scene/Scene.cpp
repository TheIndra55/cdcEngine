#include "Scene.h"

bool SceneLayer::s_enabled = false;

void SceneLayer::SetEnabled(bool enabled)
{
	s_enabled = enabled;
}