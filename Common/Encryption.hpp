#pragma once

#include "DefinesMacros.hpp"

constexpr static uint8_t XOR_KEY = 0x17;

// POC, just XOR. Can be anything tbh
// Also this code will be heavily obfuscated via vmpack or something in a real world scenario
namespace simple_encryption
{

// commenting these out because the compiler is mad they do nothing but call xor_bytes_with_hardcoded_key

//static NO_DISCARD ByteVector encrypt(const ByteVector& input_bytes);

//static NO_DISCARD ByteVector decrypt(const ByteVector& input_bytes);

NO_DISCARD ByteVector xor_bytes_with_hardcoded_key(const ByteVector& input_bytes);

}
