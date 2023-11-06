
#include "jline.h"

#include <iostream>
#include <locale>

namespace jcom
{

	jline::jline(const std::string& line) :
		mContent{ line }
	{

	}

	bool jline::Next(jpair& pair)
	{
		token& target{ pair.content };
		target = "";
		pair.type = JackToken::None;

		bool stringStarted{ false };
		bool valid{ false };

		while (mCursor < mContent.size() && !valid)
		{
			const char& c{ mContent[mCursor] };
			if (!stringStarted)
			{
				if (c != ' ' && c != '\t')
				{
					if (gReservedTokens.contains(token{ c }))
					{
						if (target.empty())
						{
							pair.type = JackToken::Symbol;
							target = c;
							mCursor++;
						}
						valid = true;
					}
					else
					{
						if (c == '"') // flag as string but don't include the double quotes
						{
							stringStarted = true;
							pair.type = JackToken::String;
						}
						else
						{
							if (std::isdigit(c) && target.empty())
								pair.type = JackToken::Int;
							target += c;
						}
						if (gReservedTokens.contains(target)) // check if current string is a keyword
						{
							const char& cn{ mContent[mCursor + 1] }; // check if next character is a space or a symbol
							if (cn == ' ' || gReservedTokens.contains(token{ cn })) // if it is, this token is complete, otherwise, it's an identifier like intOne or somn
							{
								valid = true;
								pair.type = JackToken::Keyword;
							}
						}
						mCursor++;
					}
				}
				else
				{
					if (!target.empty())
						valid = true;
					mCursor++;
				}
			}
			else
			{
				if (c == '"' || c == '\n')
					valid = true;
				else
					target += c;
				mCursor++;
			}
			
		}
		if (pair.type == JackToken::None)
			pair.type = JackToken::Id;
		return (!target.empty() || mCursor < mContent.size());
	}

}
