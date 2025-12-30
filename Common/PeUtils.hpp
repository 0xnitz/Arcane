#pragma once

#include "DefinesMacros.hpp"

// I'm not using a static lib to save on bloat
// TODO: remove all non-pic functions to remove code duplication
namespace pe_utils
{

NO_DISCARD size_t calculate_module_checksum(const wchar_t* module_name);

NO_DISCARD size_t calculate_function_checksum(const char* function_name);

NO_DISCARD Address64 rva_to_file_offset(const ByteVector& module_bytes, Address64 rva);

NO_DISCARD const IMAGE_NT_HEADERS64* get_nt_headers_pointer(const ByteVector& module_bytes);

NO_DISCARD std::vector<Address64> get_exports_rvas(const ByteVector& module_bytes);

NO_DISCARD Address64 pic_rva_to_file_offset(Byte* module_bytes, Address64 rva);

NO_DISCARD IMAGE_NT_HEADERS64* pic_get_nt_headers_pointer(Byte* module_bytes);

NO_DISCARD Address64 pic_get_export_by_name(Byte* module_bytes, const char* export_name);

NO_DISCARD PIMAGE_IMPORT_DESCRIPTOR pic_get_first_import(Byte* module_bytes, Address64 base_address);

}
