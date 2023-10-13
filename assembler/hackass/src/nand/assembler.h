#pragma once

#include <string>
#include <filesystem>

using hins = uint16_t;

struct line_state
{
	bool hasDestination{ false };
	bool hasJump{ false };
	bool hasErrors{ false };
};

// Create binary machine code file from an assembly code file
// @brief Outputs a *.hack file into exe directory. Filename will be the same as [filepath] parameter
// @param filepath - *.asm file to convert
// @return Pass or fail int
int NANDHackAssembleFile(const std::filesystem::path& filepath);

// Set the 15th bit of a Hack Instruction according to A or C instruction
// @param instruction - current binary instruction line to modify
// @param line - current assembly code line to translate from
void SetInstructionTypeBit(hins& instruction, std::string& line);

// Remove whitespace and get hints on what the line contains
// @param line - current assembly code line to translate from
// @return line_state struct containing command hints
line_state FormatLine(std::string& line);

// Build a hack machine code from a formatted assembly line
// @param line - formatted assembly line
// @param state - hints for the assembler
// @return Hack binary machine instruction
hins BuildInstruction(std::string& line, const line_state& state);
