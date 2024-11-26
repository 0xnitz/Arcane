#pragma once

#include "DefinesMacros.hpp"

#include <string>
#include <filesystem>

namespace ArcaneLogic
{

enum ArcaneParameters
{
	DriverPath = 0
};

// TODO: make a stealthier kill switch
static const std::filesystem::path ARCANE_KILL_PATH = L"C:\\Windows\\Temp\\kill.txt";
static constexpr std::wstring_view ARCANE_KILL_MAGIC = L"1";

void run(const std::wstring& command_line);

bool should_run();

}