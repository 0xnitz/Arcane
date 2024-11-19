#pragma once

#include "Exception.hpp"

class WindowsException final : public Exception
{
public:
	explicit WindowsException(ErrorCodes error_code);

	NO_DISCARD uint32_t get_windows_error() const;

	NO_DISCARD std::wstring message() const override;

private:
	uint32_t m_windows_error;
};