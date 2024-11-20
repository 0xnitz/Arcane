#include "Exception.hpp"

Exception::Exception(const ArcaneErrors::ErrorCodes error_code)
	: m_error_code(error_code)
{
}

ArcaneErrors::ErrorCodes Exception::get_error_code() const
{
	return m_error_code;
}

std::wstring Exception::message() const
{
	return L"Arcane Exception with error code: " + std::to_wstring(m_error_code) + L"!";
}
