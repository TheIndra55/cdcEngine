#pragma once

#include "cdc/runtime/cdcFile/FileSystem.h"
#include "MSFileSources.h"

namespace cdc
{
	enum ReadState
	{
		READ_STATE_INIT,
		READ_STATE_READ,
		READ_STATE_READING,
		READ_STATE_PROCESSING,
		READ_STATE_PAUSED,
		READ_STATE_EXIT,
		READ_STATE_DONE
	};

	class MSFileSystem : public cdc::FileSystem
	{
	public:
		class Request : public FileRequest
		{
		public:
			cdc::MSFileSystem* m_FileSystem;

			bool m_IsInitialised;

			int m_FileHandle;
			unsigned int m_SkipSize;
			char m_pFileName[128];

			Priority m_Priority;
			bool m_CloseFile;
			bool m_IsCancelled;

			cdc::FileReceiver* m_pReceiver;
			Status m_Status;

			int m_Ref;

			unsigned int m_BytesRead;
			unsigned int m_BytesProcessed;
			cdc::ReadState m_ReadState;
			unsigned int m_Offset;
			unsigned int m_Size;

			Request* m_Next;

			void Init(FileReceiver* receiver, const char* fileName, unsigned int fileHandle, unsigned int startOffset);

			void AddRef();
			void Release();
			void SetCompressedSize(unsigned int compressedSize);
			void SetSize(unsigned int size);
			Status GetStatus();
			void Submit(Priority priority);
			void Cancel();
			float Completed();
		};

		class File : public cdc::File
		{
		private:
			MSFileSystem* m_FileSystem;

			int m_FileHandle;
			char m_pFileName[128];

			MSFileSystem::Request* m_pRequest;

		public:
			File(const char* fileName, MSFileSystem* fileSystem);

			FileRequest* RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset);
			unsigned int GetSize();
		};

	private:
		MSFileSource* m_FileSource;

		char m_Buffer[0x100800];
		int m_BytesInBuffer;

		unsigned int m_BufferOffset;
		unsigned int m_FileOffset;
		unsigned int m_PausedBytesInBuffer;
		unsigned int m_PausedBufferOffset;
		unsigned int m_PausedFileOffset;

		cdc::ReadState m_PausedReadState;

		Request m_Requests[256];
		Request* m_Queue;
		Request* m_Free;
		unsigned int m_numUsedRequests;

	public:
		MSFileSystem(const char* basePath);

		void SetupRequests();
		void PutInQueue(Request* request, cdc::FileRequest::Priority priority);
		void RemoveFromQueue(Request* request);
		unsigned int RoundToSectors(unsigned int size);
		bool ProcessBuffer(char* buffer, Request* request, bool isReading);
		Request* PopFreeRequest();

		FileRequest* RequestRead(FileReceiver* receiver, const char* fileName, unsigned int startOffset);
		cdc::File* OpenFile(const char* fileName);
		bool FileExists(const char* fileName);
		unsigned int GetFileSize(const char* fileName);
		void SetSpecialisationMask(unsigned int specMask);
		unsigned int GetSpecialisationMask();
		Status GetStatus();
		void Update();
		void Synchronize();
	};
}