#pragma once

#include "tokens.h"
#include "jline.h"

#include <jackc.h>
#include <fstream>

namespace jcom
{
	// Wrapper for .jack files to search for each available token
	class jfile
	{
	public:
		jfile(std::ifstream& inFile);
	public:
		// Get the next available token in the file.
		// @param pair - Struct to insert data into
		// @return if another token is available
		bool NextToken(jpair& pair);
		jpair GetCurrent();
	protected:
		bool CheckContent();
	protected:
		size_t mCursor{ 0 };
		jpair mCurrent{};
		std::ifstream& mFile;
		std::string mContent{};
	};

}
