#pragma once

#include <stdbool.h>

namespace cdc
{
	class FileRequest
	{
	public:
		enum Status
		{
			SETUP,
			QUEUED,
			PROCESSING,
			DONE,
			CANCELLED
		};

		enum Priority
		{
			HIGH,
			NORMAL,
			LOW
		};

		virtual void AddRef() = 0;
		virtual void SetCompressedSize(unsigned int compressedSize) = 0;
		virtual void SetSize(unsigned int size) = 0;
		virtual Status GetStatus() = 0;
		virtual void Submit(Priority priority) = 0;
		virtual void Cancel() = 0;
		virtual float Completed() = 0;
	};

	class FileReceiver
	{
	};

	class FileSystem
	{
	public:
		enum Status
		{
			IDLE,
			BUSY
		};

		virtual FileRequest* RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset) = 0;
		virtual bool FileExists(const char* fileName) = 0;
		virtual unsigned int GetFileSize(const char* fileName) = 0;
		virtual void SetSpecialisationMask(unsigned int specMask) = 0;
		virtual unsigned int GetSpecialisationMask() = 0;
		virtual Status GetStatus() = 0;
		virtual void Update() = 0;
		virtual void Synchronize() = 0;
	};
}