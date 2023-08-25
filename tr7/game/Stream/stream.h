#pragma once

#include "game/resolve/Resolve.h"

struct StreamUnitObjectList
{
	__int16 numObjects;
	unsigned __int16 objectList[1];
};

struct UnitInfo
{
	char name[16];
	StreamUnitObjectList* objectList;
};

struct StreamUnitList
{
	int numUnits;
	UnitInfo unit[1];
};

struct ObjectDTPData
{
	struct ObjectHeader
	{
	};

	unsigned int OEDTemplateSelector;

	void* m_pAnimBlends;
	void* m_pAnimPatterns;

	void* TuneDataPtr;

	unsigned int m_numAnimBlends;
	unsigned int m_numAnimPatterns;

	ObjectDTPData::ObjectHeader Header;
};

struct Model
{
};

struct AnimListEntry
{
};

struct AnimFxHeader
{
};

struct AnimScriptObject
{
};

struct ObjectBase
{
};

struct SFXData
{
};

struct ObjectEffectFXA
{
};

struct GenericFXObject
{
};

struct ClothSetup
{
};

struct VramLink
{
};

struct Object
{
	int oflags;
	int oflags2;
	int uniqueID;
	unsigned int guiID;
	int functionTableID;
	void* obsoleteSoundBank;

	__int16 numModels;
	__int16 numAnims;
	__int16 numAnimPatterns;

	Model** modelList;
	AnimListEntry* animList;
	AnimFxHeader** animFXList;
	AnimScriptObject** animPatternList;

	int introDist;
	int vvIntroDist;
	int removeDist;
	int vvRemoveDist;

	ObjectBase* baseData;
	void* data;
	char* name;

	SFXData* soundData;

	__int16 sectionA;
	__int16 sectionB;
	__int16 sectionC;
	__int16 numberOfEffects;

	ObjectEffectFXA* effectList;
	GenericFXObject* effectData;

	ObjectDTPData* objectDTPData;

	VramLink* textureLoadList;
	unsigned __int16* childObjectList;

	int lod1Dist;
	int lod2Dist;
	char lod1Model;
	char lod2Model;
	char shadowModel;
	char lightingOverride;
	float maxCheckeeDistance;
	ClothSetup** rdSetupList;
};

static_assert(sizeof(Object) == 128, "Wrong Object size");

struct ObjectTracker
{
	ResolveObject* resolveObj;
	char* objectName;
	Object* object;
	__int16 objectID;
	__int16 objectStatus;
	__int16 numInUse;
	char numObjectsUsing;
	char objectsUsing[13];
	void* vramBlock;
};

void STREAM_Init();
ObjectTracker* STREAM_GetObjectTrackerByName(char* name);

int InsertGlobalObject(int id);