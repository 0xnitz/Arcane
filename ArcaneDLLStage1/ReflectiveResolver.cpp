#include "ReflectiveResolver.hpp"

namespace reflective_resolver
{

__declspec(dllexport) void resolve_imports(UNUSED(LPVOID command_line_pointer))
{
	// needs to fix imports first but this just a boilerplate for now
	RESOLVE(User32.dll, MessageBoxA)(NULL, NULL, NULL, NULL);
}

}
