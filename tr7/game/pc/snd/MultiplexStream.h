#pragma once

class MultiplexStream
{
public:
	static void StopAllStreams();
	static void SetSoundDirectory(const char* dir);
	static void SetCinematicDirectory(const char* dir);
};