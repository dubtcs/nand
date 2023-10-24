
#include "haclus.h"
#include "sform.h"

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
vmins ToStatic(const vmins& offset)
{
	return ToConstant(offset) + D_Address() + ToStatic() + RAdd(A_REG, D_REG, A_REG);
}

// Arithmetic

vmins RArithmeticFormat(cvmins& dest, cvmins& r1, cvmins& r2)
{
	return ((dest.empty() ? ("") : (dest + "=")) + r1 + r2 + "\n");
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
vmins RNot(cvmins& dest, cvmins& r)
{
	return RArithmeticFormat(dest, "!", r);
}
vmins RNegate(cvmins& dest, cvmins& r1)
{
	// ((-x) - (-x)) - ((-x) - (-x))
	return RArithmeticFormat(dest, "-", r1);
}

vmins RGreaterThan(int32_t lc)
{
	sform cmd{};

	const std::string lTrue{ "gtTrue_" + std::to_string(lc) };
	const std::string lSkip{ "gtSkip_" + std::to_string(lc) };

	// -1 == true
	// x > y
	// D contains x
	// M contains y
	// 
	// D=D-M
	// @gtTrue_##
	// D;JGT
	// D=0
	// @gtSkip_##
	// 0;JMP
	// (gtTrue_##)
	// D=-1
	// (gtSkip_##)
	//

	cmd.Add("D=D-M");
	cmd.Add("@" + lTrue);
	cmd.Add("D;JGT");
	cmd.Add("D=0");
	cmd.Add("@" + lSkip);
	cmd.Add("0;JMP");
	cmd.Add("(" + lTrue + ")");
	cmd.Add("D=-1");
	cmd.Add("(" + lSkip + ")");

	return cmd.GetContent();

}
vmins RLessThan(int32_t lc)
{
	sform cmd{};

	const std::string lTrue{ "ltTrue_" + std::to_string(lc) };
	const std::string lSkip{ "ltSkip_" + std::to_string(lc) };

	cmd.Add("D=D-M");
	cmd.Add("@" + lTrue);
	cmd.Add("D;JLT");
	cmd.Add("D=0");
	cmd.Add("@" + lSkip);
	cmd.Add("0;JMP");
	cmd.Add("(" + lTrue + ")");
	cmd.Add("D=-1");
	cmd.Add("(" + lSkip + ")");

	return cmd.GetContent();

}

vmins REqual(int32_t lc)
{
	sform cmd{};

	const std::string lTrue{ "eqTrue_" + std::to_string(lc) };
	const std::string lSkip{ "eqSkip_" + std::to_string(lc) };

	cmd.Add("D=D-M");
	cmd.Add("@" + lTrue);
	cmd.Add("D;JEQ");
	cmd.Add("D=0");
	cmd.Add("@" + lSkip);
	cmd.Add("0;JMP");
	cmd.Add("(" + lTrue + ")");
	cmd.Add("D=-1");
	cmd.Add("(" + lSkip + ")");

	return cmd.GetContent();
}
