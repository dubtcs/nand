
#include "analyze.h"
#include "token/jfile.h"

namespace jcom
{

	static size_t gCurrentTree{ 0 };
	static std::string gTreePrefix{};

	void WriteLine(std::ofstream& file, const std::string& str)
	{
		file << str << "\n";
	}

	std::string JTreeFormat(const std::string& str)
	{
		return gTreePrefix + str;
	}

	void IncTreeString()
	{
		gTreePrefix += '\t';
	}

	void DecTreeString()
	{
		gTreePrefix.pop_back();
	}

	void AnalyzeTokens(std::ofstream& outFile, jfile& file)
	{
		jpair pair{};
		while (file.NextToken(pair))
		{
			WriteLine(outFile, JTreeFormat(gTokenFlags.at(pair.type).opener));
			if (gReservedTokens.contains(pair.content))
			{
				IncTreeString();
				AnalyzeTokens(outFile, file);
				DecTreeString();
			}
			else
			{
				WriteLine(outFile, JTreeFormat(pair.content));
			}
			WriteLine(outFile, JTreeFormat(gTokenFlags.at(pair.type).closer));
		}
	}

	void AnalyzeFile(std::ifstream& inFile, std::ofstream& outFile)
	{
		WriteLine(outFile, "<tokens>");
		IncTreeString();

		std::string str{ };

		jfile file{ inFile };
		jpair pair{};

		AnalyzeTokens(outFile, file);

		DecTreeString();
		WriteLine(outFile, "</tokens>");

	}

}
