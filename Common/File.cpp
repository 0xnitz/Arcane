#include "File.hpp"

File::File(const std::filesystem::path& file_path,
	const FileAccess file_access,
	const FileShare file_share,
	const FileCreationDisposition file_disposition) :
	m_handle(open_file(file_path, file_access, file_share, file_disposition)),
	m_offset({ .QuadPart = 0 })
{
}

ByteVector File::read(const size_t size_to_read)
{
	ByteVector out_bytes;
	out_bytes.resize(size_to_read);

	DWORD bytes_read;
	static constexpr LPOVERLAPPED NO_OVERLAP = nullptr;
	const BOOL read_file_result = ReadFile(m_handle.get(),
		out_bytes.data(),
		static_cast<DWORD>(size_to_read),
		&bytes_read,
		NO_OVERLAP);
	if (read_file_result == FALSE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::ReadFileFailed);
	}

	return out_bytes;
}

void File::write(const ByteVector& data)
{
	DWORD bytes_written;
	static constexpr LPOVERLAPPED NO_OVERLAP = nullptr;
	const BOOL write_file_result = WriteFile(m_handle.get(),
		data.data(),
		static_cast<DWORD>(data.size()),
		&bytes_written,
		NO_OVERLAP);
	if (write_file_result == FALSE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::WriteFileFailed);
	}
}

void File::seek(const int64_t offset)
{
	const LARGE_INTEGER distance_to_move = {
		.QuadPart = offset
	};

	static constexpr DWORD SEEK_FROM_START = FILE_BEGIN;
	const BOOL set_file_pointer_result = SetFilePointerEx(m_handle.get(),
		distance_to_move,
		&m_offset,
		SEEK_FROM_START);
	if (set_file_pointer_result == FALSE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::SetFilePointerExFailed);
	}
}

size_t File::size() const
{
	LARGE_INTEGER file_size;
	const BOOL get_file_size_result = GetFileSizeEx(m_handle.get(), &file_size);
	if (get_file_size_result == FALSE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::GetFileSizeExFailed);
	}

	return file_size.QuadPart;
}

SmartHandle File::open_file(const std::filesystem::path& file_path,
                            const FileAccess file_access,
                            const FileShare file_share,
                            const FileCreationDisposition file_disposition)
{
	static constexpr LPSECURITY_ATTRIBUTES NO_INHERIT = nullptr;
	static constexpr DWORD NORMAL_ATTRIBUTES = FILE_ATTRIBUTE_NORMAL;
	static constexpr HANDLE NO_TEMPLATE = nullptr;
	const HANDLE file_handle = CreateFileW(file_path.c_str(),
		file_access,
		file_share,
		NO_INHERIT,
		file_disposition,
		NORMAL_ATTRIBUTES,
		NO_TEMPLATE);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::CreateFileWFailed);
	}

	return SmartHandle(file_handle);
}
