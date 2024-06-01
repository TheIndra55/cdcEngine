#include "d3dinstance.h"
#include "rnd_d3d.h"

#include "game/Stream/stream.h"
#include "game/pc/cdcRenderLayer/RenderLayer.h"

void D3DINSTANCE_Init()
{
	if (D3D_InNextGenMode)
	{
		for (int i = 0; i < MAX_OBJECTS; i++)
		{
			if (GlobalObjects[i].objectStatus == 2)
			{
				cdcRenderLayer::DRAW_PrepareObjectForDraw(GlobalObjects[i].object);
			}
		}
	}
	else
	{
		/* Legacy handler */
	}
}

void DRAW_PrepareObjectForDraw(Object* object)
{
	if (D3D_InNextGenMode)
	{
		cdcRenderLayer::DRAW_PrepareObjectForDraw(object);
	}
}
