
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
	std::string cmd;

	// Commands of size 1 are executions
	// Commands of size 3 are pop/push, etc...
	if (commands.size() == 1)
	{

	}
	else
	{
		const std::string& location{ commands.at(1) };
		const std::string& offset{ commands.at(2) };
		if (commands.at(0) == "push")
		{
			if (location == "constant")
			{
				cmd = ToConstant(offset) + D_Address();
			}
			else if (location == "local")
			{
				cmd = ToLocal(offset) + D_Memory();
			}
			cmd += StackInto() + StackIncrement();
			std::cout << cmd << "\n";
		}
		else if(commands.at(0) == "pop")
		{
			// Pop with no temps
			// void (int* x, int* y)
			//		*x = *x + *y;
			//		*y = *x - *y;
			//		*x = *x - *y;
			//
			cmd = StackDecrement() + StackFrom() + ToTemp(0) + M_Data();
			if (location == "local")
			{
				cmd += ToLocalBase() + "D=D+M\n@" + offset + "\nD=D+A\n";
			}
			cmd += ToStack() + A_Memory() + A_Memory() + "A=D-A\nM=D-A\n";
		}
	}

	return cmd;
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
				WriteLineToFile(outFile, ("// " + line));
				std::string assembly{ BuildAssembly(commands) };
				WriteLineToFile(outFile, assembly);
			}
		}

		inFile.close();
		outFile.close();

		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
