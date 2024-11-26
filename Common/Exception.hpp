#pragma once

#include "DefinesMacros.hpp"

#include <string>

namespace ArcaneErrors
{
	enum ErrorCodes
	{
		NotImplemented = 0,
		OpenSCManagerFailed,
		CloseServiceHandleFailed,
		CreateServiceFailed,
		DeleteServiceFailed,
		StartServiceFailed,
		ControlServiceFailed,
		CloseHandleCloseServiceHandleFailed,
		CreateFileWFailed,
		SetFilePointerExFailed,
		ReadFileFailed,
		GetFileSizeExFailed,
		WriteFileFailed,
		MultiByteToWideCharFailed,
		WideCharToMultiByteFailed,
	};
}

class Exception
{
public:
	explicit Exception(ArcaneErrors::ErrorCodes error_code);

	virtual ~Exception() = default;

	NO_DISCARD ArcaneErrors::ErrorCodes get_error_code() const;

	NO_DISCARD virtual std::wstring message() const;

protected:
	ArcaneErrors::ErrorCodes m_error_code; // Exception error codes.
};

#define CATCH_ALL(error_message) \
	catch (const Exception& exception) { DEBUG_PRINT_W(exception.message()) } \
	catch (const std::exception& exception) { DEBUG_PRINT(std::string(exception.what())); } \
	catch (...) { DEBUG_PRINT(std::string(error_message)); }
