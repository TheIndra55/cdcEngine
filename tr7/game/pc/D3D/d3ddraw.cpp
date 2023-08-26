#include "d3ddraw.h"
#include "d3dinstance.h"
#include "d3dterrain.h"

void D3DDRAW_Init()
{
	D3DINSTANCE_Init();
	D3DTERRAIN_Init();
}