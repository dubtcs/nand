
#include "translator/translator.h"

#include <iostream>

int main(int argc, char* argv[])
{
#ifdef VMTRANS_DB
	TranslateVMCode("BasicTest.vm");
#endif
	if (argc == 2)
	{
		return TranslateVMCode(argv[1]);
	}
	std::cout << "No file supplied in argument\n";
	return EXIT_SUCCESS;
}
