#include "Process.hpp"

#include "tlhelp32.h"

namespace process_utils
{

NO_DISCARD Pid get_pid_by_name(const std::wstring& process_name)
{
	static constexpr DWORD CURRENT_PROCESS = 0;
	HANDLE process_walk_handle_inner = RESOLVE(kernel32.dll, CreateToolhelp32Snapshot)(TH32CS_SNAPPROCESS, CURRENT_PROCESS);
	if (process_walk_handle_inner == INVALID_HANDLE_VALUE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::CreateToolhelp32SnapshotFailed);
	}
	
	SmartHandle process_walk_handle(process_walk_handle_inner);

	PROCESSENTRY32W current_process;
	current_process.dwSize = sizeof(PROCESSENTRY32W);

	if (!RESOLVE(kernel32.dll, Process32FirstW)(process_walk_handle.get(), &current_process))
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::Process32FirstWFailed);
	}

	do
	{
		if (current_process.szExeFile == process_name)
		{
			return current_process.th32ProcessID;
		}
	} while (RESOLVE(kernel32.dll, Process32NextW)(process_walk_handle.get(), &current_process));

	return NULL;
}

}

Process::Process(const std::filesystem::path& file_path, const std::wstring& command_line) :
	m_handle(create_process(file_path, command_line)),
	m_pid(get_process_pid(m_handle.get()))
{
}

Process::Process(Pid pid, const ProcessAccess access_rights, const std::wstring& process_name = L"") :
	m_handle(open_process(pid, access_rights, process_name)),
	m_pid(pid)
{
}

NO_DISCARD ByteVector Process::read(const Address64 address, const size_t size_to_read) const
{
	ByteVector read_bytes;
	read_bytes.resize(size_to_read);
	SIZE_T bytes_read = 0;
	BOOL read_status = RESOLVE(kernel32.dll, ReadProcessMemory)(m_handle.get(),
		reinterpret_cast<LPCVOID>(address),
		read_bytes.data(),
		size_to_read,
		&bytes_read);
	if (read_status == FALSE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::ReadProcessMemoryFailed);
	}

	return read_bytes;
}

void Process::write(const Address64 address, const ByteVector& data)
{
	SIZE_T bytes_written = 0;
	BOOL write_status = RESOLVE(kernel32.dll, WriteProcessMemory)(m_handle.get(),
		reinterpret_cast<LPVOID>(address),
		data.data(),
		data.size(),
		&bytes_written);
	if (write_status == FALSE || bytes_written != data.size())
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::WriteProcessMemoryFailed);
	}
}

NO_DISCARD HANDLE Process::get_handle()
{
	return m_handle.get();
}

NO_DISCARD SmartHandle Process::open_process(Pid pid,
	const ProcessAccess access_rights,
	const std::wstring& process_name)
{
	if (process_name != L"")
	{
		pid = process_utils::get_pid_by_name(process_name);
	}

	static constexpr BOOL DONT_INHERIT = FALSE;
	HANDLE process_handle = RESOLVE(kernel32.dll, OpenProcess)(access_rights, DONT_INHERIT, pid);
	if (process_handle == nullptr)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::OpenProcessFailed);
	}

	return SmartHandle(process_handle);
}

NO_DISCARD SmartHandle Process::create_process(const std::filesystem::path& file_path,
	const std::wstring& command_line)
{
	STARTUPINFOW startup_info = {};
	PROCESS_INFORMATION process_information = {};

	std::wstring command_line_copy = command_line;
	static constexpr LPSECURITY_ATTRIBUTES DEFAULT_LPSECURITY_ATTRIBUTES = nullptr;
	static constexpr BOOL DONT_INHERIT = FALSE;
	static constexpr LPVOID NO_ENVIRONMENT = nullptr;
	static constexpr LPCWSTR SAME_DIRECTORY = nullptr;
	BOOL create_process_result = RESOLVE(kernel32.dll, CreateProcessW)(file_path.c_str(),
		command_line_copy.data(),
		DEFAULT_LPSECURITY_ATTRIBUTES,
		DEFAULT_LPSECURITY_ATTRIBUTES,
		DONT_INHERIT,
		NORMAL_PRIORITY_CLASS,
		NO_ENVIRONMENT,
		SAME_DIRECTORY,
		&startup_info,
		&process_information);
	if (create_process_result == FALSE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::CreateProcessFailed);
	}

	return SmartHandle(process_information.hProcess);
}

NO_DISCARD Pid Process::get_process_pid(HANDLE process_handle)
{
	return GetProcessId(process_handle);
}
