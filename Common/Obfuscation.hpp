#pragma once
#include "windows.h"

#include <array>
#include <string>

constexpr static uint8_t OBFUSCATION_KEY = 0x25;

template<auto string_length>
consteval std::array<char, string_length> obfuscate(const std::string_view& string)
{
	std::array<char, string_length> obfuscated_string{};
	for (uint64_t index = 0; index < string_length; index++)
	{
		obfuscated_string[index] = string[index] ^ OBFUSCATION_KEY;
	}

	return obfuscated_string;
}

template<auto string_length>
std::string deobfuscate(const std::array<char, string_length>& obfuscated_string)
{
	std::string deobfuscated_string{};
	for (auto character : obfuscated_string)
	{
		deobfuscated_string.push_back(character ^ OBFUSCATION_KEY);
	}

	return deobfuscated_string;
}

template<auto string_length>
consteval std::array<wchar_t, string_length> wobfuscate(const std::wstring_view& string)
{
	std::array<wchar_t, string_length> obfuscated_string{};
	for (uint64_t index = 0; index < string_length; index++)
	{
		obfuscated_string[index] = string[index] ^ OBFUSCATION_KEY;
	}

	return obfuscated_string;
}

template<auto string_length>
std::wstring wdeobfuscate(const std::array<wchar_t, string_length>& obfuscated_string)
{
	std::wstring deobfuscated_string{};
	for (auto character : obfuscated_string)
	{
		deobfuscated_string.push_back(character ^ OBFUSCATION_KEY);
	}

	return deobfuscated_string;
}