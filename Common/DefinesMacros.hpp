#pragma once

#include <windows.h>

#define NO_DISCARD [[nodiscard]]

#define UNUSED(var) [[maybe_unused]] var

#ifdef NDEBUG
#define DEBUG_PRINT(message)
#define DEBUG_PRINT_W(message)
#else
#define DEBUG_PRINT(message) OutputDebugStringA(message);
#define DEBUG_PRINT_W(message) OutputDebugStringW(message);
#endif