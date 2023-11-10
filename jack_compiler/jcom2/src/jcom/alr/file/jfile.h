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
		const jpair& PeekNext();
		jpair& Get();
	protected:
		void FillPair(jpair& pair);
		bool CheckContent();
	protected:
		size_t mCursor{ 0 };
		jpair mCurrent{};
		jpair mNext{};
		std::ifstream& mFile;
		std::string mContent{};
	};

}
