#pragma once

#include "Process.hpp"
#include "DefinesMacros.hpp"
#include "SmartHandleBase.hpp"

enum ThreadCreationFlags : DWORD
{
	CreateSuspended = CREATE_SUSPENDED
};

enum ThreadAccess : DWORD
{
	ThreadAllAccess = THREAD_ALL_ACCESS,
	ThreadSuspendResume = THREAD_SUSPEND_RESUME,
	ThreadTerminate = THREAD_TERMINATE
};

class Thread final
{
public:
	explicit Thread(const Tid tid,
		const ThreadAccess thread_access);

	explicit Thread(Process& process,
		const ThreadCreationFlags creation_flags,
		LPTHREAD_START_ROUTINE thread_start,
		LPVOID param);

	Thread(Thread const&) = delete;
	Thread(Thread&&) = delete;
	Thread operator=(Thread const&) = delete;
	Thread operator=(Thread&&) = delete;

	uint32_t suspend();

	uint32_t resume();

	void queue_apc(PAPCFUNC callback, ULONG_PTR param);

private:
	NO_DISCARD static SmartHandle open_thread(const Tid tid,
		const ThreadAccess thread_access);

	NO_DISCARD static SmartHandle create_thread(Process& process,
		const ThreadCreationFlags creation_flags,
		LPTHREAD_START_ROUTINE thread_start,
		LPVOID param);

	NO_DISCARD static Tid get_thread_tid(HANDLE thread_handle);

	SmartHandle m_handle; // Thread handle.
	Tid m_tid; // Thread ID.
};

using ThreadPtr = std::unique_ptr<Thread>;