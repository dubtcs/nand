
#include "tokens.h"

namespace jcom
{

	std::unordered_map<jtok, std::string> gTokenFlags{
		{JackToken::Id, "<id>"},
		{JackToken::Int,"<int>"},
		{JackToken::Keyword, "<keyword>"},
		{JackToken::String, "<string>"},
		{JackToken::Symbol, "<symbol>"}
	};

	std::unordered_map<token, jtok> gReservedTokens{
		// Keywords
		{"class",		JackToken::Keyword},
		{"constructor", JackToken::Keyword},
		{"function",	JackToken::Keyword},
		{"method",		JackToken::Keyword},
		{"field",		JackToken::Keyword},
		{"static",		JackToken::Keyword},
		{"var",			JackToken::Keyword},
		{"int",			JackToken::Keyword},
		{"char",		JackToken::Keyword},
		{"boolean",		JackToken::Keyword},
		{"void",		JackToken::Keyword},
		{"true",		JackToken::Keyword},
		{"false",		JackToken::Keyword},
		{"null",		JackToken::Keyword},
		{"this",		JackToken::Keyword},
		{"let",			JackToken::Keyword},
		{"do",			JackToken::Keyword},
		{"if",			JackToken::Keyword},
		{"else",		JackToken::Keyword},
		{"while",		JackToken::Keyword},
		{"return",		JackToken::Keyword},

		// Symbols
		{"{", JackToken::Symbol},
		{"}", JackToken::Symbol},
		{"(", JackToken::Symbol},
		{")", JackToken::Symbol},
		{"[", JackToken::Symbol},
		{"]", JackToken::Symbol},
		{",", JackToken::Symbol},
		{".", JackToken::Symbol},
		{";", JackToken::Symbol},
		{"+", JackToken::Symbol},
		{"-", JackToken::Symbol},
		{"*", JackToken::Symbol},
		{"/", JackToken::Symbol},
		{"&", JackToken::Symbol},
		{"|", JackToken::Symbol},
		{"<", JackToken::Symbol},
		{">", JackToken::Symbol},
		{"=", JackToken::Symbol},
		{"~", JackToken::Symbol},
	};

}
