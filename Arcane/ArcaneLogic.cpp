#include "File.hpp"
#include "Primal.hpp"
#include "ArcaneLogic.hpp"
#include "ByteVectorUtils.hpp"

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
	if (!std::filesystem::exists(ARCANE_KILL_PATH))
	{
		return true;
	}

	File kill_file(ARCANE_KILL_PATH, FileAccess::GenericRead, FileShare::All, FileCreationDisposition::OpenExisting);

	const size_t kill_file_size = kill_file.size();
	if (kill_file_size < ARCANE_KILL_MAGIC.size())
	{
		return true;
	}

	const ByteVector kill_data_raw = kill_file.read(ARCANE_KILL_MAGIC.size());
	const std::wstring kill_data = ByteVectorUtils::to_wstring(kill_data_raw);

	return kill_data != ARCANE_KILL_MAGIC;
}

}
