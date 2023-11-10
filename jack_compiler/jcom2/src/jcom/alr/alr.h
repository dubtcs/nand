#pragma once

#include <jcom/jcom.h>

#include "cntx/contexts.h"
#include "file/jfile.h"

#include <string>
#include <stack>

namespace jcom
{

	class jalr
	{
	public:
		jalr(std::ifstream& inFile, std::ofstream& outFile);
	protected:
		void ParseToken();

		void IncTree(jdesc descriptor);
		void DecTree();

		void WriteToken();
		void WriteLine(const std::string& str);
	protected:
		std::ifstream& mInFile;
		std::ofstream& mOutFile;
		jfile mFile;
		std::stack<jdesc> mStack;
		std::string mTreeString{};
	};

}
