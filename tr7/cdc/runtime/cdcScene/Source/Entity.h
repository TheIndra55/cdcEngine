#pragma once

#include "cdc/runtime/cdcMath/Matrix.h"
#include "cdc/runtime/cdcRender/pc/shared/PCDrawable.h"

namespace cdc
{
	class ISceneEntity
	{
		virtual void SetMatrix(cdc::Matrix* matrix) = 0;
		virtual void SetDrawable(IDrawable* drawable) = 0;
		virtual cdc::IDrawable* GetDrawable() = 0;
		virtual void Update() = 0;
	};

	class SceneEntity : public ISceneEntity
	{
	private:
		Scene* m_pScene;
		int m_entityIndex;

		Matrix m_matrix;
		IDrawable* m_pDrawable;

	public:
		SceneEntity(Scene* pScene);

		void SetMatrix(cdc::Matrix* matrix);
		void SetDrawable(IDrawable* drawable);
		cdc::IDrawable* GetDrawable();
		void Update();
	};
}