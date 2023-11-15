
#include "alr.h"

#include <iostream>

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
				{";", jdesc::BREAKER},
			}
		},
		{ jdesc::DoStatement,
			{
				{";", jdesc::BREAKER},
				{"(", jdesc::ExpressionList},
			}
		},
		{ jdesc::IfStatement,
			{
				{"}", jdesc::BREAKER},
			}
		},
		{ jdesc::WhileStatement,
			{
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
			}
		},
	};

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
			/*if (mBruh.contains(tk))
			{
				WriteLine("Shit");
				auto j = mFnPtrs.at(tk);
				std::invoke(j, this);
			}*/
			if (tk == "class")
			{
				ParseClass();
			}
			mFile.Next();
		}
	}

	void jalr::ParseClass()
	{
		IncTree("<class>");
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
		DecTree("</class>");
	}

	void jalr::ParseClassVar()
	{
		IncTree("<classVarDec>");
		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::ClassVarDec) };
			breaker = (entry == jdesc::BREAKER);
			WriteToken();
			mFile.Next();
		}
		DecTree("</classVarDec>");
	}

	void jalr::ParseSubroutine()
	{
		IncTree("<subroutineDec>");
		bool breaker{ false };
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
		DecTree("</subroutineDec>");
	}

	void jalr::ParseSubroutineVar()
	{
		IncTree("<varDec>");
		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::VarDec) };
			breaker = (entry == jdesc::BREAKER);
			WriteToken();
			mFile.Next();
		}
		DecTree("</varDec>");
	}

	void jalr::ParseParameterList()
	{
		WriteToken();
		IncTree("<parameterList>");
		mFile.Next();

		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::ParameterList) };

			if (entry == jdesc::BREAKER)
				break;

			WriteToken();
			mFile.Next();
		}
		DecTree("</parameterList>");

		WriteToken();
		mFile.Next();
	}

	void jalr::ParseSubroutineBody()
	{
		IncTree("<subroutineBody>");
		WriteTokenNext();

		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::SubroutineBody) };
			switch (entry)
			{
				case(jdesc::VarDec): { ParseClassVar(); break; }
				case(jdesc::Statement): { ParseStatements(); break; }
				case(jdesc::BREAKER): { breaker = true; break; }
				default: { WriteTokenNext(); break; }
			}
		}

		WriteTokenNext();
		DecTree("</subroutineBody>");
	}

	void jalr::ParseStatements()
	{
		IncTree("<statements>");
		bool breaker{ false };
		while (mFile.Available() && !breaker)
		{
			jdesc entry{ GetEntrypoint(jdesc::Statement) };
			switch (entry)
			{
				case(jdesc::LetStatement): {ParseLetStatement(); break; }
				case(jdesc::DoStatement): {ParseDoStatement(); break; }
				case(jdesc::ReturnStatement): {ParseReturnStatement(); break; }
				case(jdesc::BREAKER): { breaker = true; break; }
				default: { mFile.Next(); break; }
			}
		}
		DecTree("</statements>");
	}

	void jalr::ParseLetStatement()
	{
		IncTree("<letStatement>");

		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::LetStatement) };
			WriteTokenNext();
			if (entry == jdesc::BREAKER)
				break;
		}

		DecTree("</letStatement>");
	}

	void jalr::ParseDoStatement()
	{
		IncTree("<doStatement>");

		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::DoStatement) };
			if (entry == jdesc::ExpressionList)
				ParseExpressionList();
			else
				WriteTokenNext();
			if (entry == jdesc::BREAKER)
				break;
		}

		DecTree("</doStatement>");
	}

	void jalr::ParseReturnStatement()
	{
		IncTree("<returnStatement>");

		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::ReturnStatement) };
			WriteTokenNext();
			if (entry == jdesc::BREAKER)
				break;
		}

		DecTree("</returnStatement>");
	}

	void jalr::ParseExpressionList()
	{
		WriteTokenNext();
		IncTree("<expressionList>");
		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::ExpressionList) };
			if (entry == jdesc::BREAKER)
				break;
			else
				ParseExpression();
			WriteTokenNext();
		}
		DecTree("</expressionList>");
		WriteTokenNext();
	}

	void jalr::ParseExpression()
	{
		IncTree("<expression>");
		while (mFile.Available())
		{
			jdesc entry{ GetEntrypoint(jdesc::Expression) };
			if (entry != jdesc::Expression)
			{
				// TODO
			}
		}
		DecTree("</expression>");
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

	void jalr::IncTree(const std::string& label)
	{
		WriteLine(label);
		mTreeString += '\t';
	}

	void jalr::DecTree(const std::string& label)
	{
		mTreeString.pop_back();
		WriteLine(label);
	}

	void jalr::WriteLine(const std::string& content)
	{
		mOutFile << mTreeString + content << '\n';
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
