#pragma once

#include <jackc.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace jcom
{

	using token = std::string;

	struct Tag
	{
		const std::string opener;
		const std::string closer;
	};

	enum class JackToken
	{
		None,
		Keyword,
		Symbol,
		Int,
		String,
		Id
	};
	using jtok = JackToken;

	extern std::unordered_map<JackToken, Tag> gTokenFlags; // termonal elements
	extern std::unordered_map<token, JackToken> gReservedTokens;

	extern std::unordered_set<token> gComplexKeywords;

}
