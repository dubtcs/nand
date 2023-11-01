
#include "sform.h"

static int32_t gLinecount{ 0 };

void sform::AddRaw(const std::string& add)
{
	mContent += add + "\n";
}
void sform::Add(const std::string& add)
{
	AddRaw(add + "\t\t\t// " + std::to_string(gLinecount++));
}
void sform::Comment(const std::string& add)
{
	AddRaw(add);
}
sform& sform::operator+=(const std::string& str)
{
	Add(str);
	return *this;
}
sform::sform(const std::string& str) :
	mContent{ str }
{

}
const std::string& sform::GetContent()
{
	return mContent;
}
