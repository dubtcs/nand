#pragma once

#include <string>

// Hack string formatter
class sform
{
public:
	void Add(const std::string& addition);
	const std::string& GetContent();
protected:
	std::string mContent{};
};
