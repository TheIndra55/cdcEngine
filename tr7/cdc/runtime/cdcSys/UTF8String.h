#pragma once

#include "Array.h"

namespace cdc
{
	class UTF8String
	{
	private:
		int m_len;
		cdc::Array<unsigned char> m_bytes;

		static UTF8String skm_empty;

		static const unsigned int skm_SpecialCharBeg  = 0x1800;
		static const unsigned int skm_SpecialCharEnd  = 0x18B0;
		static const unsigned int skm_UnprintableChar = 0x18AF;


	public:
		UTF8String();
		UTF8String(const char* zeroTermBytes);

		static unsigned int GetSpecialCharBeg();
		static unsigned int GetSpecialCharEnd();
		static unsigned int GetUnprintableChar();
		static bool IsSpecial(unsigned int c);
	};

	bool StrIsValidUTF8(const char* str, int* numCharsCounted);
}