#include <Windows.h>
#include <d3d9types.h>

#include "PCShaderBinary.h"

cdc::PCShaderBinary::PCShaderBinary(unsigned int* pData, bool copyData)
{
	m_isOwner = copyData;
	m_pId = nullptr;
	m_pFunction = 0;

	unsigned int* data = pData;

	if (copyData)
	{
		int i = 1;
		while (pData[i] != D3DSIO_END)
		{
			i++;
		}

		data = new unsigned int[i];
		memcpy(data, pData, sizeof(unsigned int) * i);
	}

	//if (pData[0] == 0xBAADF00D && pData[1] == 0xDEADBABE)
	//{
	//	m_pId = (cdc::PCShaderId*)data;
	//	m_pFunction = data + 4;
	//}
	//else
	//{
	//	m_pId = 
	//	m_pFunction = data;
	//}

	// TODO
	m_pFunction = pData;
}