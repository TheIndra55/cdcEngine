#pragma once

namespace cdc
{
	class IDrawable
	{
	public:
		virtual void Draw(cdc::Matrix* matrix, bool isMoved) = 0;
	};
}