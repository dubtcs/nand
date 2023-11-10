
#include "alr.h"

#include <unordered_map>
#include <unordered_set>

#include <iostream>

namespace jcom
{

	static std::unordered_map<token, std::unordered_map<jdesc, jdesc>> gConditionalDestinations
	{
		{ "(", { { jdesc::Class, jdesc::ParameterList }, { jdesc::Statement, jdesc::ExpressionList }, { jdesc::Expression, jdesc::ExpressionList } } }
	};

	static std::unordered_map<token, jdesc> gKeywordDescs
	{
		{"class",		jdesc::Class},
		{"field",		jdesc::ClassVarDec},
		{"function",	jdesc::Subroutine},
		{"method",		jdesc::Subroutine},
		{"contructor",	jdesc::Subroutine},
		{"if",			jdesc::Statement},
		{"return",		jdesc::Statement},
		{"while",		jdesc::Statement},
		{"let",			jdesc::Statement},
		{"do",			jdesc::Statement},
		{"var",			jdesc::VarDec},
		{"=",			jdesc::Expression},
	};

	static std::unordered_map<jdesc, token> gDescTokens
	{
		{jdesc::Class, "class"},
		{jdesc::ClassVarDec, "classVarDec"},
		{jdesc::Subroutine, "subroutineDec"},
		{jdesc::SubroutineBody, "subroutineBody"},
		{jdesc::Statement, "statement"},
		{jdesc::VarDec, "varDec"},
		{jdesc::Expression, "expression"},
	};

	jalr::jalr(std::ifstream& inFile, std::ofstream& outFile) :
		mInFile{ inFile },
		mOutFile{ outFile },
		mFile{ inFile }
	{
		ParseToken();
	}

	void jalr::WriteLine(const std::string& str)
	{
		mOutFile << str << "\n";
	}

	void jalr::WriteToken()
	{
		jpair cur{ mFile.GetCurrent() };
		WriteLine( gTokenFlags.at(cur.type).opener + cur.content + gTokenFlags.at(cur.type).closer );
	}

	void jalr::IncTree()
	{
		mTreeString += '\t';
	}

	void jalr::DecTree()
	{
		mTreeString.pop_back();
	}

	void jalr::ParseToken()
	{
		jpair pair{};
		bool end{ false };
		while ((!end) && (mFile.NextToken(pair) && (!mInFile.eof())))
		{
			const token& tk{ pair.content };
			if (gKeywordDescs.contains(tk))
			{
				mStack.push(gKeywordDescs.at(tk));
				IncTree();
				WriteToken();
			}
		}
	}

}
