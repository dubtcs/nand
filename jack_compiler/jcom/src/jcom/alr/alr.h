#pragma once

#include <jcom/jcom.h>

#include "cntx/contexts.h"
#include "file/jfile.h"
#include <jcom/cmp/st/smta.h>

#include <string>
#include <stack>

namespace jcom
{
	class jalr;

	using tokenset = std::unordered_set<token>;
	using entrymap = std::unordered_map<jdesc, tokenset>;

	// Entrypoint map
	using emap = std::unordered_map<token, jdesc>;
	// Context map
	using cmap = std::unordered_map<jdesc, emap>;

	// could use function pointers
	//using emap2 = std::unordered_map<token, void(jalr::*)()>;

	class jalr
	{
	public:
		jalr(std::ifstream& inFile, std::ofstream& outFile);
	protected:
		void ParseToken();

		void ParseClass();
		void ParseClassVar();
		void ParseSubroutine();
		void ParseParameterList();
		void ParseSubroutineBody();
		void ParseSubroutineVar();
		void ParseSubroutineCall(); // soft rule, no headers

		void ParseStatements();
		void ParseLetStatement();
		void ParseDoStatement();
		void ParseReturnStatement();
		void ParseIfStatement();
		void ParseWhileStatement();

		void ParseExpressionList();
		void ParseExpression();
		void ParseTerm();

		bool ContextContains(entrymap& entries, jdesc context);
		jdesc GetEntrypoint(jdesc context);

		void WriteCommand(const std::string& cmd);
		void WriteToken();
		void WriteTokenNext();
		void WriteLine(const std::string& str);
	protected:
		std::ifstream& mInFile;
		std::ofstream& mOutFile;
		jfile mFile;
		static const cmap mContexts;
		symtable mClassTable{};
		symtable mRoutineTable{};
	};

}
