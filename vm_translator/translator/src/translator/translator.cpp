
#include "translator.h"
#include "haclus.h"
#include "sform.h"

#include <fstream>
#include <unordered_map>

//
// TODO:
// MOVE ALL STRING OPERATIONS TO THE sform CLASS
//

/*
* SP		= Stack Pointer
* LCL		= Local
* ARG		= Arguments
* THIS		= This
* THAT		= That
* 5-12		= Temp
* 16-255	= Static Variables
* 
* pointer 0 should access THIS
* pointer 1 should access THAT
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
	static int32_t linecount{ 0 };

	const vmins& command{ commands.at(0) };

	// Commands of size 1 are executions
	// Commands of size 3 are pop/push
	if (commands.size() == 1)
	{
		// SIngle variable oeprations
		if ((command == "not") || (command == "neg"))
		{
			cmd = ToStack() + A_Memory() + RSubtract1(A_REG, A_REG) + D_Memory();
			cmd += ((command == "not") ? RNot(D_REG, D_REG) : RNegate(D_REG, D_REG));
			cmd += M_Data();
		}
		else
		{
			// D contains x
			// M contains y
			cmd = ToStack() + A_Memory() + RSubtract1(A_REG, A_REG) + RSubtract1(A_REG, A_REG) + D_Memory() + ToStack() + A_Memory() + RSubtract1(A_REG, A_REG);
			if (command == "add")
			{
				cmd += RAdd(D_REG, D_REG, M_REG);
			}
			else if (command == "sub")
			{
				cmd += RSubtract(D_REG, D_REG, M_REG);
			}
			else if (command == "and")
			{
				cmd += RAnd(D_REG, D_REG, M_REG);
			}
			else if (command == "or")
			{
				cmd += ROr(D_REG, D_REG, M_REG);
			}
			else if (command == "gt")
			{
				cmd += RGreaterThan(linecount++);
			}
			else if (command == "lt")
			{
				cmd += RLessThan(linecount++);
			}
			else if (command == "eq")
			{
				cmd += REqual(linecount++);
			}
			else
				std::cout << "Missing command: " << commands.at(0) << "\n";
			cmd += ToStack() + RSubtract1(M_REG, M_REG) + RSubtract1(A_REG, M_REG) + M_Data();
		}
		
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
			else if (location == H_POINTER)
			{
				cmd = ((offset == "0") ? "@3\n" : "@4\n") + D_Memory();
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
					cmd = ToThis(offset);
				}
				else if (location == H_THAT)
				{
					cmd = ToThat(offset);
				}
				else if (location == H_TEMP)
				{
					cmd = ToTemp(stoi(offset));
				}
				else if (location == H_STATIC)
				{
					cmd = ToStatic(offset);
				}
				cmd += D_Memory();
			}

			cmd += StackInto() + StackIncrement();
		}
		else if(commands.at(0) == "pop")
		{
			cmd = StackDecrement() + StackFrom();// + ToTemp(0) + M_Data();

			// Pointer is weird
			if (location == H_POINTER)
			{
				cmd += ((offset == "0") ? "@3\n" : "@4\n") + RAdd(D_REG, D_REG, A_REG);
			}
			else
			{
				// TEMP is a base register of 5 so the D+M trick won't work and needs special treatment
				if (location == H_TEMP)
				{
					cmd += ToTemp() + RAdd(D_REG, D_REG, A_REG);
				}
				else if (location == H_STATIC)
				{
					cmd += ToStatic() + RAdd(D_REG, D_REG, A_REG);
				}
				else
				{
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
					cmd += RAdd(D_REG, D_REG, M_REG);
				}

				cmd += ToAddress(offset) + RAdd(D_REG, D_REG, A_REG);
			}

			cmd += ToStack() + A_Memory() + A_Memory() + RSubtract(A_REG, D_REG, A_REG) + RSubtract(M_REG, D_REG, A_REG);//"A=D-A\nM=D-A\n";
		}
		else
			std::cout << "Missing location: " << commands.at(0) << "\n";
	}

	return cmd;
}

void WriteLineToFile(std::ofstream& file, const std::string& str) { file << str << "\n"; }
int TranslateVMCode(const std::filesystem::path& filepath)
{
	if (filepath.extension() == ".vm")
	{
		if (std::filesystem::exists(filepath))
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
	std::cout << "Incorrect file extension *" << filepath.extension().string() << " must be *.vm\n";
	return EXIT_FAILURE;
}
