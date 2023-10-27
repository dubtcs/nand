#pragma once

#include <string>

// Hack string formatter
class sform
{
public:
	sform() = default;
	sform(const std::string& str);
public:
	void Add(const std::string& addition);
	const std::string& GetContent();
	sform& operator+=(const std::string& str);
protected:
	std::string mContent{};
};
