
#include "trn.h"
#include "sform.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>

inline constexpr std::string ToStack{ "@SP" };
inline constexpr std::string ToLocal{ "@LCL" };
inline constexpr std::string ToArguments{ "@ARG" };
inline constexpr std::string ToThis{ "@THIS" };
inline constexpr std::string ToThat{ "@THAT" };
inline constexpr std::string ToTemp{ "@5" };
inline constexpr std::string ToStatic{ "@16" };

static const std::unordered_map<std::string, std::function<vmins()>> gCommandInstructions{
	{"add", []() { return "D=D+M"; }},
	{"sub", []() { return "D=D-M"; }},
	{"and", []() { return "D=D&M"; }},
	{"or",  []() { return "D=D|M"; }},
	{"neg", []() { return "D=-D";  }},
	{"not", []() { return "D=!D";  }},
	{"gt",  vmtr::GreaterThan },
	{"lt",  vmtr::LessThan },
	{"eq",  vmtr::Equal }
};

vmcmds GetCommandSet2(std::string& line)
{
	vmcmds commands{};

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

std::string BuildAssembly2(vmcmds& commands)
{
	const vmins& command{ commands.at(0) };
	const vmins& location{ commands.at(1) };
	const vmins& offset{ commands.at(2) };
	sform cmd{};
	if (commands.size() == 1)
	{
		cmd.Add(ToStack);
		cmd.Add("A=M");
		cmd.Add("A=A-1");
		if ((command == "not") || (command == "neg"))
		{
			cmd.Add("D=M");
			cmd.Add((command == "not") ? "D=!D" : "D=-D");
			cmd.Add("M=D");
		}
		else
		{
			cmd.Add("A=A-1");
			cmd.Add("D=M");
			cmd.Add(ToStack);
			cmd.Add("A=M");
			cmd.Add("A=A-1");
			cmd.Add(gCommandInstructions.at(command)());
			/*if (command == "add")
				cmd.Add("D=D+M");
			else if (command == "sub")
				cmd.Add("D=D-M");
			else if (command == "and")
				cmd.Add("D=D&M");
			else if (command == "or")
				cmd.Add("D=D|M");
			else if (command == "gt")
				cmd.Add(vmtr::GreaterThan());
			else if (command == "lt")
				cmd.Add(vmtr::LessThan());
			else if (command == "eq")
				cmd.Add(vmtr::Equal());*/
		}
		cmd.Add(ToStack);
		cmd.Add("M=M-1");
		cmd.Add("A=M-1");
		cmd.Add("M=D");
	}
	else
	{
		
	}

	return cmd.GetContent();
}

void WriteLineToFile2(std::ofstream& file, const std::string& str) 
{ 
	file << str << "\n"; 
}

int TranslateVMCode2(const std::filesystem::path& filepath)
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
				vmcmds commands{ GetCommandSet2(line) };
				if (!commands.empty())
				{
					WriteLineToFile2(outFile, ("// " + line));
					std::string assembly{ BuildAssembly2(commands) };
					WriteLineToFile2(outFile, assembly);
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
