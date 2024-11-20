#include "Service.hpp"
#include "WindowsException.hpp"

Service::Service(const SC_HANDLE& manager_handle,
	const std::wstring& service_name,
	const ServiceAccessRights desired_access,
	const ServiceType service_type,
	const ServiceStartType start_type,
	const std::filesystem::path& binary_path)
	: m_handle(create_service(manager_handle, service_name, desired_access, service_type, start_type, binary_path))
{
}

Service::~Service()
{
	try
	{
		const BOOL close_service_result = CloseServiceHandle(m_handle);
		if (close_service_result == FALSE)
		{
			throw WindowsException(ArcaneErrors::ErrorCodes::CloseServiceHandleFailed);
		}
	}
	CATCH_ALL("Exception in Service Destructor!")
}

void Service::start()
{
	static constexpr uint32_t NO_ARGUMENTS = 0;
	static constexpr LPCWSTR *NO_ARGUMENTS_PTR = nullptr;
	const BOOL start_service_result = StartServiceW(m_handle, NO_ARGUMENTS, NO_ARGUMENTS_PTR);
	if (start_service_result == FALSE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::StartServiceFailed);
	}
}

void Service::stop()
{
	SERVICE_STATUS service_status;
	static constexpr uint32_t STOP_SERVICE = SERVICE_CONTROL_STOP;
	const BOOL control_service_result = ControlService(m_handle, STOP_SERVICE, &service_status);
	if (control_service_result == FALSE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::ControlServiceFailed);
	}
}

void Service::remove()
{
	const BOOL delete_service_result = DeleteService(m_handle);
	if (delete_service_result == FALSE)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::DeleteServiceFailed);
	}
}

SC_HANDLE Service::create_service(const SC_HANDLE& manager_handle,
	const std::wstring& service_name,
	const ServiceAccessRights desired_access,
	const ServiceType service_type,
	const ServiceStartType start_type,
	const std::filesystem::path& binary_path)
{
	static constexpr DWORD NORMAL_ERROR = SERVICE_ERROR_NORMAL;
	static constexpr LPCWSTR NO_LOAD_ORDER = nullptr;
	static constexpr LPDWORD NO_TAG = nullptr;
	static constexpr LPCWSTR NO_DEPENDENCIES = nullptr;
	static constexpr LPCWSTR NO_START_NAME = nullptr;
	static constexpr LPCWSTR NO_PASSWORD = nullptr;

	const SC_HANDLE out_handle = CreateServiceW(manager_handle,
		service_name.c_str(),
		service_name.c_str(),
		desired_access,
		service_type,
		start_type,
		NORMAL_ERROR,
		binary_path.c_str(),
		NO_LOAD_ORDER,
		NO_TAG,
		NO_DEPENDENCIES,
		NO_START_NAME,
		NO_PASSWORD);
	if (!out_handle)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::CreateServiceFailed);
	}

	return out_handle;
}
