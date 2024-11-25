#include "Primal.hpp"
#include "ArcaneLogic.hpp"

namespace ArcaneLogic
{

void run(const std::wstring& command_line)
{
	Primal primal(command_line);

	// Startup plugin threads/dlls

	while (should_run())
	{
		Sleep(1000);
	}
}

bool should_run()
{
	return true;
}

}
