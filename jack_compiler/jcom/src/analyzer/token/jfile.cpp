
#include "jfile.h"

#include <iostream>

namespace jcom
{

	jfile::jfile(std::ifstream& f) :
		mFile{ f }
	{

	}

	bool jfile::NextToken(jpair& pair)
	{

		pair.type = JackToken::None;
		pair.content = "";

		// Resetting cursor and line
		if (CheckContent())
		{
			token str{};

			bool found{ false };
			bool isString{ false };

			while ((!found) && (mCursor < mContent.size()) && (CheckContent()))
			{
				const char& c{ mContent[mCursor] };
				if (!isString)
				{
					if (c != ' ' && c != '\t')
					{

						if (c == '/') // comments
						{
							if (mContent[mCursor + 1] == '/')
							{
								mCursor = mContent.size();
								CheckContent();
								continue;
							}
						}

						if (gReservedTokens.contains(token{ c }))
						{
							if (str.empty())
							{
								pair.type = JackToken::Symbol;
								str = c;
								mCursor++;
							}
							found = true;
						}
						else
						{
							if (c == '"') // flag as string but don't include the double quotes
							{
								isString = true;
								pair.type = JackToken::String;
							}
							else
							{
								if (std::isdigit(c) && str.empty())
									pair.type = JackToken::Int;
								str += c;
							}
							if (gReservedTokens.contains(str)) // check if current string is a keyword
							{
								const char& cn{ mContent[mCursor + 1] }; // check if next character is a space or a symbol
								if (cn == ' ' || gReservedTokens.contains(token{ cn })) // if it is, this token is complete, otherwise, it's an identifier like intOne or somn
								{
									found = true;
									pair.type = JackToken::Keyword;
								}
							}
							mCursor++;
						}
					}
					else
					{
						if (!str.empty())
							found = true;
						mCursor++;
					}
				}
				else
				{
					if (c == '"' || c == '\n')
						found = true;
					else
						str += c;
					mCursor++;
				}
			}

			pair.content = str;
		}

		if (pair.type == JackToken::None && (!pair.content.empty()))
			pair.type = JackToken::Id; // nothing else but Identifier

		mCurrent = pair;
		return pair.type != JackToken::None;
	}

	bool jfile::CheckContent()
	{
		if (mCursor >= mContent.size())
		{
			mCursor = 0;
			std::getline(mFile, mContent);
			while (mContent.empty() && !mFile.eof())
				std::getline(mFile, mContent);
			return !mFile.eof();
		}
		return true;
	}

	jpair jfile::GetCurrent()
	{
		return mCurrent;
	}

}
