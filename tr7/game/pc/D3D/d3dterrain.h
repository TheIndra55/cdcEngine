#pragma once
#include "game/Stream/stream.h"

void D3DTERRAIN_Init();
void DRAW_PrepareAreaForDraw(Level* level);
void DRAW_DrawTerrainGroup(Terrain* terrain, TerrainGroup* bspTree);