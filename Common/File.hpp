#pragma once

#include "DefinesMacros.hpp"
#include "SmartHandleBase.hpp"

#include <filesystem>

enum FileAccess : uint32_t
{
	GenericRead = GENERIC_READ,
	GenericWrite = GENERIC_WRITE,
	ReadWrite = GenericRead | GenericWrite
};

enum FileShare : uint32_t
{
	None = 0,
	ShareRead = FILE_SHARE_READ,
	ShareWrite = FILE_SHARE_WRITE,
	ShareDelete = FILE_SHARE_DELETE,
	All = ShareRead | ShareWrite | ShareDelete
};

enum FileCreationDisposition : uint32_t
{
	CreateNew = CREATE_NEW,
	CreateAlways = CREATE_ALWAYS,
	OpenExisting = OPEN_EXISTING,
	OpenAlways = OPEN_ALWAYS,
	TruncateExisting = TRUNCATE_EXISTING
};

class File final
{
public:
	explicit File(const std::filesystem::path& file_path,
		FileAccess file_access,
		FileShare file_share,
		FileCreationDisposition file_disposition);

	File(File const&) = delete;
	File(File&&) = delete;
	File operator=(File const&) = delete;
	File operator=(File&&) = delete;

	NO_DISCARD ByteVector read(uint32_t size_to_read);

	void write(const ByteVector& data);

	void seek(int64_t offset);

	NO_DISCARD size_t size() const;

private:
	NO_DISCARD static SmartHandle open_file(const std::filesystem::path& file_path,
	                                        FileAccess file_access,
	                                        FileShare file_share,
	                                        FileCreationDisposition file_disposition);

	SmartHandle m_handle; // File handle.

	LARGE_INTEGER m_offset; // Offset in file.
};