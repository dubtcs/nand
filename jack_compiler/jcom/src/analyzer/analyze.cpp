
#include "analyze.h"
#include "token/jfile.h"

namespace jcom
{

	static size_t gCurrentTree{ 0 };
	static std::string gTreePrefix{};

	static std::unordered_set<token> gStatements
	{
		"do", "let", "while", "return", "if"
	};

	void WriteLine(std::ofstream& file, const std::string& str)
	{
		file << str << "\n";
	}

	std::string JTreeFormat(const std::string& str)
	{
		return gTreePrefix + str;
	}

	void IncTreeString()
	{
		gTreePrefix += '\t';
	}

	void DecTreeString()
	{
		gTreePrefix.pop_back();
	}

	class jph
	{
	public:
		jph(std::ofstream& outFile, const token& name) :
			mFile{ outFile },
			mContent{ name }
		{
			WriteLine(outFile, JTreeFormat("<" + mContent + ">"));
			IncTreeString();
		}
		~jph()
		{
			DecTreeString();
			WriteLine(mFile, JTreeFormat("</" + mContent + ">"));
		}
	protected:
		std::ofstream& mFile;
		std::string mContent;
	};

	// Recursive Class Declarations
	// These all do the same thing
	// Can just have a lambda that acts as a break function, with keywords mapped to functions for specifics
	void ParseToken(std::ofstream& outFile, const jpair& pair);
	void ParseVar(std::ofstream& outfile, jfile& file);
	void ParseClass();
	void ParseClassVar(std::ofstream& of, jfile& f);
	void ParseSubroutine(std::ofstream& of, jfile& f);
	void ParseParameters(std::ofstream& of, jfile& f);
	void ParseSubroutineBody(std::ofstream& of, jfile& f);
	void ParseStatement(std::ofstream& outFile, jfile& file);
	void ParseStatementBody(std::ofstream& outFile, jfile& file);
	void ParseExpression(std::ofstream& outFile, jfile& file);
	void ParseExpressionList(std::ofstream& outFile, jfile& file);

	// Standard token output as <type>content</type>
	void ParseToken(std::ofstream& outFile, const jpair& pair)
	{
		std::string content{ gTokenFlags.at(pair.type).opener + pair.content + gTokenFlags.at(pair.type).closer };
		WriteLine(outFile, JTreeFormat(content));
	}

	void ParseExpression(std::ofstream& outFile, jfile& file)
	{
		jpair pair{};
		bool end{ false };

		// This is so we include the keyword
		ParseToken(outFile, file.GetCurrent());

		WriteLine(outFile, JTreeFormat("<expression>"));
		IncTreeString();

		while (!end && file.NextToken(pair))
		{
			const token& tk{ pair.content };
			if (pair.content == ")")
				end = true;
			else
				ParseToken(outFile, pair);
		}

		DecTreeString();
		WriteLine(outFile, JTreeFormat("</expression>"));

		// manual tree inc/dec bc the formatting rules change here???

		ParseToken(outFile, pair);
	}

	void ParseExpressionList(std::ofstream& outFile, jfile& file)
	{
		jpair pair{};
		bool end{ false };

		// This is so we include the keyword
		ParseToken(outFile, file.GetCurrent());

		WriteLine(outFile, JTreeFormat("<expressionList>"));
		IncTreeString();

		while (!end && file.NextToken(pair))
		{
			const token& tk{ pair.content };
			if (pair.content == ")")
				end = true;
			else
				ParseToken(outFile, pair);
		}

		DecTreeString();
		WriteLine(outFile, JTreeFormat("</expressionList>"));

		// manual tree inc/dec bc the formatting rules change here???
		
		ParseToken(outFile, pair);
	}

	void ParseStatementBody(std::ofstream& outFile, jfile& file)
	{

		jpair pair{};
		bool end{ false };

		// This is so we include the keyword
		ParseToken(outFile, file.GetCurrent());

		{
			jph header{ outFile, "statements" };
			while (!end && file.NextToken(pair))
			{
				const token& tk{ pair.content };
				if(tk == "}")
					end = (pair.content == "}");
				else if (tk == "var")
					ParseVar(outFile, file);
				else if (gStatements.contains(tk))
					ParseStatement(outFile, file);
				else
					ParseToken(outFile, pair);
			}
		}

		ParseToken(outFile, file.GetCurrent());

	}

	void ParseStatement(std::ofstream& outFile, jfile& file)
	{
		token h1{ file.GetCurrent().content };
		jph h{ outFile, h1 + "Statement" };

		jpair pair{};
		bool end{ false };

		// This is so we include the keyword
		ParseToken(outFile, file.GetCurrent());
		while (!end && file.NextToken(pair))
		{
			const token& tk{ pair.content };
			if (tk == "(")
				if (h1 == "do")
					ParseExpressionList(outFile, file);
				else
					ParseExpression(outFile, file);
			else if (tk == "{")
				ParseStatementBody(outFile, file);
			else
				ParseToken(outFile, pair);
			end = (pair.content == ";");
		}
	}

