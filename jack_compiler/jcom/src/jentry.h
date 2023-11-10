#pragma once

#include <jackc.h>
#include <filesystem>

namespace jcom
{
	int CompileJack(const std::filesystem::path& filepath);
	int CompileJack2(const std::filesystem::path& filepath);
}
