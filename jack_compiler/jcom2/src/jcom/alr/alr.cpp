
#include "alr.h"

#include <unordered_map>
#include <unordered_set>

#include <iostream>

// The most convoluted state machine in the world
// this is a state machine without any of the ease of use of a state machine lmfao

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
		{jdesc::Class,				"class"},
		{jdesc::ClassVarDec,		"classVarDec"},
		{jdesc::Subroutine,			"subroutineDec"},
		{jdesc::SubroutineBody,		"subroutineBody"},
		{jdesc::Statement,			"statements"},
		{jdesc::IfNested,			"statements"},
		{jdesc::ReturnStatement,	"returnStatement"},
		{jdesc::IfStatement,		"ifStatement"},
		{jdesc::WhileStatement,		"whileStatement"},
		{jdesc::DoStatement,		"doStatement"},
		{jdesc::LetStatement,		"letStatement"},
		{jdesc::VarDec,				"varDec"},
		{jdesc::Expression,			"expression"},
		{jdesc::ParameterList,		"parameterList"}
	};

	enum class jTagGate
	{
		None, Open, Close
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

	void jalr::PushTree(jdesc descriptor)
	{
		mStack.push(descriptor);
		IncTree();
	}

	void jalr::IncTree()
	{
		WriteLine(mTreeString + "<" + gDescTokens.at(mStack.top()) + ">");
		mTreeString += '\t';
	}

	void jalr::PopTree()
	{
		DecTree();
		mStack.pop();
	}

	void jalr::DecTree()
	{
		mTreeString.pop_back();
		WriteLine(mTreeString + "</" + gDescTokens.at(mStack.top()) + ">");
	}

	void jalr::ParseToken()
	{
		int32_t j{ 0 };
		while (mFile.Available())
		{
			jcontext context{ gContexts.at(mStack.top()) };
			jpair pair{ mFile.Get() };
			token tk{ pair.content };

			jTagGate tokenState{ jTagGate::None };

			// Current context overrides current token
			if (context.contextOverrides.contains(tk))
			{
				// Add the context to the stack and make it the new active context
				mStack.push(context.contextOverrides.at(tk));
				context = gContexts.at(mStack.top());
				tokenState = jTagGate::Open;
				if (mStack.top() == jdesc::Statement) // could have used standard enums with a bitwise and here :/
				{
					IncTree();
				}
			}

			// Current token is a keyword that isn't overriden
			if (gKeywordDescs.contains(tk))
			{
				mStack.push(gKeywordDescs.at(tk));
				context = gContexts.at(mStack.top());
				tokenState = jTagGate::Open;
			}
			// Current token is the current context's breaker
			else if (tk == context.breaker)
			{
				// Statements have a catch with ELSE
				if (mStack.top() == jdesc::IfStatement)
				{
					// Next keyword is else, so we keep the statement
					if (mFile.PeekNext().content == "else")
						tokenState = jTagGate::None;
					else
						tokenState = jTagGate::Close;
				}
				else
				{
					// Statement or nested statement inside another statement
					if (mStack.top() == jdesc::Statement || mStack.top() == jdesc::IfNested)
					{
						if ((mStack.top() == jdesc::IfNested) && (mFile.PeekNext().content == "else"))
							tokenState = jTagGate::None;
						else
							tokenState = jTagGate::Close;
						PopTree();
						context = gContexts.at(mStack.top());
					}
					else
						tokenState = jTagGate::Close;
				}
			}

			switch (tokenState)
			{
				case(jTagGate::None):
				{
					WriteToken();
					break;
				}
				case(jTagGate::Open):
				{
					if (context.insideBreakers)
					{
						WriteToken();
						IncTree();
					}
					else
					{
						IncTree();
						WriteToken();
					}
					break;
				}
				case(jTagGate::Close):
				{
					if (context.insideBreakers)
					{
						PopTree();
						WriteToken();
					}
					else
					{
						WriteToken();
						PopTree();
					}
					break;
				}
			}

			mFile.Next();

		}
	}

}









/*

V2
bool opening{ false };
			bool closing{ false };

			// The current context has this token marked as an override for a specific tag
			if (context.contextOverrides.contains(tk))
			{
				mStack.push(context.contextOverrides.at(tk));
				context = gContexts.at(mStack.top());
				opening = true;
			}
			// The current token is a universal keyword that has no overrides
			else if (gKeywordDescs.contains(tk))
			{
				mStack.push(gKeywordDescs.at(tk));
				context = gContexts.at(mStack.top());
				opening = true;
			}
			// The token might be the breaker keyword
			else if(tk == context.breaker)
			{
				closing = true;
				if (mStack.top() == jdesc::Statement)
				{
					const token& tk2{ mFile.PeekNext().content };
					if (tk2 == "else")
						closing = false;
				}
			}

			// Opening keyword found, but check for tag append or prepend
			if (opening)
			{
				// The opening tags go after the current symbol
				if (context.insideBreakers)
				{
					WriteToken();
					IncTree();
				}
				else
				{
					IncTree();
					WriteToken();
				}
			}
			else if (closing)
			{
				if (context.insideBreakers)
				{
					PopTree();
					WriteToken();
				}
				else
				{
					WriteToken();
					PopTree();
				}
			}
			// Standard token. Write it
			else
				WriteToken();

			mFile.Next();

*/

//if (context.contextOverrides.contains(tk)) // check for generic container keyword
			//{
			//	jcontext& nextContext{ gContexts.at(context.contextOverrides.at(tk)) };
			//	if (!nextContext.insideBreakers) // check if syntax wants the opener and closer brackets before or after the symbol starting it
			//	{
			//		IncTree(context.contextOverrides.at(tk));
			//		context = gContexts.at(context.contextOverrides.at(tk));
			//	}
			//	else
			//		insideBounds = true;
			//}

			//if (gKeywordDescs.contains(tk))
			//	IncTree(gKeywordDescs.at(tk));

			//if (mStack.top() == jdesc::Statement)
			//	if (tk == context.breaker)
			//		DecTree();
			//WriteToken();

			//if (insideBounds)
			//{
			//	IncTree(context.contextOverrides.at(tk));
			//	context = gContexts.at(context.contextOverrides.at(tk));
			//}

			//if (tk == context.breaker)
			//{
			//	if (mStack.top() == jdesc::IfStatement) // maybe add context continues? IfStatement might be the only weird one here
			//	{
			//		const token& tk2{ mFile.PeekNext().content };
			//		if (tk2 != "else")
			//			DecTree();
			//		else
			//			WriteToken();
			//	}
			//	else
			//		DecTree();
			//}
			// mFile.Next();
