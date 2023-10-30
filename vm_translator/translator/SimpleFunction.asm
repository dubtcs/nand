// function SimpleFunction.test 2
(SimpleFunction.test)
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

@0
D=A
@SP
A=M
M=D
@SP
M=M+1


// push local 0
D=0
@LCL
A=D+M
D=M
@SP
A=M
M=D
@SP
M=M+1


// push local 1
@1
D=A
@LCL
A=D+M
D=M
@SP
A=M
M=D
@SP
M=M+1


// add
@SP
A=M-1
A=A-1
D=M
@SP
A=M-1
D=D+M
@SP
AM=M-1
A=A-1
M=D

// not
@SP
A=M-1
D=M
D=!D
M=D
@SP
A=M
A=A-1
M=D

// push argument 0
D=0
@ARG
A=D+M
D=M
@SP
A=M
M=D
@SP
M=M+1


// add
@SP
A=M-1
A=A-1
D=M
@SP
A=M-1
D=D+M
@SP
AM=M-1
A=A-1
M=D

// push argument 1
@1
D=A
@ARG
A=D+M
D=M
@SP
A=M
M=D
@SP
M=M+1


// sub
@SP
A=M-1
A=A-1
D=M
@SP
A=M-1
D=D-M
@SP
AM=M-1
A=A-1
M=D

// return
@LCL
D=M
@5
M=D
D=D-A
@6
M=D
@SP
AM=M-1
D=M
@ARG
A=M
M=D
D=A+1
@SP
M=D
@5
D=M-1
@4
M=D
@5
D=M
@2
D=D-A
@3
M=D
@5
D=M
@3
D=D-A
@ARG
M=D
@5
D=M
@4
D=D-A
@LCL
M=D
@6
A=M
0;JMP

