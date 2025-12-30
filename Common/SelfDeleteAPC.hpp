#pragma once

#include "Thread.hpp"
#include "SelfDelete.hpp"
#include "DefinesMacros.hpp"

class SelfDeleteAPC : public SelfDelete
{
public:
	using SelfDelete::SelfDelete;

	virtual void do_delete();

	SelfDeleteAPC(SelfDeleteAPC const&) = delete;
	SelfDeleteAPC(SelfDeleteAPC&&) = delete;
	SelfDeleteAPC operator=(SelfDeleteAPC const&) = delete;
	SelfDeleteAPC operator=(SelfDeleteAPC&&) = delete;
};