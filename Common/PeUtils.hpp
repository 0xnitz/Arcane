#pragma once

#include "DefinesMacros.hpp"

// I'm not using a static lib to save on bloat
namespace pe_utils
{

NO_DISCARD Address64 rva_to_file_offset(const ByteVector& module_bytes, Address64 rva);

NO_DISCARD const IMAGE_NT_HEADERS64* get_nt_headers_pointer(const ByteVector& module_bytes);

NO_DISCARD std::vector<Address64> get_exports_rvas(const ByteVector& module_bytes);

}
