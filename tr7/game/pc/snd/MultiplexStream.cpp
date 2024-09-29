#include "MultiplexStream.h"

#include <cdc/runtime/cdcSound/win32/snd.h>

void MultiplexStream::StopAllStreams()
{
}

bool MultiplexStream::InitSystem()
{
	cdc::ADPCM_CalcTable();

	return true;
}

void MultiplexStream::SetSoundDirectory(const char* dir)
{
}

void MultiplexStream::SetCinematicDirectory(const char* dir)
{
}