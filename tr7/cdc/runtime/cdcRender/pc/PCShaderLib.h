#pragma once

namespace cdc
{
	class IShaderLib
	{
	public:
		virtual void PreWarm() = 0;
		virtual void Release() = 0;
	};
}