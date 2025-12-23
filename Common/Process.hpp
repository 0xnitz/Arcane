#pragma once

#include "DefinesMacros.hpp"
#include "SmartHandleBase.hpp"

#include <filesystem>

namespace process_utils
{

NO_DISCARD static Pid get_pid_by_name(const std::wstring& process_name);

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

class Process final
{
public:
	explicit Process(const std::filesystem::path& file_path,
		const std::wstring& command_line);

	explicit Process(Pid pid,
		const ProcessAccess access_rights,
		const std::wstring& process_name);

	Process(Process const&) = delete;
	Process(Process&&) = delete;
	Process operator=(Process const&) = delete;
	Process operator=(Process&&) = delete;

	NO_DISCARD ByteVector read(const Address64 address, const size_t size_to_read) const;

	void write(const Address64 address, const ByteVector& data);

	NO_DISCARD HANDLE get_handle(); // BAD BUT I WILL CHANGE IT

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