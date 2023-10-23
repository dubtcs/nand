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
@ARG
D=D+M
@2
D=D+A
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
@ARG
D=D+M
@1
D=D+A
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
@THIS
D=D+M
@6
D=D+A
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
@THAT
D=D+M
@5
D=D+A
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
@THAT
D=D+M
@2
D=D+A
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
D=D+A
@6
D=D+A
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
@5
D=A
@THAT
A=A+D
D=M
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
M=M-1
A=M-1
M=D

// push argument 1
@1
D=A
@ARG
A=M+D
D=M
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
M=M-1
A=M-1
M=D

// push this 6
@6
D=A
@THIS
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1

// push this 6
@6
D=A
@THIS
A=A+D
D=M
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
M=M-1
A=M-1
M=D

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
M=M-1
A=M-1
M=D

// push temp 6
@11
D=M
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
M=M-1
A=M-1
M=D

