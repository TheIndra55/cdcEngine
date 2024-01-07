#pragma once

#include "bgobject.h"

#include "cdc/runtime/cdcMath/Vector.h"
#include "cdc/runtime/cdcMath/Matrix.h"

struct Intro;
struct StreamUnitPortal;
struct Signal;
struct Instance;
struct ScrollInfo;
struct AnimatedInfo;
struct Level;

struct Mesh
{
};

struct BoundingSphere
{
	float x;
	float y;
	float z;
	float r;
};

struct TerrainTextureStripInfo;

struct OctreeSphere
{
	BoundingSphere boundsphere;
	TerrainTextureStripInfo* strip;

	int numSpheres;
	OctreeSphere* spheres[8];
};

struct SKUVertex
{
	__int16 x;
	__int16 y;
	__int16 z;
};

struct TerrainTextureStripInfo
{
	int vertexCount;
	int vmoObjectIndex;

	float texScrollOffset;

	unsigned int lightFlags;
	int stripFlags;

	int matIdx;

	SKUVertex sortVertexMaterial;
	SKUVertex sortVertexSpectral;

	int sortPush;

	TerrainTextureStripInfo* nextTexture;
	__int16 stripVertex[1];
};

struct XboxPcMaterialStripList
{
	unsigned int tpageid;
	unsigned int flags;
	unsigned int vbBaseOffset;
	unsigned int numStrips;

	TerrainTextureStripInfo** strips;
};

struct XboxPcMaterialList
{
	int numMaterials;
	XboxPcMaterialStripList materials[1];
};

struct TerrainGroup
{
	cdc::Vector3 globalOffset;
	cdc::Vector3 localOffset;

	int flags;
	int ID;
	int uniqueID;
	int splineID;

	Instance* instanceSpline;
	Level* level;
	Mesh* mesh;

	float textureMorphValue;
	float textureMorphStep;

	OctreeSphere* octreeSphere;
	ScrollInfo* octreeScrollInfo;
	AnimatedInfo* octreeAnimatedInfo;

	cdc::Matrix matrix;

	XboxPcMaterialList* XboxPcMaterialList;
	__int16* sortedMaterials;

	unsigned int cdcRenderDataID;
	void* cdcRenderTerrainData;

	float groupOriginX;
	float groupOriginY;
	float groupOriginZ;
};

struct TerrainAnimTexture
{
};

struct VMObject
{
};

struct VMOffsetTable
{
};

struct TerrainRenderVertex
{
	__int16 x;
	__int16 y;
	__int16 z;
	__int16 w;

	int color;

	__int16 u;
	__int16 v;

	unsigned __int16 bend;
	unsigned __int16 bendindex;
};

struct TerrainVMORenderVertex
{
	__int16 x;
	__int16 y;
	__int16 z;
	__int16 w;

	int color;

	__int16 u;
	__int16 v;

	unsigned __int16 vmoBase;
	unsigned __int16 vmoFrac;

	__int16 vx;
	__int16 vy;
	__int16 vz;
	__int16 vw;

	int vcolor;

	__int16 vu;
	__int16 vv;
};

struct Terrain
{
	__int16 UnitChangeFlags;
	__int16 spad;

	int numIntros;
	Intro* introList;

	int numStreamUnitPortals;
	StreamUnitPortal* streamUnitPortals;

	int numTerrainGroups;
	TerrainGroup* terrainGroups;
	TerrainGroup* signalTerrainGroup;

	Signal* signals;
	TerrainAnimTexture* terrainAnimTextures;

	int numBGInstances;
	BGInstance* BGInstanceList;

	int numBGObjects;
	BGObject* BGObjectList;

	int numVMObjects;
	VMObject* vmobjectList;
	VMOffsetTable* vmTableList;

	TerrainRenderVertex* XboxPcVertexBuffer;
	TerrainVMORenderVertex* XboxPcVmoBuffer;
	void* XboxPcPortalAdjustVerts;
	void* d3dvertexBuffer;

	unsigned int numTerrainVertices;
	void* d3dvmoVertexBuffer;
	unsigned int numTerrainVMOVertices;

	unsigned int cdcRenderDataID;
	void* cdcRenderTerrainData;
};

void TERRAIN_DrawUnits();