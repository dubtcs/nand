
#include "haclus.h"

#include <format>

// Stack Operations

//inline vmins StackIncrement()	{ return "@SP\nA=M\nM=D\n@SP\nM=M+1\n"; }
//inline vmins StackDecrement()	{ return "@SP\nA=M\nM=D\n@SP\nM=M-1\n"; }
//inline vmins StackInto()		{ return "@SP\nA=M\nM=D\n"; }
//inline vmins StackFrom()		{ return "@SP\nA=M\nD=M\n"; }

// Memory Pool Operations

vmins ToConstant(const vmins& address)
{
	return std::format("@{}\n", address);
}
vmins ToLocal(const vmins& offset)
{
	return ToConstant(offset) + D_Address() + "@LCL\nA=A+D\n";
}

vmins ToTemp(const int32_t& index)
{
	return "@" + std::to_string(5 + index) + "\n";
}