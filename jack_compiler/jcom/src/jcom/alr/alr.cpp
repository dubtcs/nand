
#include "alr.h"

#include <iostream>

#include <jcom/cmp/st/smta.h>

#include <algorithm>

namespace jcom
{

	//const emap2 jalr::mFnPtrs{
	//	{"class", &jalr::ParseClass}
	//};

	const cmap jalr::mContexts
	{
		{ jdesc::Class, 
			{
				{"field", jdesc::ClassVarDec},
				{"static", jdesc::ClassVarDec},
				{"function", jdesc::Subroutine},
				{"constructor", jdesc::Subroutine},
				{"method", jdesc::Subroutine},

				{"}", jdesc::BREAKER},
			}
		},
		{ jdesc::ClassVarDec,
			{
				{";", jdesc::BREAKER},
			}
		},
		{ jdesc::Subroutine,
			{
				{"var", jdesc::VarDec},
				{"(", jdesc::ParameterList},
				{"{", jdesc::SubroutineBody},

				{"}", jdesc::BREAKER},
			}
		},
		{ jdesc::ParameterList,
			{
				{")", jdesc::BREAKER},
			}
		},
		{ jdesc::VarDec,
			{
				{";", jdesc::BREAKER},
			}
		},
		{ jdesc::SubroutineBody,
			{
				{"var", jdesc::VarDec},

				{"let", jdesc::Statement},
				{"do", jdesc::Statement},
				{"if", jdesc::Statement},
				{"while", jdesc::Statement},
				{"return", jdesc::Statement},

				{"}", jdesc::BREAKER},
			}
		},
		{ jdesc::Statement,
			{
				{"let", jdesc::LetStatement},
				{"do", jdesc::DoStatement},
				{"if", jdesc::IfStatement},
				{"while", jdesc::WhileStatement},
				{"return", jdesc::ReturnStatement},

				{"}", jdesc::BREAKER},
			}
		},
		{ jdesc::LetStatement,
			{
				{"=", jdesc::Expression},
				{";", jdesc::BREAKER}
			}
		},
		{ jdesc::DoStatement,
			{
				{";", jdesc::BREAKER},
				{"(", jdesc::SubroutineCall},
				{".", jdesc::SubroutineCall}
			}
		},
		{ jdesc::IfStatement,
			{
				{"{", jdesc::Statement},
				{"(", jdesc::Expression},
				{"}", jdesc::BREAKER},
			}
		},
		{ jdesc::WhileStatement,
			{
				{"{", jdesc::Statement},
				{"(", jdesc::Expression},
				{"}", jdesc::BREAKER},
			}
		},
		{ jdesc::ReturnStatement,
			{
				{";", jdesc::BREAKER},
			}
		},
		{ jdesc::ExpressionList,
			{
				{")", jdesc::BREAKER},
			}
		},
		{ jdesc::Expression,
			{
				{"+", jdesc::Expression},
				{"-", jdesc::Expression},
				{"*", jdesc::Expression},
				{"/", jdesc::Expression},

				{"&", jdesc::Expression},
				{"|", jdesc::Expression},

				{">", jdesc::Expression},
				{"<", jdesc::Expression},

				{"=", jdesc::Expression},
				{"(", jdesc::Term}
			}
		},
		{ jdesc::Term,
			{
				{"-", jdesc::Term},
				{"~", jdesc::Term},
				{"(", jdesc::Expression},
				//{")", jdesc::BREAKER}, // don't need this as only the brackets [] have a special rule
				{"]", jdesc::BREAKER},
			}
		},
		{ jdesc::SubroutineCall,
			{
				{"(", jdesc::ExpressionList},
				{")", jdesc::BREAKER }
			}
		},
	};

	static bool IsNumber(const token& str)
	{
		return std::all_of(str.begin(), str.end(), std::isdigit);
	}

	jalr::jalr(std::ifstream& inFile, std::ofstream& outFile) :
		mInFile{ inFile },
		mOutFile{ outFile },
		mFile{ inFile }
	{
		ParseToken();
	}

	void jalr::ParseToken()
	{
		while (mFile.Available())
		{
			const token& tk{ mFile.Get().content };
			if (tk == "class")
			{
				ParseClass();
			}
			mFile.Next();
		}
	}

