
#include "alr.h"

#include <unordered_map>
#include <unordered_set>

#include <iostream>

namespace jcom
{

	static std::unordered_map<token, jdesc> gKeywordDescs
	{
		{"class",		jdesc::Class},
		{"field",		jdesc::ClassVarDec},
		{"static",		jdesc::ClassVarDec},
		{"function",	jdesc::Subroutine},
		{"method",		jdesc::Subroutine},
		{"contructor",	jdesc::Subroutine},
		{"if",			jdesc::IfStatement},
		{"return",		jdesc::ReturnStatement},
		{"while",		jdesc::WhileStatement},
		{"let",			jdesc::LetStatement},
		{"do",			jdesc::DoStatement},
		{"var",			jdesc::VarDec}
		//{"=",			jdesc::Expression},
	};

	static std::unordered_map<jdesc, token> gDescTokens
	{
		{jdesc::Class, "class"},
		{jdesc::ClassVarDec, "classVarDec"},
		{jdesc::Subroutine, "subroutineDec"},
		{jdesc::SubroutineBody, "subroutineBody"},
		{jdesc::Statement, "statements"},
		{jdesc::ReturnStatement, "returnStatement"},
		{jdesc::IfStatement, "ifStatement"},
		{jdesc::WhileStatement, "whileStatement"},
		{jdesc::DoStatement, "doStatement"},
		{jdesc::LetStatement, "letStatement"},
		{jdesc::VarDec, "varDec"},
		{jdesc::Expression, "expression"},
	};

	jalr::jalr(std::ifstream& inFile, std::ofstream& outFile) :
		mInFile{ inFile },
		mOutFile{ outFile },
		mFile{ inFile }
	{
		mStack.push(jdesc::None);
		ParseToken();
	}

	void jalr::WriteLine(const std::string& str)
	{
		mOutFile << str << "\n";
	}

	void jalr::WriteToken()
	{
		jpair& cur{ mFile.Get() };
		WriteLine( mTreeString + gTokenFlags.at(cur.type).opener + cur.content + gTokenFlags.at(cur.type).closer );
	}

	void jalr::IncTree(jdesc descriptor)
	{
		mStack.push(descriptor);
		WriteLine(mTreeString + "<" + gDescTokens.at(mStack.top()) + ">");
		mTreeString += '\t';
	}

	void jalr::DecTree()
	{
		mTreeString.pop_back();
		WriteLine(mTreeString + "</" + gDescTokens.at(mStack.top()) + ">");
		mStack.pop();
	}

	void jalr::ParseToken()
	{
		int32_t j{ 0 };
		while (mFile.Available())
		{
			jcontext context{ gContexts.at(mStack.top()) };
			jpair pair{ mFile.Get() };
			token tk{ pair.content };

			if (context.contextOverrides.contains(tk)) // check for generic container keyword
			{
				IncTree(context.contextOverrides.at(tk));
				context = gContexts.at(context.contextOverrides.at(tk));
			}

			if (gKeywordDescs.contains(tk))
				IncTree(gKeywordDescs.at(tk));

			if (mStack.top() == jdesc::Statement)
				if (tk == context.breaker)
					DecTree();
			WriteToken();

			if (tk == context.breaker)
			{
				if (mStack.top() == jdesc::IfStatement) // maybe add context continues? IfStatement might be the only weird one here
				{
					const token& tk2{ mFile.PeekNext().content };
					if (tk2 != "else")
						DecTree();
					else
						WriteToken();
				}
				else
					DecTree();
			}
			mFile.Next();

		}
	}

}
