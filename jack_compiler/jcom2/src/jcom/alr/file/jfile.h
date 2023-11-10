#pragma once

#include <jcom/jcom.h>

#include "../tkn/tokens.h"

#include <fstream>

namespace jcom
{

	struct jpair
	{
		token content;
		jtok type{ JackToken::None };
	};

	// Wrapper for .jack files to search for each available token
	class jfile
	{
	public:
		jfile(std::ifstream& inFile);
	public:
		bool Available();
		bool Next();
		jpair& Get();
	protected:
		bool CheckContent();
	protected:
		size_t mCursor{ 0 };
		jpair mCurrent{};
		std::ifstream& mFile;
		std::string mContent{};
	};

}
