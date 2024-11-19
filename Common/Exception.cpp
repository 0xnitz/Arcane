#include "Exception.hpp"

Exception::Exception(const ErrorCodes error_code)
	: m_error_code(error_code)
{
}

ErrorCodes Exception::get_error_code() const
{
	return m_error_code;
}

std::wstring Exception::message() const
{
	return L"Arcane Exception with error code: " + std::to_wstring(m_error_code) + L"!";
}
