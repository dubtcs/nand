#pragma once

// Hack assembly command clusters

#include <string>
#include <memory>

// Virtual machine assembly instruction packet
using vmins = std::string;
using cvmins = const vmins;

// Could use constexpr instead?
// But might stick to inline fucntions for parity
// eg { StackIncrement(); ToConstant(...) } vs { StackIncrement; ToConstant(...) }

// Macros

#define D_REG ("D")
#define A_REG ("A")
#define M_REG ("M")

#define H_CONST ("constant")
#define H_LOCAL ("local")
#define H_ARGS ("argument")
#define H_THIS ("this")
#define H_THAT ("that")
#define H_STATIC ("static")
#define H_TEMP ("temp")

// **********
//   INLINE
// **********

// Stack operations

inline vmins StackIncrement()	{ return "@SP\nM=M+1\n";	}	// Stack + 1
inline vmins StackDecrement()	{ return "@SP\nM=M-1\n";	}	// Stack - 1
inline vmins StackInto()		{ return "@SP\nA=M\nM=D\n";	}	// Push D register into stack
inline vmins StackFrom()		{ return "@SP\nA=M\nD=M\n";	}	// Pop memory data into D register	

// Memory pool operations

inline vmins ToStack()			{ return "@SP\n";	}
inline vmins ToArgument()		{ return "@ARG\n";	}
inline vmins ToThis()			{ return "@THIS\n"; }
inline vmins ToThat()			{ return "@THAT\n"; }
inline vmins ToLocalBase()		{ return "@LCL\n";	}

// D register operations

inline vmins D_Memory()			{ return "D=M\n";	}
inline vmins D_Address()		{ return "D=A\n";	}

// A register operations

inline vmins A_Memory()			{ return "A=M\n"; }
inline vmins A_Data()			{ return "A=D\n"; }

// M register operations

inline vmins M_Address()		{ return "M=A\n"; }
inline vmins M_Data()			{ return "M=D\n"; }

// **********
// NON INLINE
// **********

// Memory pool operations

vmins ToAddress(const std::string& address);
vmins ToConstant(const vmins& address);	// Go to address of constant
vmins ToLocal(const vmins& offset);		// Go to local address
vmins ToArgument(const vmins& offset);
vmins ToTemp(const int32_t& index);
vmins ToStatic();						// Go to address of static variable

// Arithmetic

vmins RAdd(cvmins& dest, cvmins& r1, cvmins& r2);
vmins RAdd1(cvmins& dest, cvmins& r1);
vmins RSubtract(cvmins& dest, cvmins& r1, cvmins& r2);
vmins RSubtract1(cvmins& dest, cvmins& r1);