	void ParseVar(std::ofstream& outFile, jfile& file)
	{
		jph h{ outFile, "varDec" };

		jpair pair{};
		bool end{ false };

		// This is so we include the keyword
		ParseToken(outFile, file.GetCurrent());
		while (!end && file.NextToken(pair))
		{
			ParseToken(outFile, pair);
			end = (pair.content == ";");
		}

	}

	void ParseClassVar(std::ofstream& outFile, jfile& file)
	{
		jph header{ outFile, "classVarDec" };

		jpair pair{};
		bool end{ false };

		// This is so we include the keyword
		ParseToken(outFile, file.GetCurrent());
		while (!end && file.NextToken(pair))
		{
			ParseToken(outFile, pair);
			end = (pair.content == ";");
		}

	}

	void ParseStatements(std::ofstream& outFile, jfile& file)
	{
		jph smt{ outFile, "statements" };

		jpair pair{};
		bool end{ false };

		if (gStatements.contains(file.GetCurrent().content)) // always passes, but double checking
			ParseStatement(outFile, file);
		while (!end && file.NextToken(pair))
		{
			const token& tk{ pair.content };
			if (gStatements.contains(tk))
				ParseStatement(outFile, file);
			else
				ParseToken(outFile, pair);
			end = (tk == "}");
		}

	}
	
	void ParseSubroutineBody(std::ofstream& outFile, jfile& file)
	{
		jph header{ outFile, "subroutineBody" };

		jpair pair{};
		bool end{ false };

		// This is so we include the keyword
		ParseToken(outFile, file.GetCurrent());
		while (!end && file.NextToken(pair))
		{
			const token& tk{ pair.content };
			if (tk == "}")
				end = true;
			else if (tk == "var")
				ParseVar(outFile, file);
			else if (gStatements.contains(tk))
			{
				ParseStatements(outFile, file);
				end = true;
			}
			else
				ParseToken(outFile, pair);
		}

	}

	void ParseParameters(std::ofstream& outFile, jfile& file)
	{
		jpair pair{};
		bool end{ false };

		// This is so we include the keyword
		ParseToken(outFile, file.GetCurrent());

		WriteLine(outFile, JTreeFormat("<parameterList>"));
		IncTreeString();

		while (!end && file.NextToken(pair))
		{
			const token& tk{ pair.content };
			if (tk == ")")
				end = true;
			else
				ParseToken(outFile, pair);
			end = (pair.content == ")");
		}

		DecTreeString();
		WriteLine(outFile, JTreeFormat("</parameterList>"));

		ParseToken(outFile, pair);

	}

	void ParseSubroutine(std::ofstream& outFile, jfile& file)
	{
		jph header{ outFile, "subroutineDec" };

		jpair pair{};
		bool end{ false };

		// This is so we include the keyword
		ParseToken(outFile, file.GetCurrent());
		while (!end && file.NextToken(pair))
		{
			const token& tk{ pair.content };
			if (tk == "(")
				ParseParameters(outFile, file);
			else if (tk == "{")
			{
				ParseSubroutineBody(outFile, file);
				end = true;
			}
			else
				ParseToken(outFile, pair);
		}
	}

	void ParseClass(std::ofstream& outFile, jfile& file)
	{
		WriteLine(outFile, JTreeFormat("<class>"));
		IncTreeString();

		jpair pair{};
		while (file.NextToken(pair))
		{
			const token& tk{ pair.content };
			std::cout << pair.content << ", ";
			if (tk == "static" || tk == "field")
			{
				ParseClassVar(outFile, file);
			}
			else if (tk == "method" || tk == "function" || tk == "constructor")
			{
				ParseSubroutine(outFile, file);
			}
			else
				ParseToken(outFile, pair);
		}

		DecTreeString();
		WriteLine(outFile, JTreeFormat("</class>"));
	}

	void AnalyzeTokens(std::ofstream& outFile, jfile& file)
	{
		ParseClass(outFile, file);
		//jpair pair{};
		//while (file.NextToken(pair))
		//{
		//	//WriteLine(outFile, JTreeFormat(gTokenFlags.at(pair.type).opener));
		//	//if (gReservedTokens.contains(pair.content))
		//	//{
		//	//	IncTreeString();
		//	//	AnalyzeTokens(outFile, file);
		//	//	// ^ replace with specific keyword functions like LetStatement(), DoStatement(); etc
		//	//	DecTreeString();
		//	//}
		//	//else
		//	//{
		//	//	WriteLine(outFile, JTreeFormat(pair.content));
		//	//}
		//	//WriteLine(outFile, JTreeFormat(gTokenFlags.at(pair.type).closer));
		//}
	}

	void AnalyzeFile(std::ifstream& inFile, std::ofstream& outFile)
	{
		WriteLine(outFile, "<tokens>");
		IncTreeString();

		std::string str{ };

		jfile file{ inFile };
		jpair pair{};

		AnalyzeTokens(outFile, file);

		DecTreeString();
		WriteLine(outFile, "</tokens>");

	}

}
