#pragma once

#include "File.hpp"
#include "DefinesMacros.hpp"
#include "ServiceManager.hpp"

#include <filesystem>

static const std::wstring PRIMAL_SERVICE_NAME = L"Primal";
static const std::filesystem::path PRIMAL_DEVICE_NAME = L"\\\\.\\Primal";

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

	ByteVector read_physical(Address64 address, size_t size);

	void write_physical(ByteVector data, Address64 address);

private:
	bool m_running; // Flag to indicate if the Primal driver is running.

	ServiceManager m_manager; // Service manager for the Primal driver.

	FilePtr m_primal_device; // Primal device to use for read/write.
};