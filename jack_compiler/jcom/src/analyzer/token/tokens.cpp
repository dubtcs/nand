
#include "tokens.h"

namespace jcom
{

	std::unordered_map<token, jtok> gReservedTokens{
		// Keywords
		{"class", jtok_keyword},
		{"constructor", jtok_keyword},

		// Symbols
		{"{", jtok_symbol},
		{"}", jtok_symbol},
		{"(", jtok_symbol},
		{")", jtok_symbol},
		{"[", jtok_symbol},
		{"]", jtok_symbol},
		{",", jtok_symbol},
		{".", jtok_symbol},
		{";", jtok_symbol},
		{"+", jtok_symbol},
		{"-", jtok_symbol},
		{"*", jtok_symbol},
		{"/", jtok_symbol},
		{"&", jtok_symbol},
		{"|", jtok_symbol},
		{"<", jtok_symbol},
		{">", jtok_symbol},
		{"=", jtok_symbol},
		{"~", jtok_symbol},

	};

}
