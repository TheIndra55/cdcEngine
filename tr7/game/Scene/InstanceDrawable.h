#pragma once

#include "cdc/runtime/cdcRender/pc/shared/PCDrawable.h"
#include "cdc/runtime/cdcRender/pc/PCRenderModel.h"

#include "game/Instance.h"

class InstanceDrawable : public cdc::IDrawable
{
private:
	BaseInstance* m_pInstance;
	cdc::RenderModelInstance* m_sciRMI;

public:
	InstanceDrawable(BaseInstance* pInstance);

	void Draw(cdc::Matrix* localToWorld, bool isMoved);
};