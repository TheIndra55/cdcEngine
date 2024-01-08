#pragma once

#include "PCScene.h"

#include "cdc/runtime/cdcMath/Matrix.h"

namespace cdc
{
	class IDrawable
	{
	public:
		virtual void Draw(cdc::Matrix* matrix, bool isMoved) = 0;
	};

	enum PCPass
	{
		PC_PASS_DEPTH          = 1 << 0,
		PC_PASS_DECAL          = 1 << 1,
		PC_PASS_SHADOW         = 1 << 2,
		PC_PASS_TRANSLUCENT    = 1 << 3,
		PC_PASS_OPAQUE         = 1 << 4,
		PC_PASS_PROJECTED      = 1 << 5,
		PC_PASS_AMBIENT        = 1 << 6,
		PC_PASS_MULTIPASSLIGHT = 1 << 7,
		PC_PASS_COMPOSITE      = 1 << 8,
		PC_PASS_PREDATOR       = 1 << 9,
		PC_PASS_HIGHLIGHT      = 1 << 10,
		PC_PASS_WATER          = 1 << 11,
		PC_PASS_DEPTHTOCOLOR   = 1 << 12,

		PC_PASS_COUNT          = 13,
	};

	class PCDrawable
	{
	public:
		virtual void Draw(cdc::PCPass pass, cdc::PCDrawable* pPrevious) = 0;
		virtual bool Compare(cdc::PCPass pass, cdc::PCDrawable* pOther) = 0;
		virtual unsigned int GetPasses() = 0;
		virtual unsigned int GetPolyFlags() = 0;

	protected:
		void SetSortZ(float sortZ);
		void SetScene(cdc::PCScene* pScene);

	private:
		cdc::PCScene* m_pScene;
		float m_sortZ;
	};

	class PCDrawableQueue
	{
	public:
		virtual void Add(PCDrawable* drawable) = 0;
	};

	class PCDrawableList : public PCDrawableQueue
	{
	private:
		struct Node
		{
			Node* pNext;
			PCDrawable* pElem;
		};

		Node* m_pHead;
		unsigned int m_size;

	public:
		PCDrawableList();

		void Draw(PCPass pass, PCScene* pCurScene);
		void Add(PCDrawable* drawable);
		void Clear();
	};
}