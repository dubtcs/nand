
#include "haclus.h"

#include <format>

// Stack Operations

//inline vmins StackIncrement()	{ return "@SP\nA=M\nM=D\n@SP\nM=M+1\n"; }
//inline vmins StackDecrement()	{ return "@SP\nA=M\nM=D\n@SP\nM=M-1\n"; }
//inline vmins StackInto()		{ return "@SP\nA=M\nM=D\n"; }
//inline vmins StackFrom()		{ return "@SP\nA=M\nD=M\n"; }

// Memory Pool Operations
vmins ToAddress(const std::string& address)
{
	return std::format("@{}\n", address);
}
vmins ToConstant(const vmins& address)
{
	return std::format("@{}\n", address);
}
vmins ToLocal(const vmins& offset)
{
	return ToConstant(offset) + D_Address() + "@LCL\nA=A+D\n";
}
vmins ToArgument(const vmins& offset)
{
	return ToConstant(offset) + D_Address() + "@ARG\n" + RAdd(A_REG, A_REG, D_REG);
}
vmins ToTemp(const int32_t& index)
{
	return "@" + std::to_string(5 + index) + "\n";
}

// Arithmetic

vmins RArithmeticFormat(cvmins& dest, cvmins& r1, cvmins& r2)
{
	return (dest + "=" + r1 + r2 + "\n");
}
vmins RAdd(cvmins& dest, cvmins& r1, cvmins& r2)
{
	return RArithmeticFormat(dest, r1, ("+" + r2));
}
vmins RAdd1(cvmins& dest, cvmins& r1)
{
	return RArithmeticFormat(dest, r1, "+1");
}
vmins RSubtract(cvmins& dest, cvmins& r1, cvmins& r2)
{
	return RArithmeticFormat(dest, r1, ("-" + r2));
}
vmins RSubtract1(cvmins& dest, cvmins& r1)
{
	return RArithmeticFormat(dest, r1, "-1");
}
