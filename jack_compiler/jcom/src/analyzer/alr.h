#pragma once

#include <jackc.h>

#include "token/jfile.h"

#include <string>
#include <stack>

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
		WhileStatement,
		IfStatement,
		ReturnStatement,
		Expression,
		ExpressionList
	};

	class jalr
	{
	public:
		jalr(std::ifstream& inFile, std::ofstream& outFile);
	protected:
		void ParseToken();
		void ParseClass();
		void ParseClassVar();
		void ParseSubroutine();
		void ParseParameters();

		void IncTree();
		void DecTree();
		void Header(const std::string& name);
		void Footer(const std::string& name);

		void WriteToken();
		void WriteLine(const std::string& str);
	protected:
		std::ifstream& mInFile;
		std::ofstream& mOutFile;
		jfile mFile;
		std::stack<jdesc> mStack;
		std::string mTreeString{};
	};

}
