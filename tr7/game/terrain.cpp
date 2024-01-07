#include "terrain.h"

#include "Stream/stream.h"
#include "Game/pc/D3D/d3dterrain.h"

void TERRAIN_CommonRenderLevel(StreamUnit* currentUnit)
{
	auto terrain = currentUnit->level->terrain;

	for (int i = 0; i < terrain->numTerrainGroups; i++)
	{
		DRAW_DrawTerrainGroup(terrain, &terrain->terrainGroups[i]);
	}
}

void TERRAIN_DrawUnits()
{
	for (int i = 0; i < MAX_STREAM_UNITS; i++)
	{
		auto streamUnit = &StreamTracker.StreamList[i];

		if (streamUnit->used == 2)
		{
			TERRAIN_CommonRenderLevel(streamUnit);
		}
	}
}