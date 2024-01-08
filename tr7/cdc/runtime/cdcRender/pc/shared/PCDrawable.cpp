#include "PCDrawable.h"

void cdc::PCDrawable::SetSortZ(float sortZ)
{
	m_sortZ = sortZ;
}

void cdc::PCDrawable::SetScene(cdc::PCScene* pScene)
{
	m_pScene = pScene;
}

cdc::PCDrawableList::PCDrawableList()
{
	m_pHead = nullptr;
	m_size = 0;
}

void cdc::PCDrawableList::Add(PCDrawable* drawable)
{
	auto node = new Node();
	node->pElem = drawable;
	node->pNext = m_pHead;

	m_pHead = node;
}

void cdc::PCDrawableList::Draw(PCPass pass, PCScene* pCurScene)
{
	for (Node* node = m_pHead; node != nullptr; node = node->pNext)
	{
		node->pElem->Draw(pass, nullptr);
	}
}

void cdc::PCDrawableList::Clear()
{
	m_size = 0;
	m_pHead = nullptr;
}