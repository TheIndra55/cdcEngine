#pragma once

#include "PCInternalResource.h"

namespace cdc
{
	class PCStateManager : public PCInternalResource
	{
	public:
		PCStateManager();

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}