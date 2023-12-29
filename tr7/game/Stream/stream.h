#pragma once

#include "game/resolve/Resolve.h"
#include "game/Animation/anitracker.h"
#include "game/terrain.h"
#include "game/markup.h"
#include "game/event.h"

#include "cdc/runtime/cdcMath/Vector.h"
#include "cdc/runtime/cdcScene/Source/SceneCellGroup.h"
#include "cdc/runtime/cdcPlanner/AreaDBase.h"

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

struct Segment
{
};

struct MVertex
{
};

struct MFace
{
};

struct AnimatedListInfo
{
};

struct AnimatedInfo
{
};

struct ScrollInfo
{
};

struct TextureStripInfo
{
};

struct BoneMirrorData
{
};

struct SVector
{
};

struct ModelTarget
{
};

struct Model
{
	int version;
	int numSegments;
	int numVirtSegments;

	Segment* segmentList;
	cdc::Vector3 modelScale;
	int numVertices;
	MVertex* vertexList;
	int numNormals;
	cdc::Vector3* normalList;
	int numFaces;
	MFace* faceList;

	void* OBSOLETEaniTextures;

	float maxRad;
	float maxRadSq;

	void* OBSOLETEstartTextures;
	void* OBSOLETEendTextures;

	AnimatedListInfo* animatedListInfo;
	AnimatedInfo* animatedInfo;
	ScrollInfo* scrollInfo;

	TextureStripInfo* textureStripInfo;

	int* envMappedVertices;
	int* eyeRefEnvMappedVertices;

	unsigned int* materialVertexColors;
	unsigned int* spectralVertexColors;

	__int16* pnShadowFaces;
	__int16* pnShadowEdges;

	BoneMirrorData* boneMirrorData;
	SVector* drawgroupCenterList;

	int numMarkUps;
	MarkUp* markUpList;

	int numTargets;
	ModelTarget* targetList;

	unsigned int cdcRenderDataID;
	void* cdcRenderModelData;
};

static_assert(sizeof(Model) == 160, "Wrong Model size");

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

struct RECT_PSX
{
	__int16 x;
	__int16 y;
	__int16 w;
	__int16 h;
};

struct StreamUnitPortal
{
};

struct CameraKey
{
};

struct Intro
{
};

struct Signal
{
};

struct SplineCameraLevelData
{
};

struct SFXMkr
{
};

struct PlanData
{
};

struct AttackWaveList
{
};

struct AttackWaveGroupList
{
};

struct CombatDoorsList
{
};

struct TerrainLight
{
};

struct SpecialMarkerList
{
};

struct MoveData
{
};

namespace dtp
{
	struct UnitData
	{
	};

	struct ADMD
	{
	};
}

struct Level
{
	Terrain* terrain;

	float waterZLevel;

	char backColorR;
	char backColorG;
	char backColorB;
	char cpad1;

	char spectralColorR;
	char spectralColorG;
	char spectralColorB;
	char spectralFXAlways;

	char waterColorR;
	char waterColorG;
	char waterColorB;
	char waterBlend;

	float farPlane;
	float fogFar;
	float fogNear;

	float spectralFarPlane;
	float spectralFogFar;
	float spectralFogNear;

	float waterFarPlane;
	float waterFogFar;
	float waterFogNear;

	int UnderwaterFXAlpha;
	float UnderwaterFXMovement;
	float UnderwaterFXSpeed;

	int SpectralFXAlpha;
	float SpectralFXIncrease;
	float SpectralFXCenter;

	int WaterFXAlpha;
	float WaterFXMovement;
	float WaterFXSpeed;
	int WaterFSFX;

	int numMarkUps;
	MarkUp* markupList;

	int numCameras;
	CameraKey* cameraList;

	int flags;

	int numIntros;
	Intro* introList;

	unsigned __int16* objectNameList;

	char* worldName;

	Signal* startGoingIntoWaterSignal;
	Signal* startGoingOutOfWaterSignal;

	int unitFlags;

	Signal* SignalListStart;
	__int16* SignalIDList;

	SplineCameraLevelData* splineCameraData;

	void* relocModule;

	int NumberOfSFXMarkers;
	SFXMkr* SFXMarkerList;

	unsigned int versionNumber;
	unsigned int guiID;
	char* dynamicMusicName;

	int streamUnitID;
	struct VramLink* textureLoadList;

	PlanData* planData;

	unsigned int mapRegion;
	__int16** WeatherHeightmapData;

	AttackWaveList* attackWaveList;
	AttackWaveGroupList* attackWaveGroupList;

	CombatDoorsList* combatDoorsList;

	int numTerrainLights;
	TerrainLight* terrainLights;
	unsigned __int16** terrainLightGrids;

	SpecialMarkerList* vmarkerList;
	SpecialMarkerList* pmarkerList;

	char* playerName;
	unsigned int levelCount;

	MoveData* moveData;

	void* pCdcPlannerData;
	cdc::AreaDBase* pAreaDBase;
	dtp::UnitData* pUnitData;
	dtp::ADMD* pADMDData;

	cdc::Vector3 sceneCenterOffset;

	__int16 playerObjectID;
};

struct GameTracker;

struct StreamUnit
{
	int StreamUnitID;

	char used;
	char unitHidden;
	__int16 unitFlags;

	Level* level;
	char baseAreaName[20];

	ResolveObject* resolveObj;
	void* loadData;

	float fogNear;
	float fogFar;
	float farPlane;

	char fogR;
	char fogG;
	char fogB;
	char fogOverrideFlag;

	unsigned int FrameCount;

	RECT_PSX cliprect;

	int draw;
	int drawtime;

	void(__cdecl* EventMain)(GameTracker* gameTracker, StreamUnit* unit, GlobalData* globalData);
	void(__cdecl* CallTrigger)(int trigger);
	unsigned int(__cdecl* IsTriggerActive)(int trigger);

	cdc::ISceneCellGroup* pCellGroup;
};

#define MAX_OBJECTS 94
#define MAX_STREAM_UNITS 8

struct STracker
{
	StreamUnit StreamList[MAX_STREAM_UNITS];
};

extern ObjectTracker GlobalObjects[MAX_OBJECTS];
extern STracker StreamTracker;

void STREAM_Init();
ObjectTracker* STREAM_GetObjectTrackerByName(char* name);

int InsertGlobalObject(int id);

StreamUnit* STREAM_LoadLevel(char* baseAreaName, StreamUnitPortal* streamPortal, bool loadObjects);