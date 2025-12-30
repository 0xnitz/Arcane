#include "Primal.hpp"
#include "Exception.hpp"

Primal::Primal(const std::filesystem::path& primal_path) :
	m_running(false),
	m_manager(ServiceManager())
{
	start_primal(primal_path);
	if (m_running == false)
	{
		DEBUG_PRINT(std::string(OBFUSCATE("Error Starting Primal!")));

		throw Exception(ArcaneErrors::ErrorCodes::StartServiceFailed);
	}

	m_primal_device = std::make_unique<File>(PRIMAL_DEVICE_NAME,
		static_cast<FileAccess>(FileAccess::GenericRead | FileAccess::GenericWrite),
		FileShare::None,
		FileCreationDisposition::OpenExisting);
}

Primal::~Primal()
{
	try
	{
		stop_primal();
	}
	CATCH_ALL(OBFUSCATE("Exception in Primal Destructor!"))
}

void Primal::start_primal(const std::filesystem::path& primal_path)
{
	m_manager.add_service(PRIMAL_SERVICE_NAME, ServiceAccessRights::ServiceAllAccess, ServiceType::KernelDriver, ServiceStartType::DemandStart, primal_path);

	try
	{
		m_manager.start_service(PRIMAL_SERVICE_NAME);
		m_running = true;
	}
	catch (const Exception& exception)
	{
		if (exception.get_error_code() != ArcaneErrors::ErrorCodes::StartServiceFailed)
		{
			throw;
		}

		DEBUG_PRINT(std::string(OBFUSCATE("Error loading Primal!! ") + std::to_string(GetLastError())))
	}	
}

void Primal::stop_primal()
{
	if (m_running)
	{
		m_manager.stop_service(PRIMAL_SERVICE_NAME);
	}
	
	m_manager.remove_service(PRIMAL_SERVICE_NAME);
}

ByteVector Primal::read_physical(Address64 address, size_t size)
{
	m_primal_device->seek(address);

	return m_primal_device->read(size);
}

void Primal::write_physical(ByteVector data, Address64 address)
{
	m_primal_device->seek(address);

	m_primal_device->write(data);
}
