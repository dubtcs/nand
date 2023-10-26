#pragma once

#include "trnclu.h"
#include "sform.h"

#include <string>
#include <vector>
#include <filesystem>

using vmcmds = std::vector<vmins>;

int TranslateVMCode(const std::filesystem::path& filepath);
