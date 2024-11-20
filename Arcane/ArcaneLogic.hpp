#pragma once

#include "DefinesMacros.hpp"

#include <string>

namespace ArcaneLogic
{
	enum ArcaneParameters
	{
		DriverPath = 0
	};

	void run(const std::wstring& command_line);
}