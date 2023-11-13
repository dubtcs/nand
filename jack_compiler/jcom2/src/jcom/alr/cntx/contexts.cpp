
#include "contexts.h"

namespace jcom
{

	jcontext jClassContext
	{
		"}",
		{}
	};

	jcontext jVarContext
	{
		";", {}
	};

	jcontext jSubroutineContext
	{
		"}",
		{ // conditional contexts
			{ "if",		jdesc::Statement },
			{ "let",	jdesc::Statement },
			{ "do",		jdesc::Statement },
			{ "while",	jdesc::Statement },
			{ "return", jdesc::Statement },
			{ "(",		jdesc::ParameterList}
		}
	};

	jcontext jParameterListContext
	{
		")", {}, true
	};

	jcontext jStatementContext
	{
		"}", {}
	};

	jcontext jIfStatementContext
	{
		"}",
		{
			{"{", jdesc::IfNested}
		},
	};

	jcontext jIfNestedContext
	{
		"}",
		{

		},
		true
	};

	jcontext jReturnContext
	{
		";", {}
	};

	jcontext jExpressionContext
	{
		";", {}
	};

	jcontext jNoneContext{};

	contextmap gContexts
	{
		{ jdesc::None,				jNoneContext},
		{ jdesc::Class,				jClassContext },
		{ jdesc::ClassVarDec,		jVarContext },
		{ jdesc::VarDec,			jVarContext },
		{ jdesc::Subroutine,		jSubroutineContext },
		{ jdesc::ParameterList,		jParameterListContext },
		{ jdesc::SubroutineBody,	jStatementContext },

		{ jdesc::Statement,			jStatementContext },
		{ jdesc::IfStatement,		jIfStatementContext },
		{ jdesc::IfNested,			jIfNestedContext },	
		{ jdesc::WhileStatement ,	jStatementContext },
		{ jdesc::DoStatement,		jReturnContext },
		{ jdesc::LetStatement,		jReturnContext },
		{ jdesc::ReturnStatement,	jReturnContext },

		{ jdesc::Expression,		jExpressionContext },
		{ jdesc::ExpressionList,	jExpressionContext },
	};

}
