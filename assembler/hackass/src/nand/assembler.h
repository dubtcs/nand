#pragma once

#include <string>
#include <filesystem>

using hins = uint16_t;
using hline = uint16_t;
using hachar = uint32_t;

struct line_state
{
	bool hasDestination{ false };
	bool hasJump{ false };
	bool hasErrors{ false };
	bool isLabel{ false };
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
// @param lineNumber - current assembly line number for label support
// @return line_state struct containing command hints
line_state FormatLine(std::string& line, uint16_t& lineNumber);

// Build a hack machine code from a formatted assembly line
// @param line - formatted assembly line
// @param state - hints for the assembler
// @param lineNumber - current assembly line number
// @param linePosition - output file character position for reverse labeling
// @return Hack binary machine instruction
hins BuildInstruction(std::string& line, const line_state& state, uint16_t& lineNumber, hachar linePosition);

// Build an A instruction for labels that were missing data during initial pass
// @brief Note: Does not seek to file position for replacement, this must be done manually
// @param label - label string to replace
// @return Hack binary machine instruction
hins BuildReverseLabelInstruction(const std::string& label);

// Write a hack machine instruction to a given file
// @param instruction - Hack instruction
// @param file - output file
void WriteInstruction(hins& instruction, std::ofstream& file);

// Check if a string can be converted into a number
// @param str - String to check
// @return yes or no
bool IsNumber(std::string& str);
