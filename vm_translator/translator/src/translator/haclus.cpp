
#include "haclus.h"

#include <format>

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
	return ToConstant(offset) + D_Address() + ToLocalBase() + RAdd(A_REG, M_REG, D_REG);;
}
vmins ToArgument(const vmins& offset)
{
	return ToConstant(offset) + D_Address() + ToArgument() + RAdd(A_REG, M_REG, D_REG);
}
vmins ToTemp(const int32_t& index)
{
	return "@" + std::to_string(5 + index) + "\n";
}
vmins ToThis(const vmins& offset)
{
	return ToConstant(offset) + D_Address() + ToThis() + RAdd(A_REG, M_REG, D_REG);
}
vmins ToThat(const vmins& offset)
{
	return ToConstant(offset) + D_Address() + ToThat() + RAdd(A_REG, M_REG, D_REG);
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
vmins RAnd(cvmins& dest, cvmins& r1, cvmins& r2)
{
	return RArithmeticFormat(dest, r1, ("&" + r2));
}
vmins ROr(cvmins& dest, cvmins& r1, cvmins& r2)
{
	return RArithmeticFormat(dest, r1, ("|" + r2));
}
