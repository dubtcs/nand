#pragma once

#include <string>

#define BIT(x) (1 << x)

#define JACK_EXTENSION (".jack")

namespace jcom
{
	bool IsNumber(const std::string& str);
}
