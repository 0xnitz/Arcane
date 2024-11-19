#pragma once

#include "DefinesMacros.hpp"

#include <string>

namespace ArcaneErrors
{
	enum ErrorCodes
	{
		NotImplemented = 0
	};
}

using namespace ArcaneErrors;

class Exception
{
public:
	explicit Exception(ErrorCodes error_code);

	virtual ~Exception() = default;

	NO_DISCARD ErrorCodes get_error_code() const;

	NO_DISCARD virtual std::wstring message() const;

protected:
	ErrorCodes m_error_code; // Exception error codes.
};

#define CATCH_ALL() \
	catch (const Exception& exception) { DEBUG_PRINT_W(exception.message().c_str()) } \
	catch (const std::exception& exception) { DEBUG_PRINT(exception.what()); } \
	catch (...) { DEBUG_PRINT("Unknown exception caught!"); }
