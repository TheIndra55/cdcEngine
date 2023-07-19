#pragma once

namespace cdc
{
	class PCInternalResource
	{
	private:
		PCInternalResource* m_pPrev;
		PCInternalResource* m_pNext;

	public:
		PCInternalResource();

		void OnConstruct();
		void OnDestruct();

		virtual bool OnCreateDevice() = 0;
		virtual void OnDestroyDevice() = 0;
	};
}