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

	ByteVector bytes = primal.primal_read_physical(0x1000, 0x8);
	uint64_t physical_address = reinterpret_cast<uint64_t>(bytes.data());
	DEBUG_PRINT(std::string("Physical Address: " + std::to_string(physical_address)));

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
