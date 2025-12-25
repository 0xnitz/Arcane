#include "Thread.hpp"

Thread::Thread(const Tid tid, const ThreadAccess thread_access) :
	m_process(std::make_unique<Process>(get_process_id_of_thread(m_handle), ProcessAccess::AllAccess)),
	m_handle(open_thread(tid, thread_access)),
	m_tid(tid)
{
}

Thread::Thread(const Pid pid, const ThreadCreationFlags creation_flags, LPTHREAD_START_ROUTINE thread_start, LPVOID param) :
	m_process(std::make_unique<Process>(pid, ProcessAccess::AllAccess)),
	m_handle(create_thread(m_process, creation_flags, thread_start, param)),
	m_tid(get_thread_tid(m_handle.get()))
{
}

uint32_t Thread::suspend()
{
	DWORD suspend_count = RESOLVE(kernel32.dll, SuspendThread)(m_handle.get());
	if (suspend_count == -1)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::SuspendThreadFailed);
	}

	return suspend_count;
}

uint32_t Thread::resume()
{
	DWORD suspend_count = RESOLVE(kernel32.dll, ResumeThread)(m_handle.get());
	if (suspend_count == -1)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::ResumeThreadFailed);
	}

	return suspend_count;
}

void Thread::queue_apc(PAPCFUNC callback, ULONG_PTR param)
{
	DWORD queue_apc_result = RESOLVE(kernel32.dll, QueueUserAPC)(callback, m_handle.get(), param);
	if (queue_apc_result == NULL)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::QueueUserApcFailed);
	}
}

NO_DISCARD Pid Thread::get_process_id_of_thread(const SmartHandle& thread_handle)
{
	Pid pid = GetProcessIdOfThread(thread_handle.get());
	if (pid == NULL)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::GetProcessIdOfThreadFailed);
	}

	return pid;
}

NO_DISCARD SmartHandle Thread::open_thread(const Tid tid, const ThreadAccess thread_access)
{
	static constexpr BOOL DONT_INHERIT = FALSE;
	HANDLE thread_handle = OpenThread(thread_access, DONT_INHERIT, tid);
	if (thread_handle == NULL)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::OpenThreadFailed);
	}

	return SmartHandle(thread_handle);
}

NO_DISCARD SmartHandle Thread::create_thread(const ProcessPtr& process, const ThreadCreationFlags creation_flags, LPTHREAD_START_ROUTINE thread_start, LPVOID param)
{
	static constexpr LPSECURITY_ATTRIBUTES DEFAULT_LPSECURITY_ATTRIBUTES = nullptr;
	static constexpr SIZE_T DEFAULT_STACK = 0;
	DWORD thread_id = 0;
	// TODO: need to do a friend class design without crossing includes
	HANDLE thread_handle = RESOLVE(kernel32.dll, CreateRemoteThread)(process->get_handle(),
		DEFAULT_LPSECURITY_ATTRIBUTES,
		DEFAULT_STACK,
		thread_start,
		param,
		creation_flags,
		&thread_id);
	if (thread_handle == NULL)
	{
		throw WindowsException(ArcaneErrors::ErrorCodes::CreateRemoteThreadFailed);
	}

	return SmartHandle(thread_handle);
}

NO_DISCARD Tid Thread::get_thread_tid(HANDLE thread_handle)
{
	return GetThreadId(thread_handle);
}
