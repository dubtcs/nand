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
		int32_t ParseParameterList();
		uint8_t ParseSubroutineBody();
		uint8_t ParseSubroutineVar();
		void ParseSubroutineCall(token prefix = ""); // soft rule, no headers

		void ParseStatements();
		void ParseLetStatement();
		void ParseDoStatement();
		void ParseReturnStatement();
		void ParseIfStatement();
		void ParseWhileStatement();

		void ParseArrayIndex();

		int32_t ParseExpressionList();
		int32_t ParseExpression();
		int32_t ParseTerm();

		bool ContextContains(entrymap& entries, jdesc context);
		jdesc GetEntrypoint(jdesc context);

		void PushTry(const token& varName);
		void Push(const token& varName, int32_t tableIndex);
		void PushConstant(const token& varName);
		void Pop(const token& var);

		void Label(const token& label);

		int32_t TablesContain(const token& name); // 0 == does not exist. non zero, subtract 1 for table index

		std::streampos Write(const token& content);
		void WriteCommand(const std::string& cmd);
		void WriteToken();
		void WriteTokenNext();
		void WriteLine(const std::string& str);
	protected:
		std::ifstream& mInFile;
		std::ofstream& mOutFile;
		jfile mFile;
		bool mIsMethod{ false }; // is the current function able to access member variables?
		token mClassContext{};
		token mFunctionContext{};
		int32_t mLabelStack{ 0 };
		int32_t mCurrentTable{ 0 };
		std::array<symtable, 2> mTables{};
		static const cmap mContexts;
	};

}
