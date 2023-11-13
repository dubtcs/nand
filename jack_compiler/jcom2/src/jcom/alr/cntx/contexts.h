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
		IfNested,
		ReturnStatement,
		Expression,
		ExpressionList
	};

	struct jcontext
	{
		token breaker{""}; // token used to mark the end of the context
		std::unordered_map<token, jdesc> contextOverrides;
		bool insideBreakers{ false }; // place the opening tags inside the opener/closer keywords
	};

	using contextmap = std::unordered_map<jdesc, jcontext&>;
	extern contextmap gContexts;

}
