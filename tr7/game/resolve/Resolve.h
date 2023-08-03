#pragma once

class ResolveObject
{
};

class Resolve
{
public:
	static void ChangeSpecialisation(unsigned int oldSpecMask, unsigned int newSpecMask);
	static ResolveObject* Load(const char* fileName, void(*callback)(void*, void*, void*, ResolveObject*), void* context1, void* context2, void* pointer);
};