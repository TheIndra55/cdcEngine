#include "PCIndexPool.h" 

cdc::PCIndexPool::PCIndexPool(unsigned int numInitialBins) : PCInternalResource()
{
	m_pActiveBin = nullptr;
	m_pIndexBins = nullptr;

	if (numInitialBins == 0)
	{
		numInitialBins = 1;
	}

	for (int i = 0; i < numInitialBins; i++)
	{
		auto deviceManager = cdc::PCDeviceManager::s_pInstance;
		auto bin = new Bin();

		if (deviceManager->IsStatusOk() && !deviceManager->IsCreatingResources())
		{
			if (FAILED(deviceManager->GetD3DDevice()->CreateIndexBuffer(
				0x1FFFE,
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_DEFAULT,
				&bin->pD3DIndexBuffer,
				NULL)))
			{
				OnCreateResourceFailed();
			}
		}

		bin->pNext = m_pIndexBins;
		m_pIndexBins = bin;
	}

	m_pActiveBin = m_pIndexBins;
}

cdc::PCDynamicIndexBuffer* cdc::PCIndexPool::Create(unsigned int numIndices)
{
	auto offset = 0;

	while (true)
	{
		auto bin = m_pActiveBin;

		if (bin->indexOffset + numIndices <= 65535)
		{
			offset = bin->indexOffset;
			bin->indexOffset += numIndices;

			break;
		}

		if (!bin->pNext)
		{
			auto newBin = CreateBin();

			bin->pNext = newBin;
			m_pActiveBin = newBin;

			newBin->indexOffset = numIndices;
		}

		m_pActiveBin = bin;
	}

	auto bin = m_pActiveBin;

	bin->pD3DIndexBuffer->Lock(0, 0, (void**)&bin->pIndexData, D3DLOCK_DISCARD);

	return new cdc::PCDynamicIndexBuffer(bin->pD3DIndexBuffer, (void*)&bin->pIndexData[offset], offset, numIndices);
}

cdc::PCIndexPool::Bin* cdc::PCIndexPool::CreateBin()
{
	auto deviceManager = cdc::PCDeviceManager::s_pInstance;
	auto bin = new Bin();

	if (deviceManager->IsStatusOk() && !deviceManager->IsCreatingResources())
	{
		CreateBinIndexBuffer(bin);
	}

	return bin;
}

bool cdc::PCIndexPool::CreateBinIndexBuffer(Bin* pBin)
{
	if (FAILED(cdc::PCDeviceManager::s_pInstance->GetD3DDevice()->CreateIndexBuffer(
		0x1FFFE,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&pBin->pD3DIndexBuffer,
		NULL)))
	{
		OnCreateResourceFailed();

		return false;
	}

	return true;
}

void cdc::PCIndexPool::EndScene()
{
	for (Bin* bin = m_pIndexBins; bin != nullptr; bin = bin->pNext)
	{
		if (bin->pIndexData)
		{
			bin->pD3DIndexBuffer->Unlock();
			bin->pIndexData = nullptr;
		}

		bin->indexOffset = 0;
	}

	m_pActiveBin = m_pIndexBins;
}

bool cdc::PCIndexPool::OnCreateDevice()
{
	for (Bin* bin = m_pIndexBins; bin != nullptr; bin = bin->pNext)
	{
		if (FAILED(cdc::PCDeviceManager::s_pInstance->GetD3DDevice()->CreateIndexBuffer(
			0x1FFFE,
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_DEFAULT,
			&bin->pD3DIndexBuffer,
			NULL)))
		{
			OnCreateResourceFailed();

			return false;
		}
	}

	m_pActiveBin = m_pIndexBins;

	return true;
}

void cdc::PCIndexPool::OnDestroyDevice()
{
	for (Bin* bin = m_pIndexBins; bin != nullptr; bin = bin->pNext)
	{
		if (bin->pD3DIndexBuffer)
		{
			bin->pD3DIndexBuffer->Release();
			bin->pD3DIndexBuffer = nullptr;
		}
	}

	m_pActiveBin = m_pIndexBins;
}