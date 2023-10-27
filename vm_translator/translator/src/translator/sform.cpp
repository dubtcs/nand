
#include "sform.h"

void sform::Add(const std::string& add)
{
	mContent += add + "\n";
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
