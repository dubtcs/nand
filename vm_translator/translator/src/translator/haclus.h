#pragma once

// Hack assembly command clusters

#include <string>
#include <memory>

// Virtual machine assembly instruction packet
using vmins = std::string;

// Could use constexpr instead?
// But might stick to inline fucntions for parity
// eg { StackIncrement(); ToConstant(...) } vs { StackIncrement; ToConstant(...) }

// **********
//   INLINE
// **********

// Stack operations

inline vmins StackIncrement()	{ return "@SP\nM=M+1\n";	}	// Stack + 1
inline vmins StackDecrement()	{ return "@SP\nM=M-1\n";	}	// Stack - 1
inline vmins StackInto()		{ return "@SP\nA=M\nM=D\n";	}	// Push D register into stack
inline vmins StackFrom()		{ return "@SP\nA=M\nD=M\n";	}	// Pop memory data into D register	

// Memory pool operations

inline vmins ToStack()			{ return "@SP\n"; }
inline vmins ToArguments()		{ return "@ARG\n"; }
inline vmins ToThis()			{ return "@THIS\n"; }
inline vmins ToThat()			{ return "@THAT\n"; }
inline vmins ToLocalBase()		{ return "@LCL\n"; }

// D register operations

inline vmins D_Memory()			{ return "D=M\n"; }
inline vmins D_Address()		{ return "D=A\n"; }

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

vmins ToConstant(const vmins& address);	// Go to address of constant
vmins ToLocal(const vmins& offset);		// Go to local address
vmins ToTemp(const int32_t& index);
vmins ToStatic();						// Go to address of static variable
