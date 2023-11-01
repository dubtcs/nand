
#include "trn.h"
#include "sform.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include <stack>

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
inline constexpr charp ToThis{ "@3" };
inline constexpr charp ToThat{ "@4" };
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

static std::unordered_map<std::string, int32_t> gFunctions{
	// used to keep track of function return amounts
};

//static std::stack<std::string> gCallStack{}; // this should only ever be size 1 so it could just be a string
static std::string gCurrentFilename{};
static std::string gCurrentFunction{};
static int32_t gRetCount{ 0 };

vmins GeneratePushCommand(const vmins& location, const vmins& offset)
{
	sform cmd{};
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
			if (location != H_POINTER)
			{
				if (offset != "0")
				{
					cmd.Add("@" + offset);
					cmd.Add("D=A");
				}
				else
					cmd.Add("D=0");
				cmd.Add(gToInstructions.at(location)(offset));
				cmd.Add(((location == H_STATIC) ? "A=D+A" : "A=D+M"));
			}
			else
			{
				cmd += gToInstructions.at(location)(offset);
			}
		}
		cmd.Add("D=M");
	}
	cmd.Add(ToStack);
	cmd.Add("A=M");
	cmd.Add("M=D");
	cmd.Add(ToStack);
	cmd.Add("M=M+1");

	return cmd.GetContent();
}
vmins GeneratePopCommand(const vmins& location, const vmins& offset)
{
	sform cmd{};

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

	return cmd.GetContent();
}

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
		if (command != H_RETURN)
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
		else
		{
			// Create temporary endFrame var
			cmd += ToLocal;
			cmd += "D=M";
			cmd += "@endFrame"; // @5
			cmd += "M=D";

			// Create temporay returnAddress var
			cmd += "@5";
			cmd += "A=D-A"; // endFrame - 5, bc we use @5 as temp
			cmd += "D=M";
			cmd += "@rtAd"; // @6
			cmd += "M=D";

			// Pop stack to argument position
			cmd += ToStack;
			cmd += "AM=M-1";
			cmd += "D=M";
			cmd += ToArguments;
			cmd += "A=M";
			cmd += "M=D";

			// Set stack to ARG + 1
			cmd += ToArguments;
			cmd += "D=M+1";
			cmd += ToStack;
			cmd += "M=D";

			// Recover THAT to endFrame - 1 
			cmd += "@endFrame";
			cmd += "A=M-1";
			cmd += "D=M";
			cmd += ToThat;
			cmd += "M=D";

			// Recover THIS to endFrame - 2
			cmd += "@2";
			cmd += "D=A";
			cmd += "@endFrame";
			cmd += "A=M-D";
			cmd += "D=M";
			cmd += ToThis;
			cmd += "M=D";

			// Recover ARG to ednFrame - 3
			cmd += "@3"; // subtract amount
			cmd += "D=A";
			cmd += "@endFrame";
			cmd += "A=M-D";
			cmd += "D=M";
			cmd += ToArguments;
			cmd += "M=D";

			// Recover LCL to endFrame - 4
			cmd += "@4";
			cmd += "D=A";
			cmd += "@endFrame";
			cmd += "A=M-D";
			cmd += "D=M";
			cmd += ToLocal;
			cmd += "M=D";

			// Go to return address
			cmd += "@rtAd";
			cmd += "A=M";
			cmd += "0;JMP";
		}
	}
	else if (commands.size() == 2)
	{
		const vmins& location{ commands.at(1) };
		//vmins label{ gCurrentFunction + "$" + location };
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
			cmd = GeneratePushCommand(location, offset);
		}
		else if (command == H_POP)
		{
			cmd = GeneratePopCommand(location, offset);
		}
		else if (command == H_FUNC)
		{
			// Set current function state
			gCurrentFunction = location;
			gRetCount = 0;
			cmd += "(" + location + ")";
			// Set local variables and push stack
			int32_t lclCount{ stoi(offset) };
			for (int32_t i{ 0 }; i < lclCount; i++)
			{
				cmd += GeneratePushCommand(H_CONST, "0");
			}
		}
		else if (command == H_CALL)
		{
			vmins retLabel{ gCurrentFunction + ".return" + std::to_string(gRetCount++)};

			// Push return address 5
			cmd += "@" + retLabel;
			cmd += "D=A";
			cmd += ToStack;
			cmd += "A=M";
			cmd += "M=D";
			cmd += ToStack;
			cmd += "M=M+1";

			// Push LCL 4
			cmd += ToLocal;
			cmd += "D=M";
			cmd += ToStack;
			cmd += "A=M";
			cmd += "M=D";
			cmd += ToStack;
			cmd += "M=M+1";

			// Push ARG 3
			cmd += ToArguments;
			cmd += "D=M";
			cmd += ToStack;
			cmd += "A=M";
			cmd += "M=D";
			cmd += ToStack;
			cmd += "M=M+1";

			// Push THIS 2
			cmd += ToThis;
			cmd += "D=M";
			cmd += ToStack;
			cmd += "A=M";
			cmd += "M=D";
			cmd += ToStack;
			cmd += "M=M+1";

			// Push THAT 1
			cmd += ToThat;
			cmd += "D=M";
			cmd += ToStack;
			cmd += "A=M";
			cmd += "M=D";
			cmd += ToStack;
			cmd += "M=M+1";

			// Adjust ARG
			int32_t addressOffset{ stoi(offset) + 5 }; // numArgs + 5
			cmd += "@" + (std::to_string(addressOffset)); //  SP - (numArgs + 5)
			cmd += "D=A";
			cmd += ToStack;
			cmd += "D=M-D";
			cmd += ToArguments;
			cmd += "M=D";

			// Adjust LCL
			cmd += ToStack;
			cmd += "D=M";
			cmd += ToLocal;
			cmd += "M=D";

			// Go to function
			cmd += "@" + location;
			cmd += "0;JMP";

			// Add return label
			cmd += "(" + retLabel + ")";
		}
		else
			std::cout << "Command: " << command << " not supported. [" << location << "][" << offset << "]\n";
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

