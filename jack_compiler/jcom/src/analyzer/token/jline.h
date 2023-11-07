#pragma once

#include "tokens.h"
#include <string>

namespace jcom
{

	struct jpair
	{
		jtok type{ JackToken::None };
		token content{};
	};

	class jline
	{
	public:
		jline() = default;
		jline(const std::string& line);
	public:
		bool Next(jpair& target);
	protected:
		size_t mCursor{ 0 };
		const std::string& mContent;
		friend class jfile;
	};

}
