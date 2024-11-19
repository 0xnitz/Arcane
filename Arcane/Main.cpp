#include "Exception.hpp"
#include "DefinesMacros.hpp"

#include <windows.h>

int wWinMain(
    UNUSED(_In_ HINSTANCE hInstance),
    UNUSED(_In_opt_ HINSTANCE hPrevInstance),
    UNUSED(_In_ LPWSTR lpCmdLine),
    UNUSED(_In_ int nShowCmd)
)
{
    // protections

    try
    {
        DEBUG_PRINT("Hello, World")
    }
    CATCH_ALL()

	return 0;
}