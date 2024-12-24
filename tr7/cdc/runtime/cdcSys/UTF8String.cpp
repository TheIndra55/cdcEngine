#include "UTF8String.h"

cdc::UTF8String::UTF8String()
{
}

cdc::UTF8String::UTF8String(const char* zeroTermBytes)
{
}

unsigned int cdc::UTF8String::GetSpecialCharBeg()
{
	return skm_SpecialCharBeg;
}

unsigned int cdc::UTF8String::GetSpecialCharEnd()
{
	return skm_SpecialCharEnd;
}

unsigned int cdc::UTF8String::GetUnprintableChar()
{
	return skm_UnprintableChar;
}

bool cdc::UTF8String::IsSpecial(unsigned int c)
{
	return c >= skm_SpecialCharBeg && c < skm_SpecialCharEnd;
}

bool cdc::StrIsValidUTF8(const char* str, int* numCharsCounted)
{
	*numCharsCounted = 0;

	return true;
}