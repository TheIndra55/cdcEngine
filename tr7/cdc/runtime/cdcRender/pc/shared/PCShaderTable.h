#pragma once

#include "PCVertexShader.h"

namespace cdc
{
	class PCShaderTable
	{
	private:
		char* m_pBasePtr;
		unsigned int* m_pOffsetTable;
		unsigned int m_tableSize;
		unsigned int m_dataSize;

	public:
		PCShaderTable(void* pShaderTableData);

		unsigned int GetSize();

		unsigned int* GetShaderFunction(unsigned int index);
		bool ShaderExists(unsigned int index);

		virtual void PreWarmShader(unsigned int index) = 0;
	};

	class PCVertexShaderTable : PCShaderTable
	{
	private:
		PCVertexShader** m_pVertexShaders;
		bool m_copyData;

	public:
		PCVertexShaderTable(void* pShaderTableData, bool copyData, bool neverPrecreate);

		void PreWarmShader(unsigned int index);
		PCVertexShader* GetVertexShader(unsigned int index);
	};
}