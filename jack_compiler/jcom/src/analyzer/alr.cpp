
#include "alr.h"

#include <unordered_map>
#include <unordered_set>

namespace jcom
{

	static std::unordered_map<token, std::unordered_map<jdesc, jdesc>> gConditionalDestinations
	{
		{ "(", { { jdesc::Class, jdesc::ParameterList }, { jdesc::Statement, jdesc::ExpressionList } } }
		
	};

	jalr::jalr(std::ifstream& inFile, std::ofstream& outFile) :
		mInFile{ inFile },
		mOutFile{ outFile },
		mFile{ inFile }
	{

	}

}
