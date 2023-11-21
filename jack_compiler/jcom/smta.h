#pragma once

#include <jcom/jcom.h>

#include <unordered_map>
#include <memory>

namespace jcom
{

	// Virtual memory segments
	enum class jpool
	{
		NONE, CONST, STATIC, FIELD, VAR, ARG, THIS, THAT, POINTER, TEMP
	};

	struct syminfo
	{
		token classType;
		jpool pool{ jpool::NONE };
		int32_t index{ 0 };
	};

	class symtable
	{
	public:
		symtable();
	public:
		void Define(const token& str, const token& classType, jpool pool);
		bool Contains(const token& str) const;
		int32_t GetPoolCount(jpool pool) const;
		const token& GetType(const token& str) const;
		jpool GetPool(const token& str) const;
		int32_t GetCount(const token& str) const;
		const syminfo& GetInfo(const token& str) const;
	protected:
		std::unordered_map<jpool, int32_t> mPoolCount{};
		std::unordered_map<token, syminfo> mInfo{};
		//std::shared_ptr<symtable> mPrevious{ nullptr }; // previous scope's symtable. Basically a linked list.
		// nvm there is at max 2 scopes. One for class, one for subroutine
	};

}
