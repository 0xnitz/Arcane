#include "ByteVectorUtils.hpp"
#include "WindowsException.hpp"

namespace ByteVectorUtils
{

ByteVector from_string(const std::string& string_to_convert)
{
	return { string_to_convert.begin(), string_to_convert.end() };
}

ByteVector from_wstring(const std::wstring& string_to_convert)
{
	ByteVector out_vector;

	static constexpr UINT UTF8 = CP_UTF8;
	static constexpr DWORD NO_FLAGS = 0;
	static constexpr LPSTR QUERY_SIZE = nullptr;
	static constexpr int QUERY_SIZE_LENGTH = 0;
	static constexpr LPCCH NO_DEFAULT_CHAR = nullptr;
	static constexpr LPBOOL USE_DEFAULT_CHAR = nullptr;
	const int required_size = WideCharToMultiByte(UTF8,
		NO_FLAGS,
		string_to_convert.data(),
		static_cast<int>(string_to_convert.size()),
		QUERY_SIZE,
		QUERY_SIZE_LENGTH,
		NO_DEFAULT_CHAR,
		USE_DEFAULT_CHAR);
	if (!required_size)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::WideCharToMultiByteFailed);
	}

	out_vector.resize(required_size);

	const int conversion_result = WideCharToMultiByte(UTF8,
		NO_FLAGS,
		string_to_convert.data(),
		static_cast<int>(string_to_convert.size()),
		reinterpret_cast<LPSTR>(out_vector.data()),
		required_size,
		NO_DEFAULT_CHAR,
		USE_DEFAULT_CHAR);
	if (!conversion_result)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::WideCharToMultiByteFailed);
	}

	return out_vector;
}

std::string to_string(const ByteVector& vector)
{
	return { vector.begin(), vector.end() };
}

std::wstring to_wstring(const ByteVector& vector)
{
	std::wstring out_string;

	static constexpr UINT UTF8 = CP_UTF8;
	static constexpr DWORD NO_FLAGS = 0;
	static constexpr LPWSTR QUERY_SIZE = nullptr;
	static constexpr int QUERY_SIZE_LENGTH = 0;
	const int required_size = MultiByteToWideChar(UTF8, 
		NO_FLAGS,
		reinterpret_cast<const char*>(vector.data()),
		static_cast<int>(vector.size()),
		QUERY_SIZE,
		QUERY_SIZE_LENGTH);
	if (!required_size)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::MultiByteToWideCharFailed);
	}

	out_string.resize(required_size);

	const int conversion_result = MultiByteToWideChar(UTF8,
		NO_FLAGS,
		reinterpret_cast<const char*>(vector.data()),
		static_cast<int>(vector.size()),
		out_string.data(),
		required_size);
	if (!conversion_result)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::MultiByteToWideCharFailed);
	}

	return out_string;
}

}
