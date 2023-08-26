#include "RenderRA.h"
#include "rnd_d3d.h"

void RenderG2_SetBGColor(float r, float g, float b)
{
}

void RenderG2_BeginScene(bool shadowEnabled)
{
	if (!D3D_InNextGenMode)
	{
		// TODO
	}

	D3D_BeginScene(shadowEnabled);
}

void RenderG2_EndScene(RenderG2_FrameRate rate)
{
	D3D_EndScene();
}