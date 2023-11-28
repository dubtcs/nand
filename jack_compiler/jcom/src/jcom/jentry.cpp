
#include "jentry.h"

namespace jcom
{
	int CompileFile(const std::filesystem::path& path)
	{
		if (path.extension() == JACK_EXTENSION)
		{
			std::ifstream inFile{ path.string() };
			std::string newName{ '\\' + path.filename().replace_extension("txt").string() };
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
					fail = (fail || !CompileFile(it));
				}
				return fail;
			}
		}
		return EXIT_FAILURE;
	}
}
