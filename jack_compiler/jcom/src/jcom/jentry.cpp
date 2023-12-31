
#include "jentry.h"
#include <iostream>

static inline constexpr const char* FILE_EXTENSION{ "vm" };

namespace jcom
{
	int CompileFile(const std::filesystem::path& path)
	{
		std::cout << "Compiling: " << path.string() << '\n';
		if (path.extension() == JACK_EXTENSION)
		{
			std::ifstream inFile{ path.string() };
			std::string newName{ '\\' + path.filename().replace_extension(FILE_EXTENSION).string() };
			std::ofstream outFile{ path.parent_path().string() + newName };

			jalr anl{ inFile, outFile };

			inFile.close();
			outFile.close();
			return EXIT_SUCCESS;
		}
		return EXIT_FAILURE;
	}

	int CompileJack(const std::filesystem::path& path)
	{
		if (std::filesystem::exists(path))
		{
			if (std::filesystem::is_regular_file(path))
			{
				return CompileFile(path);
			}
			else if (std::filesystem::is_directory(path))
			{
				bool fail{ false };
				for (const std::filesystem::path& it : std::filesystem::recursive_directory_iterator(path))
				{
					fail = (!CompileFile(it) || fail);
				}
				return fail;
			}
		}
		return EXIT_FAILURE;
	}
}
