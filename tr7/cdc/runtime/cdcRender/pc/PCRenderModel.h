#pragma once

#include "PCShaderLib.h"
#include "PCTexture.h"
#include "PCRenderDevice.h"

#include "shared/PCStaticVertexBuffer.h"
#include "shared/PCStaticIndexBuffer.h"

#include "cdc/runtime/cdcRender/RenderResource.h"
#include "cdc/runtime/cdcMath/Vector.h"
#include "cdc/runtime/cdcMath/Matrix.h"

namespace cdc
{
	struct PCModelData
	{
		enum PCRenderModelFlags
		{
			kSkySphere = 1 << 0,
			kZFacade = 1 << 1,
			kDFacade = 1 << 2,
			kDynamicLighting = 1 << 3,
			kDistanceLight = 1 << 4,
			kStencilShadow = 1 << 5,
		};

		enum Type : int
		{
			kTypeRigid,
			kTypeSkinned,
			kTypeVmoAll,
			kTypeVmoXYZ,
			kTypeVmoRGBA,
			kTypeVmoUV,
			kTypeBend,
			kTypeDummy = 123456,
		};

		unsigned int magicNumber;

		unsigned int flags;
		unsigned int totalDataSize;
		unsigned int numIndices;

		cdc::Vector3 boundingSphereCenter;
		cdc::Vector3 boxMin;
		cdc::Vector3 boxMax;
		float boundingSphereRadius;

		Type modelType;

		float sortBias;

		unsigned int primGroupOffset;
		unsigned int modelBatchOffset;
		unsigned int boneOffset;
		unsigned int materialOffset;
		unsigned int indexOffset;
		unsigned int stencilDataOffset;

		unsigned __int16 numPrimGroups;
		unsigned __int16 numModelBatches;
		unsigned __int16 numBones;
		unsigned __int16 numMaterials;
		unsigned __int16 numPixmaps;
		unsigned __int16 highestBendIndex;
	};

	static_assert(sizeof(PCModelData) == 112, "Wrong PCModelData size");

	struct PCPrimGroup
	{
		unsigned int baseIndex;
		unsigned int numPrimitives;
		unsigned int numVertices;
		unsigned __int16 vertexShaderFlags;
		unsigned int materialIndex;
	};

	static_assert(sizeof(PCPrimGroup) == 20, "Wrong PCPrimGroup size");

	struct PCModelBatch
	{
		unsigned int flags;
		unsigned int numPrimGroups;
		unsigned __int16 skinMapSize;

		union
		{
			unsigned int* pSkinMap;
			unsigned int skinMapOffset;
		};

		union
		{
			void* pVertexData;
			unsigned int vertexDataOffset;
		};

		cdc::PCStaticVertexBuffer* pVertexBuffer;
		D3DVERTEXELEMENT9 vertexElements[16];

		unsigned int vertexFormat;
		unsigned int vertexStride;
		unsigned int numVertices;
		unsigned int baseIndex;
		unsigned int numPrimitives;
	};

	static_assert(sizeof(PCModelBatch) == 172, "Wrong PCModelBatch size");

	class RenderMesh : public RenderResource
	{
	public:
		void Release();
		int GetMemorySize();
	};

	class PCRenderModel : public RenderMesh
	{
	private:
		unsigned __int16 m_numPrimGroups;
		PCPrimGroup* m_pPrimGroups;

		unsigned __int16 m_numModelBatches;
		PCModelBatch* m_pModelBatches;

		unsigned int* m_pBones;

		cdc::PCTexture** m_pTextures;

		unsigned __int16* m_pIndexData;
		cdc::PCStaticIndexBuffer* m_pIndexBuffer;

		void BuildStencilData();

	public:
		PCModelData* m_pHeader;

		PCRenderModel(PCModelData* pRenderMeshData, TextureMap** pTextures, IShaderLib** pShaders, RenderMesh* pRefMesh, PCRenderDevice* pRenderDevice);
	};

	class RenderModelInstance : public RenderResource
	{
	public:
		void Release();
		int GetMemorySize();

		virtual void Draw(cdc::Matrix* pRootMatrix) = 0;
	};

	class PCRenderModelInstance : public RenderModelInstance
	{
	private:
		cdc::PCRenderModel* m_pRenderModel;

		cdc::Vector3 m_center;
		float m_radius;

	public:
		PCRenderModelInstance(PCRenderModel* pRenderModel, PCRenderDevice* pRenderDevice);

		void Draw(cdc::Matrix* pRootMatrix);
	};
}