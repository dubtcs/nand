
#include "translator.h"
#include "haclus.h"

#include <fstream>
#include <unordered_map>

/*
* SP		= Stack Pointer
* LCL		= Local
* ARG		= Arguments
* THIS		= This
* THAT		= That
* 16-255	= Static Variables
*/

hcmds GetCommandSet(std::string& line)
{
	hcmds commands{};

	std::string currentCommand{};
	for (char& c : line)
	{
		if (c == '/')
		{
			break;
		}
		else if (c == ' ')
		{
			commands.push_back(currentCommand);
			currentCommand = "";
		}
		else
		{
			currentCommand += c;
		}
	}

	if (!currentCommand.empty())
	{
		commands.push_back(currentCommand);
	}

	return commands;
}

std::string BuildAssembly(hcmds& commands)
{
	std::string assembly{};

	// Commands of size 1 are executions
	// Commands of size 3 are pop/push, etc...
	if (commands.size() == 1)
	{

	}
	else
	{
		if (commands.at(1) == "constant")
		{
			vmins cmd{ ToConstant(commands.at(2)) + D_Address() + StackInto() + StackIncrement()};
			std::cout << cmd;
		}

		//if (commands.at(1) == "constant")
		//{
		//	//std::format("@{}\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", commands.at(2));
		//	//std::cout << std::format("@{}\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", commands.at(2));
		//}
	}

	return assembly;
}

void WriteLineToFile(std::ofstream& file, const std::string& str) { file << str << "\n"; }
int TranslateVMCode(const std::filesystem::path& filepath)
{
	if (filepath.extension() == ".vm")
	{
		std::ifstream inFile{};
		inFile.open(filepath.string());

		std::string outName{ filepath.filename().replace_extension(".asm").string() };
		std::ofstream outFile{ outName };

		std::string line{};
		while (std::getline(inFile, line))
		{
			hcmds commands{ GetCommandSet(line) };
			if (!commands.empty())
			{
				std::string assembly{ BuildAssembly(commands) };
			}
		}

		inFile.close();
		outFile.close();

		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
