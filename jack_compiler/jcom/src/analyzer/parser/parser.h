#pragma once

#include <jackc.h>

#include "../token/tokens.h"
#include "../token/jfile.h"

#include <unordered_map>
#include <functional>
#include <fstream>

namespace jcom
{

	void ParseClass();

	extern std::unordered_map<token, std::function<void()>> gNTRules; // non terminal rule functions

}
