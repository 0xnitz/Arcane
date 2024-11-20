#include "WindowsException.hpp"

#include <windows.h>

WindowsException::WindowsException(const ArcaneErrors::ErrorCodes error_code) :
	Exception(error_code),
	m_windows_error(GetLastError())
{
}

uint32_t WindowsException::get_windows_error() const
{
	return m_windows_error;
}

std::wstring WindowsException::message() const
{
	return L"WindowsException with Arcane Error -> " + std::to_wstring(m_error_code) +
		L" Windows Error -> " + std::to_wstring(m_windows_error) + L"!";
}
