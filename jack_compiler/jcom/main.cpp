
#include <jentry.h>
#include <iostream>

int main(int argc, char* argv[])
{
#ifdef JCOM_DEBUG
	jcom::CompileJack("ignores/a2");
#endif
	if (argc > 2)
		return jcom::CompileJack(argv[1]);
	return EXIT_FAILURE;
}
