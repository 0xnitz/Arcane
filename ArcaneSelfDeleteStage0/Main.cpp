#include "Stage0Logic.hpp"
#include "DefinesMacros.hpp"

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
        Stage0Logic::run(lpCmdLine);

        DEBUG_PRINT(std::string("Stage 0 finished execution! Cleanup..."))
    }
    CATCH_ALL(OBFUSCATE("Exception caught in Stage0 main!"))

	return 0;
}