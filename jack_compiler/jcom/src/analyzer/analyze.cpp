
#include "analyze.h"

namespace jcom
{

	void AnalyzeFile(std::ifstream& inFile)
	{
		std::string str{ };
		while (std::getline(inFile, str))
		{
			jline line{ str };
			jpair pair{};

			while (line.Next(pair))
			{
				std::cout << gTokenFlags.at(pair.type) << " : " << pair.content << "\n";
			}
		}

	}

	jtok bruh()
	{
		static size_t m{ gTokenFlags.size() - 1 };
		if (m > 0)
			m--;
		return static_cast<jtok>(BIT(m));
	}

}
