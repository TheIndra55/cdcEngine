#pragma once

namespace cdc
{
	class PCScene
	{
	public:
		virtual unsigned int GetId();
		virtual void Begin();
		virtual void End();
	};
}