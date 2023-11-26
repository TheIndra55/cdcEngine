#pragma once

class UIImageManager
{
private:
	struct UIImageRefCount
	{
		const char* filename;
		int refCount;
		void* image;
	};

	static UIImageRefCount UIImages[16];
	static int imageCount;

public:
	static void Init();
};