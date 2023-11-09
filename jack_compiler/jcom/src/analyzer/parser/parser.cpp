
#include "parser.h"

namespace jcom
{

	std::unordered_map<token, std::function<void()>> gNonTerminalExpressions
	{
		{ "class", ParseClass }
	};

	void ParseClass()
	{

	}

}
