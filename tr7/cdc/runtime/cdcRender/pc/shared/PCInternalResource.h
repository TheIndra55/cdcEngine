#pragma once

namespace cdc
{
	class PCInternalResource
	{
	public:
		PCInternalResource* m_pPrev;
		PCInternalResource* m_pNext;

		PCInternalResource();

		void OnConstruct();
		void OnDestruct();

		virtual bool OnCreateDevice() = 0;
		virtual void OnDestroyDevice() = 0;
	};
}