#pragma once

#include "DefinesMacros.hpp"
#include "ReflectiveLoader.hpp"

namespace reflective_resolver
{

typedef FARPROC(*GETPROCADDRESS)(HMODULE hModule, LPCSTR lpProcName);
typedef HMODULE(*LOADLIBRARYA)(LPCSTR lpLibFileName);
typedef BOOL(*DLLENTRYPOINT)(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

// Resolves all imports, calls DLLMain
__declspec(dllexport) void resolve_imports(PREFLECTIVE_PARAMS params);

}