vmins GenerateBootstrapEntry()
{
	sform cmd{};

	// Set SP
	cmd += "@261";
	cmd += "D=A";
	cmd += ToStack;
	cmd += "M=D";

	cmd += "@Sys.init";
	cmd += "0;JMP";

	return cmd.GetContent();
}

// Creates an output file and inserts required bootstrap code
std::ofstream CreateVMOutFile(const std::string& filename)
{
	//std::string outName{ path.filename().replace_extension(".asm").string() };
	std::ofstream outFile{ filename + ".asm" };
	WriteLineToFile(outFile, GenerateBootstrapEntry());
	return outFile;
}

int TranslateVMCode(const std::filesystem::path& filepath)
{
	if (std::filesystem::exists(filepath))
	{
		if (filepath.extension() == ".vm")
		{
			std::ifstream inFile{};
			inFile.open(filepath.string());

			std::ofstream outFile{ CreateVMOutFile(filepath.filename().replace_extension("").string()) };
			gCurrentFilename = filepath.filename().replace_extension("").string();
			TranslateVMFile(filepath, inFile, outFile);
			
			inFile.close();
			outFile.close();
			return EXIT_SUCCESS;
		}
		else if (std::filesystem::is_directory(filepath))
		{
			std::filesystem::path newPath{ filepath.string() + "/" + filepath.filename().string() };
			std::ofstream outFile{ CreateVMOutFile(newPath.string()) };

			WriteLineToFile(outFile, "// Translating directory");
			for (const std::filesystem::path& fi : std::filesystem::recursive_directory_iterator(filepath))
			{
				std::cout << fi.string() << "\n";
				if (fi.extension() == ".vm")
				{
					WriteLineToFile(outFile, "// FILE " + fi.string());
					gCurrentFilename = filepath.filename().replace_extension("").string();
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


/*
vmins lclOffset{ "@" + std::to_string(stoi(offset)) };
			int32_t rNumber{ 0 };
			if (gFunctions.contains(location))
				rNumber = gFunctions.at(location);
			vmins rLabel{ location + ".return" + std::to_string(rNumber++) };

			cmd += "(" + location + ")";
cmd += ToStack;
cmd += "A=M";
cmd += "M=0"; // return address;
cmd += ToStack;
cmd += "M=M+1";

// Push local 
cmd += ToLocal;
cmd += "D=M";
cmd += ToStack;
cmd += "A=M";
cmd += "M=D";
cmd += ToStack;
cmd += "M=M+1";

// Push arg
cmd += ToArguments;
cmd += "D=M";
cmd += ToStack;
cmd += "A=M";
cmd += "M=D";
cmd += ToStack;
cmd += "M=M+1";

// Push this
cmd += ToThis;
cmd += "D=M";
cmd += ToStack;
cmd += "A=M";
cmd += "M=D";
cmd += ToStack;
cmd += "M=M+1";

// Push That
cmd += ToThat;
cmd += "D=M";
cmd += ToStack;
cmd += "A=M";
cmd += "M=D";
cmd += ToStack;
cmd += "M=M+1";

// Set new ARG and LCL
cmd += ToStack;
cmd += "D=M";
cmd += ToLocal;
cmd += "M=D";
cmd += ("@" + offset);
cmd += "D=D-A";
cmd += "@5";
cmd += "D=D-A";
cmd += ToArguments;
cmd += "M=D";

cmd += "(RETURN_ADDRESS)";

gFunctions.at(location) = rNumber;
*/


/*


			//// Push return address
			//cmd += "@" + retLabel;
			//cmd += "D=A";
			//cmd += ToStack;
			//cmd += "A=M";
			//cmd += "M=D";
			//cmd += ToStack;
			//cmd += "M=M+1";

			//// Push LCL
			//cmd += ToLocal;
			//cmd += "D=M";
			//cmd += ToStack;
			//cmd += "A=M";
			//cmd += "M=D";
			//cmd += ToStack;
			//cmd += "M=M+1";

			//// Push ARG
			//cmd += ToArguments;
			//cmd += "D=M";
			//cmd += ToStack;
			//cmd += "A=M";
			//cmd += "M=D";
			//cmd += ToStack;
			//cmd += "M=M+1";

			//// Push THIS
			//cmd += ToThis;
			//cmd += "D=M";
			//cmd += ToStack;
			//cmd += "A=M";
			//cmd += "M=D";
			//cmd += ToStack;
			//cmd += "M=M+1";

			//// Push THAT
			//cmd += ToThat;
			//cmd += "D=M";
			//cmd += ToStack;
			//cmd += "A=M";
			//cmd += "M=D";
			//cmd += ToStack;
			//cmd += "M=M+1";

			//// Move ARG
			//cmd += ToStack;
			//cmd += "D=M";
			//cmd += "@5";
			//cmd += "D=D-A";
			//cmd += ToArguments;
			//cmd += "M=D";

			//// Set LCL
			//cmd += ToStack;
			//cmd += "D=M";
			//cmd += ToLocal;
			//cmd += "M=D";

			//// GOTO
			//cmd += "@" + location;
			//cmd += "0;JMP";

			//cmd += "(" + retLabel + ")";

*/


/*

// recover state
			cmd += ToLocal;
			cmd += "D=M";
			cmd += "@5"; //  endFrame
			cmd += "M=D";

			// retAddress = endFrame - 5;
			cmd += "D=D-A";
			cmd += "@6"; // retAddress
			cmd += "M=D";

			// ARG = pop stack
			cmd += ToStack;
			cmd += "AM=M-1";
			cmd += "D=M";
			cmd += ToArguments;
			cmd += "A=M";
			cmd += "M=D";

			// SP = ARG + 1
			cmd += "D=A+1";
			cmd += ToStack;
			cmd += "M=D";

			// THAT = (endFrame - 1)
			cmd += "@5"; // endFrame
			cmd += "A=M-1";
			cmd += "D=M";
			cmd += ToThat;
			cmd += "M=D";

			// THIS = (endFrame - 2)
			cmd += "@5"; // endFrame
			cmd += "D=M";
			cmd += "@2";
			cmd += "A=D-A";
			cmd += "D=M";
			cmd += ToThis;
			cmd += "M=D";

			// ARG = (endFrame - 3)
			cmd += "@5"; // endFrame
			cmd += "D=M";
			cmd += "@3";
			cmd += "A=D-A";
			cmd += "D=M";
			cmd += ToArguments;
			cmd += "M=D";

			// LCL = (endFrame - 4)
			cmd += "@5"; // endFrame
			cmd += "D=M";
			cmd += "@4";
			cmd += "A=D-A";
			cmd += "D=M";
			cmd += ToLocal;
			cmd += "M=D";

			// goto retAddress
			cmd += "@6"; // retAddress
			cmd += "A=M";
			cmd += "0;JMP";


*/