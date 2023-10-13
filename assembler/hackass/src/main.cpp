
#include "nand/assembler.h"

#include <iostream>

int main(int argc, char* argv[])
{
	NANDHackAssembleFile("add/add.asm");
	/*if (argc < 2)
	{
		std::cout << "Missing file argument.\n";
		return EXIT_FAILURE;
	}*/
	//return NANDHackAssembleFile(argv[1]);
}
