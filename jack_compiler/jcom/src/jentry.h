#pragma once

#include <jackc.h>
#include <filesystem>

namespace jcom
{
	int CompileJack(const std::filesystem::path& filepath);
}
