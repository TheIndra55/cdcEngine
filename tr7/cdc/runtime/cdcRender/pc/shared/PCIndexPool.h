#pragma once

#include "PCInternalResource.h"
#include "PCDeviceManager.h"
#include "PCDynamicIndexBuffer.h"

namespace cdc
{
	class PCIndexPool : PCInternalResource
	{
	private:
		struct Bin
		{
			IDirect3DIndexBuffer9* pD3DIndexBuffer;
			unsigned __int16* pIndexData;
			unsigned int indexOffset;
			Bin* pNext;
		};

		Bin* m_pActiveBin;
		Bin* m_pIndexBins;

	public:
		PCIndexPool(unsigned int numInitialBins);

		cdc::PCDynamicIndexBuffer* Create(unsigned int numIndices);
		Bin* CreateBin();
		bool CreateBinIndexBuffer(Bin* pBin);

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}