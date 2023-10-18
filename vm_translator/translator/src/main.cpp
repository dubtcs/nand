
#include "translator/translator.h"

#include <iostream>

int main(int argc, char* argv[])
{
	TranslateVMCode("BasicTest.vm");
	if (argc == 2)
	{
		return TranslateVMCode(argv[1]);
	}
	return EXIT_FAILURE;
}
