
#include "alr.h"

#include <iostream>

#include <jcom/cmp/st/smta.h>

#include <algorithm>
#include <bitset>

namespace jcom
{

	//const emap2 jalr::mFnPtrs{
	//	{"class", &jalr::ParseClass}
	//};

	static const std::unordered_map<token, token> gOperatorSymbols
	{
		{ "-", "sub" },
		{ "+", "add" },
		{ "*", "call Math.multiply 2" }, // OS Call
		{ "/", "call Math.divide 2" },	// OS Call
		{ ">", "gt" },
		{ "<", "lt" },
		{ "=", "eq" }, // bruh
		{ "&", "and" },
		{ "|", "or" },
	};

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
				case(jdesc::ClassVarDec): { ParseClassVar();	break; }
				case(jdesc::Subroutine): { ParseSubroutine();	break; }
				default: { mFile.Next(); break; }
			}
		}
	}

	void jalr::ParseClassVar()
	{
		bool breaker{ false };

		jpool varPool{ gTokenToPool.at(mFile.Get().content) }; // dangerous, no index 
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
				mTables.at(TABLE_CLASS).Define(tk, varKind, varPool);
				const syminfo& info{ mTables.at(mCurrentTable).GetInfo(tk) };
			}
			mFile.Next();
		}
	}

	void jalr::ParseSubroutine()
	{
		bool breaker{ false };
		mCurrentTable = TABLE_SUB;

		bool isConstructor{ mFile.Get().content == "constructor" };
		mIsMethod = ((mFile.Get().content == "method") || isConstructor); // skip function keyword
		mFile.Next();
		mFile.Next(); // skip return type
		mFunctionContext = mFile.Get().content; // get function name
		mLabelStack = 0;

		// Reset current table
		symtable& table{ mTables.at(mCurrentTable) };
		table = symtable{};

		if (mIsMethod)
			table.Define("this", mClassContext, jpool::POINTER);

		int32_t args{ 0 };

		while (mFile.Available() && !breaker)
		{
			const token& tk{ mFile.Get().content };
			jdesc entry{ GetEntrypoint(jdesc::Subroutine) };

			switch (entry)
			{
				case(jdesc::ParameterList): { 
					args += ParseParameterList() + (mIsMethod - isConstructor); // isMethod either 0 or 1 so we can just add :D
					WriteLine("\nfunction " + mClassContext + "." + mFunctionContext + " " + std::to_string(args));
					if (mIsMethod && !isConstructor)
					{
						WriteLine("push argument 0"); // set pointer
						WriteLine("pop pointer 0"); // set this
					}
					break; 
				}
				case(jdesc::VarDec):		{ ParseSubroutineVar(); break; }
				case(jdesc::SubroutineBody):{ 
					if (isConstructor)
					{
						PushConstant(std::to_string(mTables.at(TABLE_CLASS).GetPoolCount(jpool::THIS)));
						WriteLine("call Memory.alloc 1"); // OS level function
						WriteLine("pop pointer 0");
					}
					ParseSubroutineBody(); 
					break; }
				case(jdesc::BREAKER):		{ breaker = true; break; }
				default:					{ mFile.Next(); break; } // should never get this if syntax is correct
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
		mFile.Next(); // skip keyword
		std::string varKind{ mFile.Get().content };
		mFile.Next(); // skip type

		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::VarDec) };
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

	int32_t jalr::ParseParameterList()
	{
		//WriteToken();
		mFile.Next();
		int32_t count{ 0 };

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
					count++;
					kind = "";
				}
			}

			mFile.Next();
		}

		mFile.Next();
		return count;
	}

	void jalr::ParseSubroutineBody()
	{
		mFile.Next();

		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::SubroutineBody) };
			switch (entry)
			{
				case(jdesc::VarDec): { ParseSubroutineVar(); break; }
				case(jdesc::Statement): { ParseStatements(); break; }
				case(jdesc::BREAKER): { breaker = true; break; }
				default: { mFile.Next(); break; }
			}
		}

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

		token prev{ mFile.Get().content };
		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::DoStatement) };
			if (entry == jdesc::SubroutineCall)
				ParseSubroutineCall((mFile.Get().content == ".") ? prev : mClassContext); // check if it's external or not, use current class context if none provided
			else
			{
				prev = mFile.Get().content;
				mFile.Next();
			}
			if (entry == jdesc::BREAKER)
				break;
		}

	}

	void jalr::ParseReturnStatement()
	{
		mFile.Next(); // skip return keyword
		if (mFile.Get().content != ";") // optional expression
			ParseExpression();
		else
			PushConstant("0"); // always need to return a value, so we push 0 if no expression is found
		WriteLine("return");
	}

	void jalr::ParseIfStatement()
	{
		bool breaker{ false };
		token l1{ "branch." + mClassContext + "." + mFunctionContext + std::to_string(mLabelStack++) };
		token lif{ l1 + ".PASS" };
		token lelse{ l1 + ".END" };
		Label(l1);

		bool hasElse{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::IfStatement) };
			mFile.Next();

			switch (entry)
			{
				case(jdesc::Expression): { 
					ParseExpression();
					WriteLine("not");
					WriteLine("if-goto " + lif);
					WriteLine("goto " + lelse);
					break; 
				}
				case(jdesc::Statement): {
					Label(lif);
					ParseStatements(); 
					break; 
				}
				case(jdesc::BREAKER): {
					if (mFile.Get().content == "else")
					{
						hasElse = true;
						mFile.Next();
						mFile.Next();
						Label(lelse);
						ParseStatements();// start parsing statements
					}
					else
					{
						breaker = true;
						if (!hasElse)
							Label(lelse);
					}
					break;
				}
			}
		}
	}

	void jalr::ParseWhileStatement()
	{
		// copied from if statement with minor changes
		token l1{ "loop." + mClassContext + "." + mFunctionContext + std::to_string(mLabelStack++) };
		token lif{ l1 + ".CONT" };
		token lelse{ l1 + ".END" };
		Label(l1);

		bool breaker{ false };

		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::IfStatement) };
			//WriteTokenNext();
			mFile.Next();

			switch (entry)
			{
				case(jdesc::Expression): { 
					ParseExpression();
					WriteLine("not");
					WriteLine("if-goto " + lif);
					WriteLine("goto " + lelse);
					break; 
				}
				case(jdesc::Statement): { ParseStatements(); break; }
				case(jdesc::BREAKER): { 
					breaker = true; 
					Label(lelse);
					break; 
				}
			}
		}
	}

	int32_t jalr::ParseExpressionList()
	{
		mFile.Next(); // skip keyword
		int32_t count{ 0 };
		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::ExpressionList) };
			if (entry == jdesc::BREAKER)
				break;
			else
				count += ParseExpression();
			if (mFile.Get().content == ")")
				break;
			mFile.Next(); // skip keyword
		}
		mFile.Next(); // skip keyword
		return count;
	}

	int32_t jalr::ParseExpression()
	{
		std::stack<token> operatorStack{};

		jdesc entry{ GetEntrypoint(jdesc::Expression) };
		int32_t count{ 0 };

		if (entry != jdesc::BREAKER)
		{
			count += ParseTerm(); // this advances file cursor
			const token& tk{ mFile.Get().content };
			if (gOperatorSymbols.contains(tk)) // ignores PEMDAS for now
			{
				operatorStack.push(tk);
				mFile.Next();
				ParseExpression();
			}
		}

		// run operator commands
		while (!operatorStack.empty())
		{
			const token& top{ operatorStack.top() };
			WriteLine(gOperatorSymbols.at(top));
			operatorStack.pop();
			count--; // each of these consumes 1 variable
		}

		return count;
	}

	int32_t jalr::ParseTerm()
	{
		token tk{ mFile.Get().content };
		jtok type{ mFile.Get().type };

		int32_t count{ 1 };

		// Unary operator
		switch (type)
		{
			case(jtok::Int):
			{
				if (IsNumber(tk))
					PushConstant(tk);
				else
					std::cout << "Integer constant but not a number: " << tk << '\n';
				break;
			}
			case(jtok::String):
			{
				for (const char& c : tk)
					WriteLine("String.appendChar(" + std::string{ c } + ")");
				break;
			}
			case(jtok::Keyword):
			{
				// null == 0
				// false == 0
				// true == -1
				if (tk == "true")
				{
					PushConstant("1");
					WriteLine("neg");
				}
				else if (tk == "false" || tk == "null")
					PushConstant("0");
				else if (tk == "this")
					Push(tk, TABLE_SUB);
				break;
			}
			case(jtok::Symbol):
			{
				// Check for unary operators
				if (tk == "~") // not
				{
					mFile.Next();
					ParseTerm();
					WriteLine("not");
					return count; // return here because these are special cases without token skips
				}
				else if (tk == "-") // negate
				{
					mFile.Next();
					ParseTerm();
					WriteLine("neg");
					return count; // return here because these are special cases without token skips
				}
				else if (tk == "(")
				{
					mFile.Next();
					count = ParseExpression(); // count overwrite
				}
				else
					count = 0;
				break;
			}
			case(jtok::Id):
			{
				const token& next{ mFile.PeekNext().content };
				if (next == "[" || next == "." || next == "(")
				{
					// array or subroutine
					if (next == "(")
					{
						ParseSubroutineCall();
						return count;
					}
					else if (next == ".")
					{
						mFile.Next();
						ParseSubroutineCall(tk); // edge cases, return immediately
						return count;
					}
					else
					{
						// arrays
					}
				}
				else // just an ID
				{
					PushTry(tk);
				}
				break;
			}
		}

		mFile.Next();
		return count;
	}

	void jalr::ParseSubroutineCall(token label)
	{
		// This is only called once it's confirmed to be a subroutine call.
		// Check if id is followed by parenthesis or a .
		// eg joe.""; or mama();

		if (mFile.Get().content == ".")
		{
			mFile.Next();
			label += "." + mFile.Get().content;
		}
		else
		{ // method call
			label = mFile.Get().content;
			WriteLine("push pointer 0");
		}

		bool breaker{ false };
		int32_t argumentCount{ 0 };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::SubroutineCall) };
			switch (entry)
			{
				case(jdesc::ExpressionList): { argumentCount += ParseExpressionList(); breaker = true; break; }
				case(jdesc::BREAKER): { breaker = true; break; }
				default: { mFile.Next();  break; }
			}
		}

		WriteLine("call " + label + " " + std::to_string(argumentCount));
		
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

	void jalr::Push(const token& name, int32_t index)
	{
		const syminfo& info{ mTables.at(index).GetInfo(name) };
		WriteLine("push " + gPoolToToken.at(info.pool) + " " + std::to_string(info.index));
	}

	void jalr::PushConstant(const token& num)
	{
		WriteLine("push " + gPoolToToken.at(jpool::CONST) + " " + num);
	}

	void jalr::PushTry(const token& name)
	{
		if ((mCurrentTable > 0) && mTables.at(TABLE_SUB).Contains(name))
			Push(name, TABLE_SUB);
		else if (mTables.at(TABLE_CLASS).Contains(name))
			Push(name, TABLE_CLASS);
		else
			std::cout << "No symbol found: " << name << '\n';
	}

	void jalr::Pop(const token& name)
	{
		const syminfo& info {
			mTables.at(TABLE_SUB).Contains(name) ? 
			mTables.at(TABLE_SUB).GetInfo(name) : 
			mTables.at(TABLE_CLASS).GetInfo(name) // this doesn't even bother checking if it exists :/
		};
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

	void jalr::Label(const token& label)
	{
		WriteLine("label " + label);
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


/*

bool breaker{ false };
		bool isNested{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::Term) };
			const token tk{ mFile.Get().content };
			mFile.Next();
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
					{
						// Not a subroutine call or array
						if (IsNumber(tk))
						{
							PushConstant(tk);
						}
						else
						{
							// Can do this only because there is a limit of only 2 scopes in Jack
							if ((mCurrentTable > 0) && mTables.at(TABLE_SUB).Contains(tk))
								Push(tk, TABLE_SUB);
							else if (mTables.at(TABLE_CLASS).Contains(tk))
								Push(tk, TABLE_CLASS);
							else if (gOperatorSymbols.contains(tk))
								WriteLine(gOperatorSymbols.at(tk));

							breaker = true;
						}
					}
					break;
				}
			}
		}

*/