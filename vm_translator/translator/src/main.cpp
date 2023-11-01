
#include "translator/trn.h"

#include <iostream>

int main(int argc, char* argv[])
{
#ifdef VMTRANS_DB
	TranslateVMCode("ignores/staticstest");
	//TranslateVMCode("ignores");
#endif
	if (argc >= 2)
	{
		return TranslateVMCode(argv[1]);
	}
	std::cout << "No file supplied in argument\n";
	return EXIT_SUCCESS;
}
