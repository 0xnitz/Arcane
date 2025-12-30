#include "Encryption.hpp"

namespace simple_encryption
{

	/*NO_DISCARD ByteVector encrypt(const ByteVector& input_bytes)
	{
		return xor_bytes_with_hardcoded_key(input_bytes);
	}

	NO_DISCARD ByteVector decrypt(const ByteVector& input_bytes)
	{
		return xor_bytes_with_hardcoded_key(input_bytes);
	}*/

	NO_DISCARD ByteVector xor_bytes_with_hardcoded_key(const ByteVector& input_bytes)
	{
		ByteVector xored_bytes;
		xored_bytes.resize(input_bytes.size());

		for (const Byte& current_byte : input_bytes)
		{
			xored_bytes.push_back(current_byte ^ XOR_KEY);
		}

		return xored_bytes;
	}

}