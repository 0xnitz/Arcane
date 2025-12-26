#include "PeUtils.hpp"

namespace pe_utils
{

NO_DISCARD Address64 rva_to_file_offset(const ByteVector& module_bytes, Address64 rva)
{
	const IMAGE_NT_HEADERS64* nt_headers = get_nt_headers_pointer(module_bytes);
	const IMAGE_OPTIONAL_HEADER64* optional_header = &nt_headers->OptionalHeader;
	const IMAGE_SECTION_HEADER* section_headers = reinterpret_cast<const IMAGE_SECTION_HEADER*>(
		reinterpret_cast<Address64>(optional_header) + nt_headers->FileHeader.SizeOfOptionalHeader);

	for (WORD i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i)
	{
		Address64 section_start = section_headers[i].VirtualAddress;
		size_t section_size = section_headers[i].Misc.VirtualSize ? section_headers[i].Misc.VirtualSize : section_headers[i].SizeOfRawData;
		
		if (rva >= section_start && rva < section_start + section_size)
		{
			return section_headers[i].PointerToRawData + (rva - section_start);
		}
	}

	return 0;
}

NO_DISCARD const IMAGE_NT_HEADERS64* get_nt_headers_pointer(const ByteVector& module_bytes)
{
	if (module_bytes.size() < sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS64))
	{
		return nullptr;
	}

	const IMAGE_DOS_HEADER* dos_header = reinterpret_cast<const IMAGE_DOS_HEADER*>(module_bytes.data());
	if (dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS64) > module_bytes.size())
	{
		return nullptr;
	}

	return reinterpret_cast<const IMAGE_NT_HEADERS64*>(module_bytes.data() + dos_header->e_lfanew);
}

std::vector<Address64> get_exports_rvas(const ByteVector& module_bytes)
{
	const IMAGE_NT_HEADERS64* nt_headers = get_nt_headers_pointer(module_bytes);
	const IMAGE_DATA_DIRECTORY& export_directory_rva = nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if (export_directory_rva.VirtualAddress == 0 || export_directory_rva.Size == 0)
	{
		return {};
	}

	Address64 export_directory_offset = rva_to_file_offset(module_bytes, export_directory_rva.VirtualAddress);
	if (!export_directory_offset || export_directory_offset + sizeof(IMAGE_EXPORT_DIRECTORY) > module_bytes.size())
	{
		return {};
	}

	const IMAGE_EXPORT_DIRECTORY* export_directory = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(module_bytes.data() + static_cast<size_t>(export_directory_offset));
	const DWORD number_of_functions = export_directory->NumberOfFunctions;
	const DWORD func_table_rva = export_directory->AddressOfFunctions;

	Address64 func_table_off = rva_to_file_offset(module_bytes, func_table_rva);
	const size_t funcs_bytes = static_cast<size_t>(number_of_functions) * sizeof(DWORD);
	if (!func_table_off || func_table_off + funcs_bytes > module_bytes.size())
	{
		return {};
	}

	const DWORD* func_table = reinterpret_cast<const DWORD*>(module_bytes.data() + static_cast<size_t>(func_table_off));
	std::vector<Address64> result;
	result.resize(number_of_functions);

	for (DWORD i = 0; i < number_of_functions; ++i)
	{
		result.push_back(static_cast<Address64>(func_table[i]));
	}

	return result;
}

}