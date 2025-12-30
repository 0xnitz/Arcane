#include "PeUtils.hpp"

namespace pe_utils
{

NO_DISCARD size_t calculate_module_checksum(const wchar_t* module_name)
{
	size_t checksum = 0;
	const wchar_t* module_iterator = module_name;
	wchar_t current_char = *module_iterator;
	while (current_char)
	{
		checksum += current_char;

		module_iterator++;
		current_char = *module_iterator;
	}

	return checksum;
}

NO_DISCARD size_t calculate_function_checksum(const char* function_name)
{
	size_t checksum = 0;
	const char* function_iterator = function_name;
	char current_char = *function_iterator;
	while (current_char)
	{
		checksum += current_char;

		function_iterator++;
		current_char = *function_iterator;
	}

	return checksum;
}

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

NO_DISCARD IMAGE_NT_HEADERS64* pic_get_nt_headers_pointer(Byte* module_bytes)
{
	const IMAGE_DOS_HEADER* dos_header = reinterpret_cast<const IMAGE_DOS_HEADER*>(module_bytes);

	return reinterpret_cast<IMAGE_NT_HEADERS64*>(module_bytes + dos_header->e_lfanew);
}

NO_DISCARD Address64 pic_get_export_by_name(Byte* module_bytes, const char* export_name)
{
	const IMAGE_NT_HEADERS64* nt_headers = pic_get_nt_headers_pointer(module_bytes);
	const IMAGE_DATA_DIRECTORY& export_directory_rva = nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if (export_directory_rva.VirtualAddress == 0 || export_directory_rva.Size == 0)
	{
		return {};
	}

	Address64 export_directory_offset = pic_rva_to_file_offset(module_bytes, export_directory_rva.VirtualAddress);
	const IMAGE_EXPORT_DIRECTORY* export_directory = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(module_bytes + static_cast<size_t>(export_directory_offset));

	const DWORD number_of_functions = export_directory->NumberOfFunctions;
	const DWORD func_table_rva = export_directory->AddressOfFunctions;
	Address64 func_table_off = pic_rva_to_file_offset(module_bytes, func_table_rva);
	const DWORD* func_table = reinterpret_cast<const DWORD*>(module_bytes + static_cast<size_t>(func_table_off));

	const DWORD names_table_rva = export_directory->AddressOfNames;
	Address64 names_table_offset = pic_rva_to_file_offset(module_bytes, names_table_rva);
	const char** names_table = reinterpret_cast<const char**>(module_bytes + static_cast<size_t>(names_table_offset));

	for (DWORD i = 0; i < number_of_functions; ++i)
	{
		if (calculate_function_checksum(names_table[i]) == calculate_function_checksum(export_name))
		{
			return func_table[i];
		}
	}

	return 0;
}

NO_DISCARD PIMAGE_IMPORT_DESCRIPTOR pic_get_first_import(Byte* module_bytes, Address64 base_address)
{
	const IMAGE_NT_HEADERS64* nt_headers = pic_get_nt_headers_pointer(module_bytes);
	const IMAGE_DATA_DIRECTORY& import_directory_rva = nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	if (import_directory_rva.VirtualAddress == 0 || import_directory_rva.Size == 0)
	{
		return {};
	}

	Address64 import_directory_offset = pic_rva_to_file_offset(module_bytes, import_directory_rva.VirtualAddress);
	
	return reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(base_address + import_directory_offset);
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

NO_DISCARD Address64 pic_rva_to_file_offset(Byte* module_bytes, Address64 rva)
{
	const IMAGE_NT_HEADERS64* nt_headers = pic_get_nt_headers_pointer(module_bytes);
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

}