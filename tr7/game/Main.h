#pragma once

#include "cdc/runtime/cdcFile/FileSystem.h"

void InitFS();
void InitArchive();

char* FSHelper_ReadFile(const char* fileName, char memType, cdc::FileSystem* pFS);