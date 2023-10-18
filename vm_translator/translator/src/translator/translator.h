#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

// Switch on the i'th bit
#define BITL(i) (1 << i)

using henum = uint32_t;
using hcmds = std::vector<std::string> ;

// Hack VM Command Type
//enum hCType : henum
//{
//	hCType_Add = BITL(0),
//	hCType_Sub = BITL(1)
//};

struct htr_line_state
{
	hcmds commands;
};

int TranslateVMCode(const std::filesystem::path& filepath);
