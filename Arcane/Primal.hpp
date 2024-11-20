#pragma once

#include "DefinesMacros.hpp"
#include "ServiceManager.hpp"

#include <filesystem>

static const std::wstring PRIMAL_SERVICE_NAME = L"Primal";

class Primal final
{
public:
	explicit Primal(const std::filesystem::path& primal_path);

	Primal(Primal const&) = delete;
	Primal(Primal&&) = delete;
	Primal operator=(Primal const&) = delete;
	Primal operator=(Primal&&) = delete;

	~Primal();

	void start_primal(const std::filesystem::path& primal_path);

	void stop_primal();

private:
	bool m_running; // Flag to indicate if the Primal driver is running.

	ServiceManager m_manager; // Service manager for the Primal driver.
};