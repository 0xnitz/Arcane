#include "Primal.hpp"
#include "ArcaneLogic.hpp"

#include <winternl.h>

namespace ArcaneLogic
{

void run(const std::wstring& command_line)
{
	Primal primal(command_line);

	DEBUG_PRINT_W(command_line)

		Sleep(1000 * 5);
}

}
