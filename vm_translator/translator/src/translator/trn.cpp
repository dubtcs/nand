
#include "trn.h"
#include "sform.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>

//
// Hack memory layout
// 0			: Stack Pointer,	@SP
// 1			: Local Pointer,	@LCL
// 2			: Argument Pointer, @ARG
// 3			: This Pointer,		@THIS
// 4			: That Pointer,		@THAT
// 5 - 12		: Temp Sotrage
// 13 - 15		: General Registers
// 16 - 255		: Statiuc Variables
// 256 - 2017	: Stack
//

//
// Project 08
// Loops
// Functions
// Branching
//

using charp = const char*;
inline constexpr charp ToStack{ "@SP" };
inline constexpr charp ToLocal{ "@LCL" };
inline constexpr charp ToArguments{ "@ARG" };
inline constexpr charp ToThis{ "@THIS" };
inline constexpr charp ToThat{ "@THAT" };
inline constexpr charp ToTemp{ "@5" };
inline constexpr charp ToStatic{ "@16" };

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

static const std::unordered_map<std::string, std::function<vmins(const vmins& offset)>> gToInstructions{
	{ H_CONST,	[](const vmins& offset) { return ("@" + offset); }},
	{ H_LOCAL,	[](const vmins& offset) { return ToLocal; }},
	{ H_TEMP,	[](const vmins& offset) { return ToTemp; }},
	{ H_POINTER,[](const vmins& offset) { return ((offset == "0") ? "@3" : "@4"); }},
	{ H_THIS,	[](const vmins& offset) { return ToThis; }},
	{ H_ARGS,	[](const vmins& offset) { return ToArguments; }},
	{ H_THAT,	[](const vmins& offset) { return ToThat; }},
	{ H_STATIC,	[](const vmins& offset) { return ToStatic; }}
};

static const std::unordered_map<std::string, std::function<vmins(const vmins& offset)>> gPopSetDDA{

};

vmcmds GetCommandSet(std::string& line)
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
			if (!currentCommand.empty())
			{
				commands.push_back(currentCommand);
			}
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

std::string BuildAssembly(vmcmds& commands)
{
	const vmins& command{ commands.at(0) };
	sform cmd{};
	if (commands.size() == 1)
	{
		cmd.Add(ToStack);
		cmd.Add("A=M-1");
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
			cmd.Add("A=M-1");
			cmd.Add(gCommandInstructions.at(command)());
		}
		cmd.Add(ToStack);
		cmd.Add(((command == "not") || (command == "neg")) ? "A=M" : "AM=M-1");
		cmd.Add("A=A-1");
		cmd.Add("M=D");
	}
	else if (commands.size() == 2)
	{
		const vmins& location{ commands.at(1) };
		if (command == H_LABEL)
		{
			cmd += "(" + location + ")";
		}
		else if (command == H_GOTO)
		{
			cmd += "@" + location;
			cmd += "0;JMP";
		}
		else if (command == H_IFGO)
		{
			cmd += ToStack;
			cmd += "AM=M-1";
			cmd += "D=M";
			cmd += "@" + location;
			cmd += "D;JNE";
		}
	}
	else
	{
		const vmins& location{ commands.at(1) };
		const vmins& offset{ commands.at(2) }; 
		if (command == H_PUSH)
		{
			if (location == H_CONST)
			{
				cmd.Add("@" + offset);
				cmd.Add("D=A");
			}
			else
			{
				if (location == H_TEMP)
				{
					cmd.Add("@" + std::to_string(5 + stoi(offset)));
				}
				else
				{
					if (offset != "0")
					{
						cmd.Add("@" + offset);
						cmd.Add("D=A");
					}
					else
						cmd.Add("D=0");
					cmd.Add(gToInstructions.at(location)(offset));
					cmd.Add(((location == H_STATIC) ? "A=D+A" : "A=M+D"));
				}
				cmd.Add("D=M");
			}
			cmd.Add(ToStack);
			cmd.Add("A=M");
			cmd.Add("M=D");
			cmd.Add(ToStack);
			cmd.Add("M=M+1");
		}
		else if (command == H_POP)
		{
			cmd.Add(ToStack);
			cmd.Add("AM=M-1");
			cmd.Add("D=M");
			cmd.Add(gToInstructions.at(location)(offset));
			if (location != H_POINTER)
			{
				if (location == H_TEMP || location == H_STATIC)
					cmd.Add("D=D+A");
				else
					cmd.Add("D=D+M");
				cmd.Add("@" + offset);
			}
			cmd.Add("D=D+A");
			cmd.Add(ToStack);
			cmd.Add("A=M");
			cmd.Add("A=M");
			cmd.Add("A=D-A");
			cmd.Add("M=D-A");
		}
		else
			std::cout << "Command: " << command << " not supported. [" << location << "]["<<offset<<"]\n";
	}

	return cmd.GetContent();
}

void WriteLineToFile(std::ofstream& file, const std::string& str) 
{ 
	file << str << "\n"; 
}

int TranslateVMFile(const std::filesystem::path& filepath, std::ifstream& inFile, std::ofstream& outFile)
{
	std::string line{};
	while (std::getline(inFile, line))
	{
		vmcmds commands{ GetCommandSet(line) };
		if (!commands.empty())
		{
			WriteLineToFile(outFile, ("// " + line));
			std::string assembly{ BuildAssembly(commands) };
			WriteLineToFile(outFile, assembly);
		}
	}
	return EXIT_SUCCESS;
}

int TranslateVMCode(const std::filesystem::path& filepath)
{
	if (std::filesystem::exists(filepath))
	{
		if (filepath.extension() == ".vm")
		{
			std::ifstream inFile{};
			inFile.open(filepath.string());

			std::string outName{ filepath.filename().replace_extension(".asm").string() };
			std::ofstream outFile{ outName };

			TranslateVMFile(filepath, inFile, outFile);
			
			inFile.close();
			outFile.close();
			return EXIT_SUCCESS;
		}
		else if (std::filesystem::is_directory(filepath))
		{
			std::string outName{ filepath.filename().replace_extension(".asm").string() };
			std::ofstream outFile{ outName };

			WriteLineToFile(outFile, "// Translating directory");
			for (const std::filesystem::path& fi : std::filesystem::recursive_directory_iterator(filepath))
			{
				std::cout << fi.string() << "\n";
				if (fi.extension() == ".vm")
				{
					WriteLineToFile(outFile, "// FILE " + fi.string());
					std::ifstream inFile{};
					inFile.open(fi.string());
					TranslateVMFile(fi, inFile, outFile);
					inFile.close();
				}
			}

			outFile.close();
			return EXIT_SUCCESS;
		}
	}
	std::cout << "Incorrect file extension *" << filepath.extension().string() << " must be *.vm\n";
	return EXIT_FAILURE;
}
