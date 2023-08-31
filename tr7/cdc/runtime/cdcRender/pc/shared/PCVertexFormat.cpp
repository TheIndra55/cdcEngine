#include "PCVertexFormat.h"
#include "PCDeviceManager.h"

cdc::PCVertexFormat* cdc::PCVertexFormat::s_pFirstFormat = nullptr;

cdc::PCVertexFormat::PCVertexFormat(D3DVERTEXELEMENT9* pVertexElements) : PCInternalResource()
{
	m_pVertexElements = nullptr;
	m_pD3DVertexDeclaration = nullptr;
	m_stride = 0;
	m_pNextFormat = nullptr;

	int i = 0;
	auto element = pVertexElements;
	while (element->Stream != 0xff)
	{
		switch (element->Type)
		{
		case D3DDECLTYPE_FLOAT1:
		case D3DDECLTYPE_D3DCOLOR:
		case D3DDECLTYPE_UBYTE4:
		case D3DDECLTYPE_SHORT2:
		case D3DDECLTYPE_UBYTE4N:
		case D3DDECLTYPE_SHORT2N:
		case D3DDECLTYPE_USHORT2N:
		case D3DDECLTYPE_UDEC3:
		case D3DDECLTYPE_DEC3N:
		case D3DDECLTYPE_FLOAT16_2:
			m_stride += 4;

			break;

		case D3DDECLTYPE_FLOAT2:
		case D3DDECLTYPE_SHORT4:
		case D3DDECLTYPE_SHORT4N:
		case D3DDECLTYPE_USHORT4N:
		case D3DDECLTYPE_FLOAT16_4:
			m_stride += 8;

			break;

		case D3DDECLTYPE_FLOAT3:
			m_stride += 12;

			break;

		case D3DDECLTYPE_FLOAT4:
			m_stride += 16;

			break;
		}

		i++;
		element++;
	}

	i++;
	m_pVertexElements = new D3DVERTEXELEMENT9[i];
	memcpy(m_pVertexElements, pVertexElements, i * sizeof(D3DVERTEXELEMENT9));

	OnConstruct();
}

bool cdc::PCVertexFormat::OnCreateDevice()
{
	auto device = cdc::PCDeviceManager::s_pInstance->m_pD3DDevice;

	if (FAILED(device->CreateVertexDeclaration(m_pVertexElements, &m_pD3DVertexDeclaration)))
	{
		OnCreateResourceFailed();
		return false;
	}

	return true;
}

void cdc::PCVertexFormat::OnDestroyDevice()
{
}

cdc::PCVertexFormat* cdc::PCVertexFormat::Create(D3DVERTEXELEMENT9* pVertexElements)
{
	PCVertexFormat* lastFormat = nullptr;

	if (s_pFirstFormat)
	{
		lastFormat = s_pFirstFormat;

		while (lastFormat->m_pNextFormat != nullptr)
		{
			lastFormat = lastFormat->m_pNextFormat;
		}
	}

	auto format = new PCVertexFormat(pVertexElements);

	if (lastFormat)
	{
		lastFormat->m_pNext = format;
	}
	else
	{
		s_pFirstFormat = format;
	}

	return format;
}