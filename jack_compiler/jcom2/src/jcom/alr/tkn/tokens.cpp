
#include "tokens.h"

namespace jcom
{

	// replace <> with &gt; &lt; &quot; &amp;
	std::unordered_map<jtok, Tag> gTokenFlags{
		{JackToken::Id, {"<identifier>", "</identifier>"} },
		{JackToken::Int,{"<integerConstant>", "</integerConstant>"}},
		{JackToken::Keyword, {"<keyword>", "</keyword>"}},
		{JackToken::String, {"<stringConstant>", "</stringConstant>"}},
		{JackToken::Symbol, {"<symbol>", "</symbol>"}}
	};

	std::unordered_set<token> gComplexKeywords
	{
		"class", "constructor", "function", "method", "field", "static", "var", "let", "do", "if", "while", "return"
	};

	std::unordered_map<token, jtok> gKeywords{
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
