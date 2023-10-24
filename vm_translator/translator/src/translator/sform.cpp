
#include "sform.h"

void sform::Add(const std::string& add)
{
	mContent += add + "\n";
}
const std::string& sform::GetContent()
{
	return mContent;
}
