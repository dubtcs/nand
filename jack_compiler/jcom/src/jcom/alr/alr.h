#pragma once

#include <jcom/jcom.h>

#include "cntx/contexts.h"
#include "file/jfile.h"
#include <jcom/cmp/st/smta.h>

#include <string>
#include <stack>
#include <queue>
#include <array>

namespace jcom
{
	class jalr;

	using tokenset = std::unordered_set<token>;
	using entrymap = std::unordered_map<jdesc, tokenset>;

	// Entrypoint map
	using emap = std::unordered_map<token, jdesc>;
	// Context map
	using cmap = std::unordered_map<jdesc, emap>;

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
		void ParseSubroutineCall(token prefix = ""); // soft rule, no headers

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

		void PushTry(const token& varName);
		void Push(const token& varName, int32_t tableIndex);
		void PushConstant(const token& varName);
		void Pop(const token& var);

		void Label(const token& label);

		void WriteCommand(const std::string& cmd);
		void WriteToken();
		void WriteTokenNext();
		void WriteLine(const std::string& str);
	protected:
		std::ifstream& mInFile;
		std::ofstream& mOutFile;
		jfile mFile;
		token mClassContext{};
		int32_t mCurrentTable{ 0 };
		std::array<symtable, 2> mTables{};
		static const cmap mContexts;
	};

}
