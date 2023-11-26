#include <string.h>

#include "UIItemImage.h"

UIImageManager::UIImageRefCount UIImageManager::UIImages[16];
int UIImageManager::imageCount;

void UIImageManager::Init()
{
	memset(UIImages, 0, sizeof(UIImages));
	imageCount = 0;
}