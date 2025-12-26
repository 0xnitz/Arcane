#pragma once

#include "DefinesMacros.hpp"

namespace reflective_resolver
{

// Resolves all imports, calls DLLMain
__declspec(dllexport) void resolve_imports(LPVOID command_line_pointer);

}