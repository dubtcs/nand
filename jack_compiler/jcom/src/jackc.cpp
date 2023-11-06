
#include "jackc.h"

#include <algorithm>

namespace jcom
{

	bool IsNumber(const std::string& str)
	{
		return std::all_of(str.begin(), str.end(), ::isdigit);
	}

}
