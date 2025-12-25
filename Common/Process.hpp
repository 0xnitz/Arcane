#pragma once

#include "DefinesMacros.hpp"
#include "SmartHandleBase.hpp"

#include <filesystem>

namespace process_utils
{

NO_DISCARD Pid get_pid_by_name(const std::wstring& process_name);

}

// TODO: Better thread creation API, currently revealing api. Tried to use friend but need to do it better

enum ProcessAccess : DWORD
{
	AccessDelete = DELETE,
	AccessRead = READ_CONTROL,
	AllAccess = PROCESS_ALL_ACCESS,
	AccessCreateThread = PROCESS_CREATE_THREAD,
	AccessSuspendResume = PROCESS_SUSPEND_RESUME,
	AccessReadWrite = PROCESS_VM_OPERATION
};

enum AllocationType : DWORD
{
	Commit = MEM_COMMIT,
	Reserve = MEM_RESERVE,
	Reset = MEM_RESET,
	ResetUndo = MEM_RESET_UNDO,
	LargePages = MEM_LARGE_PAGES,
	Physical = MEM_PHYSICAL,
	TopDown = MEM_TOP_DOWN
};

enum Protection : DWORD
{
	ProtectionNoAccess = PAGE_NOACCESS,
	ProtectionReadOnly = PAGE_READONLY,
	ProtectionReadWrite = PAGE_READWRITE,
	ProtectionReadExecute = PAGE_EXECUTE_READ,
	ProtectionReadWriteExecute = PAGE_EXECUTE_READWRITE,
	ProtectionCopyOnWrite = PAGE_WRITECOPY
};

class Process final
{
public:
	explicit Process(const std::filesystem::path& file_path,
		const std::wstring& command_line);

	explicit Process(Pid pid,
		const ProcessAccess access_rights,
		const std::wstring& process_name = L"");

	Process(Process const&) = delete;
	Process(Process&&) = delete;
	Process operator=(Process const&) = delete;
	Process operator=(Process&&) = delete;

	NO_DISCARD ByteVector read(const Address64 address, const size_t size_to_read) const;

	void write(const Address64 address, const ByteVector& data);

	Address64 allocate_memory(const size_t size, const AllocationType allocation_type, const Protection protection);

	void change_protection(const Address64 address, const size_t size, const Protection new_protection);

	NO_DISCARD HANDLE get_handle(); // BAD BUT I WILL CHANGE IT

	NO_DISCARD Pid get_pid();

private:
	NO_DISCARD static SmartHandle open_process(Pid pid,
		const ProcessAccess access_rights,
		const std::wstring& process_name);

	NO_DISCARD static SmartHandle create_process(const std::filesystem::path& file_path,
		const std::wstring& command_line);

	NO_DISCARD static Pid get_process_pid(HANDLE process_handle);

	SmartHandle m_handle; // Process handle.
	Pid m_pid; // Process ID.
};

using ProcessPtr = std::unique_ptr<Process>;