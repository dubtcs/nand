
#include "jline.h"

#include <iostream>

namespace jcom
{

	jline::jline(const std::string& line) :
		mContent{ line }
	{

	}

	bool jline::Next(token& target)
	{
		target = "";
		bool valid{ false };
		while (mCursor < mContent.size() && !valid)
		{
			const char& c{ mContent[mCursor] };
			if (c != ' ')
			{
				if (gReservedTokens.contains(token{ c }))
				{
					if (target.empty())
					{
						target = c;
						mCursor++;
					}
					valid = true;
				}
				else
				{
					target += c;
					if (gReservedTokens.contains(target))
						valid = true;
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
		return (!target.empty() || mCursor < mContent.size());
	}

}
