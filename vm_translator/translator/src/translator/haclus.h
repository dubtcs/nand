#pragma once

// Hack assembly command clusters
#include <string>

// Virtual machine assembly instruction packet
using vmins = std::string;

// Could use constexpr instead?
// But might stick to inline fucntions for parity
// eg { StackIncrement(); ToConstant(...) } vs { StackIncrement; ToConstant(...) }

// **********
//   INLINE
// **********

// Stack operations

inline vmins StackIncrement()	{ return "@SP\nA=M\nM=D\n@SP\nM=M+1\n"; }	// Stack + 1
inline vmins StackDecrement()	{ return "@SP\nA=M\nM=D\n@SP\nM=M-1\n"; }	// Stack - 1
inline vmins StackInto()		{ return "@SP\nA=M\nM=D\n";				}	// Push D register into stack
inline vmins StackFrom()		{ return "@SP\nA=M\nD=M\n";				}	// Pop memory data into D register	

// Memory pool operations

inline vmins ToStack()			{ return "@SP\n"; }
inline vmins ToLocal()			{ return "@LCL\n"; }
inline vmins ToArguments()		{ return "@ARG\n"; }
inline vmins ToThis()			{ return "@THIS\n"; }
inline vmins ToThat()			{ return "@THAT\n"; }

// D register operations

inline vmins D_Memory()			{ return "D=M\n"; }
inline vmins D_Address()		{ return "D=A\n"; }

// A register operations

inline vmins A_Memory()			{ return "A=M\n"; }
inline vmins A_Data()			{ return "A=D\n"; }

// **********
// NON INLINE
// **********

// Memory pool operations

vmins ToConstant(const vmins& address);	// Go to address of constant
//vmins ToStatic();						// Go to address of static variable

// A register operations

//vmins A_Increment();	// Address + 1
//vmins A_Decrement();	// Address - 1
