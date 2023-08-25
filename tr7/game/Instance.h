#pragma once

struct BaseInstance
{
};

struct Instance : BaseInstance
{
};

void INSTANCE_InitInstanceList();
void INSTANCE_ClearAllSoundStreamHolders();