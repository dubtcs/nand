#pragma once

#include <jackc.h>
#include <string>
#include <memory>
#include <unordered_map>

namespace jcom
{

	using token = std::string;

	enum jtok : int32_t
	{
		jtok_keyword = BIT(0),
		jtok_symbol = BIT(1),
		jtok_int = BIT(2),
		jtok_str = BIT(3),
		jtok_id = BIT(4)
	};

	extern std::unordered_map<token, jtok> gReservedTokens;

}
