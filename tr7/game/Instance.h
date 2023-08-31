#pragma once

#include "game/List.h"
#include "game/Stream/stream.h"

struct Instance;

struct BaseInstance
{
	NodeType node;
	Instance* next;
	Instance* prev;

	Object* object;
	char currentRenderModel;
};

struct Instance : BaseInstance
{
};

void INSTANCE_InitInstanceList();
void INSTANCE_ClearAllSoundStreamHolders();