// push constant 10
@10
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop local 0
@SP
M=M-1
@SP
A=M
D=M
@5
M=D
@LCL
D=D+M
@0
D=D+A
@SP
A=M
A=M
A=D-A
M=D-A

// push constant 21
@21
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 22
@22
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop argument 2
@SP
M=M-1
@SP
A=M
D=M
@5
M=D
@SP
A=M
A=M
A=D-A
M=D-A

// pop argument 1
@SP
M=M-1
@SP
A=M
D=M
@5
M=D
@SP
A=M
A=M
A=D-A
M=D-A

// push constant 36
@36
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop this 6
@SP
M=M-1
@SP
A=M
D=M
@5
M=D
@SP
A=M
A=M
A=D-A
M=D-A

// push constant 42
@42
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 45
@45
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop that 5
@SP
M=M-1
@SP
A=M
D=M
@5
M=D
@SP
A=M
A=M
A=D-A
M=D-A

// pop that 2
@SP
M=M-1
@SP
A=M
D=M
@5
M=D
@SP
A=M
A=M
A=D-A
M=D-A

// push constant 510
@510
D=A
@SP
A=M
M=D
@SP
M=M+1

// pop temp 6
@SP
M=M-1
@SP
A=M
D=M
@5
M=D
@SP
A=M
A=M
A=D-A
M=D-A

// push local 0
@0
D=A
@LCL
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1

// push that 5
@SP
A=M
M=D
@SP
M=M+1

// add

// push argument 1
@SP
A=M
M=D
@SP
M=M+1

// sub

// push this 6
@SP
A=M
M=D
@SP
M=M+1

// push this 6
@SP
A=M
M=D
@SP
M=M+1

// add

// sub

// push temp 6
@SP
A=M
M=D
@SP
M=M+1

// add

