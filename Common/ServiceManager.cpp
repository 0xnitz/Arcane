#include "ServiceManager.hpp"
#include "WindowsException.hpp"

#include <winternl.h>

ServiceManager::ServiceManager() :
	m_handle(open_service_manager())
{
}

void ServiceManager::add_service(const std::wstring& service_name,
	const ServiceAccessRights desired_access,
	const ServiceType service_type,
	const ServiceStartType start_type,
	const std::filesystem::path& binary_path)
{
	m_services[service_name] = std::make_unique<Service>(m_handle,
		service_name,
		desired_access,
		service_type,
		start_type,
		binary_path);
}

void ServiceManager::start_service(const std::wstring& service_name)
{
	m_services[service_name]->start();
}

void ServiceManager::stop_service(const std::wstring& service_name)
{
	m_services[service_name]->stop();
}

void ServiceManager::remove_service(const std::wstring& service_name)
{
	m_services[service_name]->remove();
	m_services.erase(service_name);
}

ServiceManager::~ServiceManager()
{
	try
	{
		const BOOL close_service_result = CloseServiceHandle(m_handle);
		if (close_service_result == FALSE)
		{
			throw WindowsException(ArcaneErrors::ErrorCodes::CloseServiceHandleFailed);
		}
	}
	CATCH_ALL("Exception in ServiceManager Destructor!")
}

SC_HANDLE ServiceManager::open_service_manager()
{
	const SC_HANDLE out_handle = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
	if (out_handle == nullptr)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::OpenSCManagerFailed);
	}

	return out_handle;
}
