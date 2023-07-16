#include <stdlib.h>

#include "Main.h"

#include "cdc/runtime/cdcFile/FileSystem.h"
#include "cdc/runtime/cdcFile/MS/MSFileSystem.h"
#include "cdc/runtime/cdcFile/FileReceivers.h"

cdc::FileSystem* g_pDiskFS;
cdc::FileSystem* g_pFS;

void InitFS()
{
	auto fileSystem = new cdc::MSFileSystem("");

	g_pDiskFS = fileSystem;
	g_pFS = fileSystem;
}

char* FSHelper_ReadFile(const char* fileName, char memType, cdc::FileSystem* pFS)
{
	auto size = pFS->GetFileSize(fileName);

	if (!size)
	{
		return 0;
	}

	auto buffer = (char*)malloc(size + 1);

	if (!buffer)
	{
		return 0;
	}

	auto receiver = new cdc::FileUserBufferReceiver();
	receiver->m_pBuffer = buffer;

	auto request = pFS->RequestRead(receiver, fileName, 0);

	if (request)
	{
		request->Submit(cdc::FileRequest::HIGH);
		pFS->Synchronize();

		buffer[size] = 0;

		return buffer;
	}
	else
	{
		return 0;
	}
}