#include <stdio.h>

#include "stream.h"

#include "cdc/runtime/cdcSys/Assert.h"

#include "game/OBTable.h"
#include "game/Main.h"
#include "game/pc/D3D/d3dinstance.h"

StreamUnitList* gUnitList = nullptr;

ObjectTracker GlobalObjects[MAX_OBJECTS];

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

ObjectTracker* STREAM_GetObjectTrackerByName(char* name)
{
	auto id = OBTABLE_GetObjectID(name);

	if (!id)
	{
		cdc::FatalError("Unable to get ID for object: %s", name);
	}

	auto object = InsertGlobalObject(id);

	if (object == -1)
	{
		return nullptr;
	}

	return &GlobalObjects[object];
}

void STREAM_LoadObjectReturn(void* loadData, void* data, void* data2, ResolveObject* resolveObject)
{
	auto object = (Object*)loadData;
	auto tracker = (ObjectTracker*)data;

	auto dtpData = object->objectDTPData;
	if (dtpData && dtpData->OEDTemplateSelector)
	{
		object->data = dtpData->TuneDataPtr;
	}

	tracker->objectStatus = 2;

	for (int i = 0; i < object->numModels; i++)
	{
		if (!object->modelList[i])
		{
			cdc::FatalError("FATAL ERROR: object '%s' is missing model '%d' (out of %d models)!", tracker->objectName, i, object->numModels);
			return;
		}

		auto version = object->modelList[i]->version;

		if (version != 0x4C20453)
		{
			cdc::FatalError("FATAL ERROR: model version (%X) for object '%s' does not match the game version (%X)!", version, tracker->objectName, 0x4C20453);
			return;
		}
	}

	if (object->lod1Model == -1)
	{
		object->oflags &= 0xFFDFFFFF;
	}
	else
	{
		object->oflags |= 0x200000;
	}

	OBTABLE_InitObjectWithID(object);

	if (object->animList)
	{
		ANITRACKER_TranslateAnimationList(object->animList);
	}

	OBTABLE_InitAnimPointers(tracker);

	DRAW_PrepareObjectForDraw(object);
}

int InsertGlobalObject(int id)
{
	auto name = gObjectList->object[id - 1].name;

	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		auto object = &GlobalObjects[i];

		if (object->objectStatus && object->objectStatus != 5 && GlobalObjects->objectID == id)
		{
			return i;
		}
	}

	ObjectTracker* object;
	int i;
	for (i = 0; i < MAX_OBJECTS; i++)
	{
		object = &GlobalObjects[i];

		if (!object->objectStatus)
		{
			break;
		}
	}

	char string[256];
	LOAD_ObjectFileName(string, name, "drm");

	object->objectName = name;
	object->objectStatus = 1;
	object->objectID = id;
	object->resolveObj = Resolve::Load(string, STREAM_LoadObjectReturn, object, nullptr, (void**)&object->object);
	object->numInUse = 0;
	object->numObjectsUsing = 0;

	return i;
}