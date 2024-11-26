#pragma once

namespace cdc
{
	enum eReadWrite
	{
		READ,
		WRITE
	};

	class DebugFile
	{
	protected:
		char m_Filename[128];

	public:
		virtual bool Open(const char* fileName, eReadWrite readWrite) = 0;
		virtual void Close() = 0;
		virtual unsigned int Read(void* buffer, unsigned int size) = 0;
		virtual unsigned int Write(void* buffer, unsigned int size) = 0;
		virtual void Flush() = 0;
		virtual bool Exists(const char* fileName) = 0;
		virtual bool EndOfFile() = 0;
		virtual unsigned int GetSize() = 0;
		virtual bool IsOpen() = 0;
	};

	class DebugFileSystem
	{
	private:
		static DebugFileSystem* s_instance;

	public:
		DebugFile* GetFile();
		bool FileExists(const char* fileName);
		unsigned int GetFileSize(const char* fileName);

		static DebugFileSystem* Create();
		static void Destroy();

		friend DebugFileSystem* DebugFileSystemPtr();
	};

	DebugFileSystem* DebugFileSystemPtr();
}