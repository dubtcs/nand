
#include "smta.h"

namespace jcom
{

	std::unordered_map<token, jpool> gTokenToPool
	{
		{"field",	jpool::THIS},
		{"static",	jpool::STATIC},
		{"arg",		jpool::ARG},
		{"var",		jpool::LOCAL}
	};

	std::unordered_map<jpool, token> gPoolToToken
	{
		{jpool::CONST,	"constant"},
		{jpool::STATIC, "static"},
		{jpool::TEMP,	"temp"},
		{jpool::ARG,	"argument"},
		{jpool::LOCAL,	"local"},
		{jpool::THIS,	"this"},
		{jpool::POINTER,"pointer"}
	};

	symtable::symtable()
	{

	}

	void symtable::Define(const token& str, const token& classType, jpool pool)
	{
		if (!mPoolCount.contains(pool))
			mPoolCount.emplace(pool, 0);
		int32_t index{ mPoolCount.at(pool)++ };
		mInfo.insert({ str, { classType, pool, index } });
	}

	int32_t symtable::GetPoolCount(jpool pool) const
	{
		if (mPoolCount.contains(pool))
			return mPoolCount.at(pool);
		return 0;
	}

	bool symtable::Contains(const token& str) const
	{
		return mInfo.contains(str);
	}

	const syminfo& symtable::GetInfo(const token& str) const
	{
		return mInfo.at(str);
	}
	const token& symtable::GetType(const token& str) const
	{
		return GetInfo(str).classType;
	}
	jpool symtable::GetPool(const token& str) const
	{
		return GetInfo(str).pool;
	}
	int32_t symtable::GetCount(const token& str) const
	{
		return GetInfo(str).index;
	}

}
