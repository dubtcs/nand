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
	void Comment(const std::string& comment);
	const std::string& GetContent();
	sform& operator+=(const std::string& str);
protected:
	void AddRaw(const std::string& raw);
	std::string mContent{};
};
