#include "InstanceDrawable.h"

InstanceDrawable::InstanceDrawable(BaseInstance* pInstance)
{
	m_pInstance = pInstance;
	m_sciRMI = nullptr;
}

void InstanceDrawable::Draw(cdc::Matrix* localToWorld, bool isMoved)
{
	auto model = m_pInstance->object->modelList[m_pInstance->currentRenderModel];

	if (model)
	{
		auto modelInstance = (cdc::RenderModelInstance*)model->cdcRenderModelData;

		if (modelInstance)
		{
			modelInstance->Draw(localToWorld);
		}
	}
}