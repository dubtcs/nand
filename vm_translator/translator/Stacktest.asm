// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// eq
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@eqTrue_0
D;JEQ
D=0
@eqSkip_0
0;JMP
(eqTrue_0)
D=-1
(eqSkip_0)

@SP
AM=M-1
A=A-1
M=D

// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1

// eq
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@eqTrue_1
D;JEQ
D=0
@eqSkip_1
0;JMP
(eqTrue_1)
D=-1
(eqSkip_1)

@SP
AM=M-1
A=A-1
M=D

// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// eq
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@eqTrue_2
D;JEQ
D=0
@eqSkip_2
0;JMP
(eqTrue_2)
D=-1
(eqSkip_2)

@SP
AM=M-1
A=A-1
M=D

// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// lt
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@ltTrue_3
D;JLT
D=0
@ltSkip_3
0;JMP
(ltTrue_3)
D=-1
(ltSkip_3)

@SP
AM=M-1
A=A-1
M=D

// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1

// lt
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@ltTrue_4
D;JLT
D=0
@ltSkip_4
0;JMP
(ltTrue_4)
D=-1
(ltSkip_4)

@SP
AM=M-1
A=A-1
M=D

// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// lt
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@ltTrue_5
D;JLT
D=0
@ltSkip_5
0;JMP
(ltTrue_5)
D=-1
(ltSkip_5)

@SP
AM=M-1
A=A-1
M=D

// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// gt
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@gtTrue_6
D;JGT
D=0
@gtSkip_6
0;JMP
(gtTrue_6)
D=-1
(gtSkip_6)

@SP
AM=M-1
A=A-1
M=D

// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1

// gt
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@gtTrue_7
D;JGT
D=0
@gtSkip_7
0;JMP
(gtTrue_7)
D=-1
(gtSkip_7)

@SP
AM=M-1
A=A-1
M=D

// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// gt
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@gtTrue_8
D;JGT
D=0
@gtSkip_8
0;JMP
(gtTrue_8)
D=-1
(gtSkip_8)

@SP
AM=M-1
A=A-1
M=D

// push constant 57
@57
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 31
@31
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 53
@53
D=A
@SP
A=M
M=D
@SP
M=M+1

// add
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D+M
@SP
AM=M-1
A=A-1
M=D

// push constant 112
@112
D=A
@SP
A=M
M=D
@SP
M=M+1

// sub
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D-M
@SP
AM=M-1
A=A-1
M=D

// neg
@SP
A=M
A=A-1
D=M
D=-D
M=D
@SP
AM=M-1
A=A-1
M=D

// and
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D&M
@SP
AM=M-1
A=A-1
M=D

// push constant 82
@82
D=A
@SP
A=M
M=D
@SP
M=M+1

// or
@SP
A=M
A=A-1
A=A-1
D=M
@SP
A=M
A=A-1
D=D|M
@SP
AM=M-1
A=A-1
M=D

// not
@SP
A=M
A=A-1
D=M
D=!D
M=D
@SP
AM=M-1
A=A-1
M=D

