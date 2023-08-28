#pragma once

#include "cdc/runtime/cdcMath/Matrix.h"

namespace cdc
{
	class IDrawable
	{
	public:
		virtual void Draw(cdc::Matrix* matrix, bool isMoved) = 0;
	};
}