	void jalr::ParseClass()
	{
		mFile.Next();
		mClassContext = mFile.Get().content;

		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::Class) };
			switch (entry)
			{
				case(jdesc::ClassVarDec): { ParseClassVar();		break; }
				case(jdesc::Subroutine): { ParseSubroutine();	break; }
				default: { WriteTokenNext(); break; }
			}
		}
	}

	void jalr::ParseClassVar()
	{
		bool breaker{ false };

		jpool varPool{ gTokenToPool.at(mFile.Get().content) }; // dnagerous, no index checking
		mFile.Next();
		std::string varKind{ mFile.Get().content };
		mFile.Next();

		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::ClassVarDec) };
			breaker = (entry == jdesc::BREAKER);
			const token& tk{ mFile.Get().content };
			if ((!breaker) && (tk != ","))
			{
				mTables.at(mCurrentTable).Define(tk, varKind, varPool);
				const syminfo& info{ mTables.at(mCurrentTable).GetInfo(tk) };
			}
			mFile.Next();
		}
	}

	void jalr::ParseSubroutine()
	{
		bool breaker{ false };
		mCurrentTable = TABLE_SUB;


		// Reset current table
		symtable& table{ mTables.at(mCurrentTable) };
		table = symtable{};

		table.Define("this", mClassContext, jpool::ARG);

		while (mFile.Available() && !breaker)
		{
			const token& tk{ mFile.Get().content };
			jdesc entry{ GetEntrypoint(jdesc::Subroutine) };

			switch (entry)
			{
				case(jdesc::VarDec):		{ ParseSubroutineVar(); break; }
				case(jdesc::ParameterList): { ParseParameterList(); break; }
				case(jdesc::SubroutineBody):{ ParseSubroutineBody(); break; }
				case(jdesc::BREAKER):		{ breaker = true; break; }
				default:					{ WriteTokenNext(); break; } // should never get this if syntax is correct
			}
		}

		mCurrentTable = TABLE_CLASS;
		mFile.Next();
	}

	// this and class var can be condensed now that xml is meaningless
	void jalr::ParseSubroutineVar()
	{
		bool breaker{ false };

		jpool varPool{ gTokenToPool.at(mFile.Get().content) }; // dnagerous, no index checking
		mFile.Next();
		std::string varKind{ mFile.Get().content };
		mFile.Next();

		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::VarDec) };
			breaker = (entry == jdesc::BREAKER);

			const token& tk{ mFile.Get().content };
			if ((!breaker) && (tk != ","))
			{
				mTables.at(mCurrentTable).Define(tk, varKind, varPool);
				std::cout << tk << '\n';
				const syminfo& info{ mTables.at(mCurrentTable).GetInfo(tk) };
			}

			mFile.Next();
		}
	}

	void jalr::ParseParameterList()
	{
		WriteToken();
		mFile.Next();

		token kind{};
		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::ParameterList) };

			if (entry == jdesc::BREAKER)
				break;

			symtable& table{ mTables.at(mCurrentTable) };
			const token& tk{ mFile.Get().content };
			if (tk != ",")
			{
				if (kind.empty())
					kind = tk;
				else
				{
					table.Define(tk, kind, jpool::ARG);
					kind = "";
				}
			}

			mFile.Next();
		}

		mFile.Next();
	}

	void jalr::ParseSubroutineBody()
	{
		WriteTokenNext();

		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::SubroutineBody) };
			switch (entry)
			{
				case(jdesc::VarDec): { ParseSubroutineVar(); break; }
				case(jdesc::Statement): { ParseStatements(); break; }
				case(jdesc::BREAKER): { breaker = true; break; }
				default: { WriteTokenNext(); break; }
			}
		}

		WriteToken();
	}

	void jalr::ParseStatements()
	{
		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::Statement) };
			switch (entry)
			{
				case(jdesc::LetStatement): { ParseLetStatement(); break; }
				case(jdesc::DoStatement): { ParseDoStatement(); break; }
				case(jdesc::ReturnStatement): { ParseReturnStatement(); break; }
				case(jdesc::IfStatement): {ParseIfStatement(); break; }
				case(jdesc::WhileStatement): {ParseWhileStatement(); break; }
				case(jdesc::BREAKER): { breaker = true; break; }
				default: { mFile.Next(); break; }
			}
		}
	}

	void jalr::ParseLetStatement()
	{

		// ignore LET
		mFile.Next();

		token name{ mFile.Get().content };

		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::LetStatement) };
			//WriteTokenNext();
			mFile.Next();
			if (entry == jdesc::BREAKER)
				break;
			else if (entry == jdesc::Expression)
				ParseExpression();
		}

		Pop(name);

	}

	void jalr::ParseDoStatement()
	{

		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::DoStatement) };
			if (entry == jdesc::SubroutineCall)
				ParseSubroutineCall();
			else
				WriteTokenNext();
			if (entry == jdesc::BREAKER)
				break;
		}

	}

	void jalr::ParseReturnStatement()
	{

		WriteTokenNext(); // return 
		if (mFile.Get().content != ";") // optional expression
			ParseExpression();
		WriteTokenNext(); // ;

	}

	void jalr::ParseIfStatement()
	{
		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::IfStatement) };
			WriteTokenNext();

			switch (entry)
			{
				case(jdesc::Expression): { ParseExpression(); break; }
				case(jdesc::Statement): { ParseStatements(); break; }
				case(jdesc::BREAKER): {
					if (mFile.Get().content == "else")
					{
						//WriteTokenNext(); // write } and move
						WriteTokenNext(); // write else and move
						WriteTokenNext(); // write { and move
						ParseStatements();// start parsing statements
					}
					else
					{
						//WriteTokenNext();
						breaker = true;
					}
					break;
				}
				//default: //WriteTokenNext();
			}
		}
	}

	void jalr::ParseWhileStatement()
	{
		// copied from if statement with minor changes
		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::IfStatement) };
			WriteTokenNext();

			switch (entry)
			{
				case(jdesc::Expression): { ParseExpression(); break; }
				case(jdesc::Statement): { ParseStatements(); break; }
				case(jdesc::BREAKER): { breaker = true; break; }
			}
		}
	}

	void jalr::ParseExpressionList()
	{
		WriteTokenNext();
		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::ExpressionList) };
			if (entry == jdesc::BREAKER)
				break;
			else
				ParseExpression();
			WriteTokenNext();
		}
		WriteTokenNext();
	}

	void jalr::ParseExpression()
	{
		bool theseFound{ false };
		jdesc entry{ GetEntrypoint(jdesc::Expression) };
		if (entry != jdesc::BREAKER)
			ParseTerm();
	}

	void jalr::ParseTerm()
	{
		bool breaker{ false };
		bool isNested{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::Term) };
			const token tk{ mFile.Get().content };
			mFile.Next();
			std::cout << tk << '\n';
			switch (entry)
			{
				case(jdesc::Expression): { ParseExpression(); break; }
				case(jdesc::Term): { ParseTerm(); break; }
				case(jdesc::BREAKER): 
				{
					breaker = true;
					WriteTokenNext();
					break;
				}
				default:
				{
					if (IsNumber(tk))
					{
						PushConstant(tk);
					}
					else
					{
						if (mTables.at(mCurrentTable).Contains(tk))
							Push(tk);
						else if (mTables.at(mCurrentTable - 1).Contains(tk)) // this (-1) is dangerous ug og
							Push(tk);
						//else
							// complex expression
					}

					const token& next{ mFile.Get().content };
					if (next == "[") // handle arrays
					{
						isNested = true;
						WriteTokenNext(); // Write opening brace/bracket
						ParseExpression();
					}
					else if (next == "(" || next == ".")
					{
						ParseSubroutineCall();
						breaker = true;
					}
					else
						breaker = true;
					break;
				}
			}
		}
	}

	void jalr::ParseSubroutineCall()
	{
		// This is only called once it's confirmed to be a subroutine call.
		// Check if id is followed by parenthesis or a .
		// eg joe.""; or mama();

		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::SubroutineCall) };
			switch (entry)
			{
				case(jdesc::ExpressionList): { ParseExpressionList(); breaker = true; break; }
				case(jdesc::BREAKER): { breaker = true; break; }
				default: { WriteTokenNext(); break; }
			}
		}
	}

	// dep, use GetEntrypoint()
	bool jalr::ContextContains(entrymap& entries, jdesc context)
	{
		const token& tk{ mFile.Get().content };
		if (entries.contains(context))
			return entries.at(context).contains(tk);
		return false;
	}

	jdesc jalr::GetEntrypoint(jdesc context)
	{
		jdesc rdesc{ jdesc::None };
		if (mContexts.contains(context))
		{
			const emap& entries{ mContexts.at(context) };
			const token& tk{ mFile.Get().content };
			if (entries.contains(tk))
				rdesc = entries.at(tk);
		}
		return rdesc;
	}

	void jalr::Push(const token& name)
	{
		const syminfo& info{ mTables.at(mCurrentTable).GetInfo(name) };
		WriteLine("push " + gPoolToToken.at(info.pool) + " " + std::to_string(info.index));
	}

	void jalr::PushConstant(const token& num)
	{
		WriteLine("push " + gPoolToToken.at(jpool::CONST) + " " + num);
	}

	void jalr::Pop(const token& name)
	{
		const syminfo& info{ mTables.at(mCurrentTable).GetInfo(name) };
		WriteLine("pop " + gPoolToToken.at(info.pool) + " " + std::to_string(info.index));
	}

	void jalr::WriteLine(const std::string& content)
	{
		mOutFile << content << '\n';
	}

	void jalr::WriteCommand(const std::string& cmd)
	{
		WriteLine(cmd);
	}

	void jalr::WriteToken()
	{
		jpair& pair{ mFile.Get() };
		const Tag& tag{ gTokenFlags.at(pair.type) };
		WriteLine(tag.opener + pair.content + tag.closer);
	}
	
	void jalr::WriteTokenNext()
	{
		WriteToken();
		mFile.Next();
	}

}
