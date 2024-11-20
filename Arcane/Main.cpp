#include "Exception.hpp"
#include "ArcaneLogic.hpp"
#include "DefinesMacros.hpp"

#include <windows.h>

int wWinMain(
    UNUSED(_In_ HINSTANCE hInstance),
    UNUSED(_In_opt_ HINSTANCE hPrevInstance),
    _In_ LPWSTR lpCmdLine,
    UNUSED(_In_ int nShowCmd)
)
{
    // protections

    try
    {
		ArcaneLogic::run(lpCmdLine);
    }
    CATCH_ALL("Exception caught in Arcane main!")

	return 0;
}