
#include "assembler.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <bitset>

// TODO: Labels only work if they show up before they are referenced
// Add a reverse replacement or have 2 passes

static std::unordered_map<std::string, hline> gSymbols
{
	// Registers
	{"R0",		0},
	{"R1",		1},
	{"R2",		2},
	{"R3",		3},
	{"R4",		4},
	{"R5",		5},
	{"R6",		6},
	{"R7",		7},
	{"R8",		8},
	{"R9",		9},
	{"R10",		10},
	{"R11",		11},
	{"R12",		12},
	{"R13",		13},
	{"R14",		14},
	{"R15",		15},
	// Accessories
	{"SCREEN",	16384},
	{"R10",		24576},
	// Generic
	{"SP",		0},
	{"LCL",		1},
	{"ARG",		2},
	{"THIS",	3},
	{"THAT",	4},
};
static const std::unordered_map<std::string, hins> gStatements
{
	{"0",	0b0101010},
	{"1",	0b0111111},
	{"-1",	0b0111010},

	{"D",	0b0001100},
	{"A",	0b0110000},
	{"M",	0b1110000},

	{"!D", 0b0001111},
	{"!A", 0b0110011},
	{"!M", 0b1110011},

	{"-D", 0b0001111},
	{"-A", 0b0110011},
	{"-M", 0b1110011},

	{"D+1", 0b0011111},
	{"A+1", 0b0110111},
	{"M+1", 0b1110111},

	{"D+1", 0b0001110},
	{"A+1", 0b0110010},
	{"M+1", 0b110010},

	{"D+A", 0b0000010},
	{"D+M", 0b1000010},

	{"D-A", 0b0010011},
	{"D-M", 0b1010011},

	{"A-D", 0b0000111},
	{"A-M", 0b1000111},

	{"D&A", 0b0000000},
	{"D&M", 0b1000000},

	{"D|A", 0b0010101},
	{"D|M", 0b1010101}

};
static const std::unordered_map<std::string, hins> gJumps
{
	{"JGT", 0b001},
	{"JEQ", 0b010},
	{"JLT", 0b100},
	{"JGT", 0b001},
	{"JGE", 0b011},
	{"JLE", 0b110},
	{"JMP", 0b111},
};

static std::unordered_map<std::string, std::vector<hachar>> gLabelReplacements{};

line_state FormatLine(std::string& line, uint16_t& lnum)
{
	line_state state{};
	bool isComment{ false };
	bool commentInit{ false };

	char* startChar{ nullptr };
	char* endChar{ nullptr };

	line.erase(std::remove_if(line.begin(), line.end(),
		[&](char& c)
		{
			if (!isComment)
			{
				if (c == '/')
				{
					isComment = true;
					//if (commentInit)
					//{
						//isComment = true;
						//state.hasErrors = false;
					//}
					//commentInit = true;
					//state.hasErrors = true;
				}
				else if (c == ';')
					state.hasJump = true;
				else if (c == '=')
					state.hasDestination = true;
				else if (c == '(')
				{
					state.isLabel = true;
					startChar = &c;
				}
				else if (c == ')')
					endChar = &c;
			}
			return ((c == ' ') || isComment);
		}),
		line.end());

	if (startChar != nullptr && endChar != nullptr)
	{
		std::string index{ startChar + 1, endChar };
		gSymbols[index] = lnum;
	}

	return state;
}

void SetInstructionTypeBit(hins& instruction, std::string& line)
{
	instruction |= (((line[0] != '@') & 1U) << 15);
}

bool IsNumber(std::string& str)
{
	return std::all_of(str.begin(), str.end(), ::isdigit);
}

hins BuildInstruction(std::string& line, const line_state& state, uint16_t& lnum, hachar fileposition)
{
	hins instruction{ 0 };
	SetInstructionTypeBit(instruction, line);
	if (line[0] == '@')
	{
		std::string strnum{ line.begin() + 1, line.end() };
		int32_t num{ 0 };
		if (IsNumber(strnum))
		{
			num = std::stoi(strnum);
		}
		else if (gSymbols.contains(strnum))
		{
			num = gSymbols.at(strnum);
		}
		else
		{
			gLabelReplacements[strnum].push_back(fileposition);
		}
		instruction |= (num & 0b0111111111111111); // 15 bitmask
	}
	else
	{
		instruction |= (0b11 << 13); // padding
		int32_t cursor{ 0 };

		// Destination
		if (state.hasDestination)
		{
			if (cursor < line.length())
			{
				char c{ line[cursor] };
				while ((cursor < line.length()) && (cursor < 4) && (c != '='))
				{
					if (c == 'M')
						instruction |= (1U << 3);
					else if (c == 'D')
						instruction |= (1U << 4);
					else if (c == 'A')
						instruction |= (1U << 5);
					cursor++;
					if (cursor < line.length())
						c = line[cursor];
				}
			}
			cursor++; // move past =
		}

		// Statement
		{
			std::string statement{};
			char& c{ line[cursor] };
			while ((c != ';') && (cursor < line.length()))
			{
				statement += c;
				cursor++;
				c = line[cursor];
			}
			cursor++;
			if (gStatements.contains(statement))
			{
				instruction |= (gStatements.at(statement) << 6);
			}
		}

		// Jump
		if (state.hasJump)
		{
			std::string jumpCode{ line.begin() + cursor, line.begin() + cursor + 3 }; // all jump codes are 3 length
			if (gJumps.contains(jumpCode))
				instruction |= gJumps.at(jumpCode);
		}
	}
	return instruction;
}

hins BuildReverseLabelInstruction(const std::string& label)
{
	hins instruction{ 0 };
	int32_t num{ gSymbols.at(label) };
	instruction |= (num & 0b0111111111111111); // 15 bitmask
	return instruction;
}

void WriteInstruction(hins& instruction, std::ofstream& file)
{
	file << std::bitset<16>(instruction).to_string() << std::endl;
}

int NANDHackAssembleFile(const std::filesystem::path& filepath)
{
	if (filepath.extension() == ".asm")
	{
		std::ifstream inFile{};
		inFile.open(filepath.string());

		std::string filename{ filepath.filename().replace_extension(".hack").string() };
		std::ofstream outFile{ filename };

		std::string line{};

		uint16_t linecount{ 0 };
		while (std::getline(inFile, line))
		{
			line_state state{ FormatLine(line, linecount) };
			if (!line.empty())
			{
				if (!state.isLabel)
				{
					hachar linePosition{ static_cast<hachar>(outFile.tellp()) };
					hins instruction{ BuildInstruction(line, state, linecount, linePosition) };
					WriteInstruction(instruction, outFile);
					linecount++;
				}
			}
		}

		// Fill in missing labels
		for (const std::pair<std::string, std::vector<hachar>>& pair : gLabelReplacements)
		{
			for (const hachar& lc : pair.second)
			{
				//std::cout << lc << std::endl;
				outFile.seekp(lc);
				hins instruction{ BuildReverseLabelInstruction(pair.first) };
				WriteInstruction(instruction, outFile);
			}
		}

		outFile.close();
		inFile.close();
	}
	else
	{
		std::cout << "File extension must be *.asm\n";
	}
	return EXIT_SUCCESS;
}
