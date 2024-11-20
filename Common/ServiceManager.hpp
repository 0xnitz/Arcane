#pragma once

#include "Service.hpp"
#include "DefinesMacros.hpp"
#include "SmartHandleBase.hpp"

#include <map>
#include <string>

using ServiceMap = std::map<std::wstring, std::unique_ptr<Service>>;

class ServiceManager final
{
public:
	explicit ServiceManager();

	ServiceManager(ServiceManager const&) = delete;
	ServiceManager(ServiceManager&&) = delete;
	ServiceManager operator=(ServiceManager const&) = delete;
	ServiceManager operator=(ServiceManager&&) = delete;

	void add_service(const std::wstring& service_name,
		ServiceAccessRights desired_access,
		ServiceType service_type,
		ServiceStartType start_type,
		const std::filesystem::path& binary_path);

	void start_service(const std::wstring& service_name);

	void stop_service(const std::wstring& service_name);

	void remove_service(const std::wstring& service_name);

private:
	NO_DISCARD static SmartSCHandle open_service_manager();

	SmartSCHandle m_handle; // Service Manager Handle.

	ServiceMap m_services; // Services managed by this Service Manager.
};