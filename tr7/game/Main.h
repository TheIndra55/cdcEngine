#pragma once

#include "cdc/runtime/cdcFile/FileSystem.h"

extern char ActiveBuildName[16];
extern char ActiveBuildDir[32];

cdc::FileSystem* GetFS();

void InitFS();
void InitArchive();

void SetupBuildDir(const char* configName);

char* FSHelper_ReadFile(const char* fileName, char memType, cdc::FileSystem* pFS);