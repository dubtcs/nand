
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
		cmd = ToStack() + A_Memory() + RSubtract1(A_REG, A_REG) + RSubtract1(A_REG, A_REG) + D_Memory() + ToStack() + A_Memory() + RSubtract1(A_REG, A_REG);
		if (commands.at(0) == "add")
		{
			// @SP
			// A=M
			// A=A-1
			// A=A-1
			// D=M
			// @SP
			// A=M
			// A=A-1
			// D=D-M
			// A=A-1
			// M=D
			// @SP
			// M=M-1
			cmd += RAdd(D_REG, D_REG, M_REG);
		}
		else if (commands.at(0) == "sub")
		{
			cmd += RSubtract(D_REG, D_REG, M_REG);
		}

		cmd += RSubtract1(A_REG, A_REG) + M_Data() + StackDecrement();
	}
	else
	{
		const std::string& location{ commands.at(1) };
		const std::string& offset{ commands.at(2) };
		if (commands.at(0) == "push")
		{
			if (location == H_CONST)
			{
				cmd = ToConstant(offset) + D_Address();
			}
			else
			{
				if (location == H_LOCAL)
				{
					cmd = ToLocal(offset);
				}
				else if (location == H_ARGS)
				{
					cmd = ToArgument(offset);
				}
				else if (location == H_THIS)
				{
					cmd = ToArgument(offset);
				}
				else if (location == H_THAT)
				{
					cmd = ToArgument(offset);
				}
				else if (location == H_TEMP)
				{
					cmd = ToTemp(stoi(offset));
				}

				cmd += D_Memory();
			}

			cmd += StackInto() + StackIncrement();
		}
		else if(commands.at(0) == "pop")
		{
			cmd = StackDecrement() + StackFrom();// + ToTemp(0) + M_Data();

			if (location == H_LOCAL)
			{
				cmd += ToLocalBase();
			}
			else if (location == H_ARGS)
			{
				cmd += ToArgument();
			}
			else if (location == H_THIS)
			{
				cmd += ToThis();
			}
			else if (location == H_THAT)
			{
				cmd += ToThat();
			}
			else if (location == H_STATIC)
			{
				//cmd += ToStatic();
			}
			else if (location == H_TEMP)
			{
				cmd += ToTemp(0);
			}
			else
				std::cout << "MISSING: " << location << "\n";

			cmd += RAdd(D_REG, D_REG, M_REG) + ToAddress(offset) + RAdd(D_REG, D_REG, A_REG);
			cmd += ToStack() + A_Memory() + A_Memory() + RSubtract(A_REG, D_REG, A_REG) + RSubtract(M_REG, D_REG, A_REG);//"A=D-A\nM=D-A\n";
		}
		else
			std::cout << "MISSING: " << commands.at(0) << "\n";


		//std::cout << cmd << "\n";
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
	std::cout << "Incorrect file extension *" << filepath.extension().string() << " must be *.vm\n";
	return EXIT_FAILURE;
}
