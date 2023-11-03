#pragma once

#include "tokens.h"
#include <string>

namespace jcom
{

	class jline
	{
	public:
		jline(const std::string& line);
	public:
		bool Next(token& target);
	protected:
		size_t mCursor{ 0 };
		const std::string& mContent;
	};

}
