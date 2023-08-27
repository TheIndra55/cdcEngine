#pragma once

namespace cdc
{
	class RenderResource
	{
	public:
		virtual void Release() = 0;
		virtual int GetMemorySize() = 0;
	};
}