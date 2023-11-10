#pragma once

#include <jcom/jcom.h>

#include <unordered_map>

namespace jcom
{

	enum class jdesc
	{
		None,
		Class,
		ParameterList,
		ClassVarDec,
		Subroutine,
		SubroutineBody,
		VarDec,
		Statement,
		DoStatement,
		LetStatement,
		WhileStatement,
		IfStatement,
		ReturnStatement,
		Expression,
		ExpressionList
	};

	struct jcontext
	{
		token breaker{""};
		std::unordered_map<token, jdesc> contextOverrides;
		bool saveBreaker{ false };
	};

	using contextmap = std::unordered_map<jdesc, jcontext&>;
	extern contextmap gContexts;

}
