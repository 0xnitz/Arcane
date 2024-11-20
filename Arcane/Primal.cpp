#include "Primal.hpp"
#include "Exception.hpp"

Primal::Primal(const std::filesystem::path& primal_path) :
	m_running(false),
	m_manager(ServiceManager())
{
	start_primal(primal_path);
}

Primal::~Primal()
{
	try
	{
		stop_primal();
	}
	CATCH_ALL("Exception in Primal Destructor!")
}

void Primal::start_primal(const std::filesystem::path& primal_path)
{
	m_manager.add_service(PRIMAL_SERVICE_NAME, ServiceAccessRights::AllAccess, ServiceType::KernelDriver, ServiceStartType::DemandStart, primal_path);

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

		DEBUG_PRINT(std::string("Error loading Primal!! " + std::to_string(GetLastError())))
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
