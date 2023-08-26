#include "d3dinstance.h"
#include "rnd_d3d.h"

#include "game/Stream/stream.h"
#include "game/pc/cdcRenderLayer/RenderLayer.h"

void D3DINSTANCE_Init()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (GlobalObjects[i].objectStatus == 2 && D3D_InNextGenMode)
		{
			cdcRenderLayer::DRAW_PrepareObjectForDraw(GlobalObjects[i].object);
		}
	}
}