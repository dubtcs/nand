
#include "jfile.h"

#include <iostream>

namespace jcom
{

	jfile::jfile(std::ifstream& f) :
		mFile{ f }
	{
		Next();
	}

	bool jfile::Next()
	{

		mCurrent = jpair{};

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

						if (gKeywords.contains(token{ c }))
						{
							if (str.empty())
							{
								mCurrent.type = JackToken::Symbol;
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
								mCurrent.type = JackToken::String;
							}
							else
							{
								if (std::isdigit(c) && str.empty())
									mCurrent.type = JackToken::Int;
								str += c;
							}
							if (gKeywords.contains(str)) // check if current string is a keyword
							{
								const char& cn{ mContent[mCursor + 1] }; // check if next character is a space or a symbol
								if (cn == ' ' || gKeywords.contains(token{ cn })) // if it is, this token is complete, otherwise, it's an identifier like intOne or somn
								{
									found = true;
									mCurrent.type = JackToken::Keyword;
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

			mCurrent.content = str;
		}

		if (mCurrent.type == JackToken::None && (!mCurrent.content.empty()))
			mCurrent.type = JackToken::Id; // nothing else but Identifier

		return mCurrent.type != JackToken::None;
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

	jpair& jfile::Get()
	{
		return mCurrent;
	}

	bool jfile::Available()
	{
		return !mFile.eof();
	}

}
