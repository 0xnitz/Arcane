#pragma once

#include "Service.hpp"
#include "DefinesMacros.hpp"

#include <map>
#include <string>

using ServiceMap = std::map<std::wstring, std::unique_ptr<Service>>;

class ServiceManager final
{
public:
	explicit ServiceManager();

	void add_service(const std::wstring& service_name,
		ServiceAccessRights desired_access,
		ServiceType service_type,
		ServiceStartType start_type,
		const std::filesystem::path& binary_path);

	void start_service(const std::wstring& service_name);

	void stop_service(const std::wstring& service_name);

	void remove_service(const std::wstring& service_name);

	~ServiceManager();

private:
	NO_DISCARD static SC_HANDLE open_service_manager();

	SC_HANDLE m_handle; // Service Manager Handle.

	ServiceMap m_services; // Services managed by this Service Manager.
};