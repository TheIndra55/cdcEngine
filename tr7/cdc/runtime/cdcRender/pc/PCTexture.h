#pragma once

#include "cdc/runtime/cdcRender/RenderResource.h"

namespace cdc
{
	class TextureMap : public RenderResource
	{
	public:
		void Release();
		int GetMemorySize();
	};

	class PCTexture : public TextureMap
	{
	};
}