#pragma once

namespace cdc
{
	struct PCShaderId
	{
		unsigned __int64 hi;
		unsigned __int64 lo;
	};

	class PCShaderBinary
	{
	public:
		PCShaderId* m_pId;
		unsigned int* m_pFunction;
		bool m_isOwner;

		PCShaderBinary(unsigned int* pData, bool copyData);
	};
}