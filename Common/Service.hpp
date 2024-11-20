#pragma once

#include "DefinesMacros.hpp"

#include <string>
#include <filesystem>

enum ServiceAccessRights
{
	QueryConfig = SERVICE_QUERY_CONFIG,
	ChangeConfig = SERVICE_CHANGE_CONFIG,
	QueryStatus = SERVICE_QUERY_STATUS,
	EnumerateDependents = SERVICE_ENUMERATE_DEPENDENTS,
	Start = SERVICE_START,
	Stop = SERVICE_STOP,
	PauseContinue = SERVICE_PAUSE_CONTINUE,
	Interrogate = SERVICE_INTERROGATE,
	UserDefinedControl = SERVICE_USER_DEFINED_CONTROL,
	Delete = DELETE,
	ReadControl = READ_CONTROL,
	WriteDac = WRITE_DAC,
	WriteOwner = WRITE_OWNER,
	StandardRightsRequired = STANDARD_RIGHTS_REQUIRED,
	AllAccess = SERVICE_ALL_ACCESS,
	StartStopDelete = DELETE | SERVICE_START | SERVICE_STOP
};

enum ServiceType
{
	KernelDriver = SERVICE_KERNEL_DRIVER,
	FileSystemDriver = SERVICE_FILE_SYSTEM_DRIVER,
	Adapter = SERVICE_ADAPTER,
	RecognizerDriver = SERVICE_RECOGNIZER_DRIVER,
	Win32OwnProcess = SERVICE_WIN32_OWN_PROCESS,
	Win32ShareProcess = SERVICE_WIN32_SHARE_PROCESS,
	InteractiveProcess = SERVICE_INTERACTIVE_PROCESS
};

enum ServiceStartType
{
	AutoStart = SERVICE_AUTO_START,
	BootStart = SERVICE_BOOT_START,
	DemandStart = SERVICE_DEMAND_START,
	Disabled = SERVICE_DISABLED,
	SystemStart = SERVICE_SYSTEM_START
};

class Service final
{
public:
	explicit Service(const SC_HANDLE& manager_handle,
		const std::wstring& service_name,
		ServiceAccessRights desired_access,
		ServiceType service_type,
		ServiceStartType start_type,
		const std::filesystem::path& binary_path);

	~Service();

	void start();

	void stop();

	void remove();

private:
	static NO_DISCARD SC_HANDLE create_service(const SC_HANDLE& manager_handle,
		const std::wstring& service_name,
		ServiceAccessRights desired_access,
		ServiceType service_type,
		ServiceStartType start_type,
		const std::filesystem::path& binary_path);

	SC_HANDLE m_handle; // Service handle.
};
