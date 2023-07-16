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
	public:
		virtual int ReceiveData(const char* data, unsigned int dataSize, unsigned int requestOffset) = 0;
		virtual void ReceiveStarted(FileRequest* request, unsigned int requestSize) = 0;
		virtual void ReceiveCancelled(FileRequest* request) = 0;
		virtual void ReceiveDone(FileRequest* request) = 0;
	};

	class File
	{
	public:
		virtual FileRequest* RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset) = 0;
		virtual unsigned int GetSize() = 0;
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
		virtual File* OpenFile(const char* fileName) = 0;
		virtual bool FileExists(const char* fileName) = 0;
		virtual unsigned int GetFileSize(const char* fileName) = 0;
		virtual void SetSpecialisationMask(unsigned int specMask) = 0;
		virtual unsigned int GetSpecialisationMask() = 0;
		virtual Status GetStatus() = 0;
		virtual void Update() = 0;
		virtual void Synchronize() = 0;
	};
}