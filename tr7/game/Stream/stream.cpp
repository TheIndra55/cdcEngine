#include <stdio.h>

#include "stream.h"

#include "game/OBTable.h"
#include "game/Main.h"

StreamUnitList* gUnitList = nullptr;

void STREAM_Init()
{
	if (!gUnitList)
	{
		OBTABLE_Init();

		auto fileSystem = GetFS();

		char objListPath[300];
		sprintf(objListPath, "\\TR7\\%s\\objlist.dat", ActiveBuildName);

		if (fileSystem->GetFileSize(objListPath) > 0)
		{
			gUnitList = (StreamUnitList*)FSHelper_ReadFile(objListPath, 0, fileSystem);
			auto objectList = (StreamUnitObjectList*)&gUnitList->unit[gUnitList->numUnits + 1];

			for (int i = 0; i < gUnitList->numUnits; i++)
			{
				auto unit = &gUnitList->unit[i];

				if (*unit->name)
				{
					unit->objectList = objectList;

					objectList++;
				}
			}
		}
		else
		{
			gUnitList = new StreamUnitList();
			gUnitList->numUnits = 0;
		}
	}
}