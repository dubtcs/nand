#pragma once

#include <jackc.h>
#include <string>
#include <memory>
#include <unordered_map>

namespace jcom
{

	using token = std::string;

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

	extern std::unordered_map<JackToken, std::string> gTokenFlags;
	extern std::unordered_map<token, JackToken> gReservedTokens;

}
