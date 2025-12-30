#pragma once

#include "Process.hpp"
#include "DefinesMacros.hpp"
#include "ReflectiveLoader.hpp"

class SelfDelete
{
public:
	explicit SelfDelete(const Pid remote_pid, const ProcessAccess access_rights);

	explicit SelfDelete();

	virtual void do_delete() = 0;

	SelfDelete(SelfDelete const&) = delete;
	SelfDelete(SelfDelete&&) = delete;
	SelfDelete operator=(SelfDelete const&) = delete;
	SelfDelete operator=(SelfDelete&&) = delete;

protected:
	ProcessPtr m_deletor_process; // The process that will run the deletion logic after the main process finishes execution.

	std::filesystem::path m_file_path; // The path of the to-delete file.
};

