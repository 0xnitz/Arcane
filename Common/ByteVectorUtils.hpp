#pragma once

#include "DefinesMacros.hpp"

#include <string>

namespace ByteVectorUtils
{

ByteVector from_string(const std::string& string_to_convert);

ByteVector from_wstring(const std::wstring& string_to_convert);

std::string to_string(const ByteVector& vector);

std::wstring to_wstring(const ByteVector& vector);

}