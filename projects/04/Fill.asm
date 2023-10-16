// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.


// 512 x 256
// Memory Layout
// RAM
// SCREEN
// KBD

// Program Start
@iScreenValue
M=0;

(CHECKINPUT)
    @iRegister // Current screen 16 bit register
    M=0;

    @KBD
    D=M;
    
    @KEYPRESSED
    D;JNE

    @iScreenValue
    D=M;
    @CHECKINPUT
    D;JEQ

    // Only getting here if it iScreenValue has changed
    @iScreenValue
    M=0;
    @FILL
    0;JMP

(KEYPRESSED)
    @iScreenValue
    D=M;
    // Screen already black, go back to main
    @CHECKINPUT
    D;JNE
    // Set screen value to black, call SETSCREEN
    @iScreenValue
    M=-1;
    @FILL
    0;JMP

(FILL)
    @SCREEN
    D=A;
    @iRegister
    M=D;
(FILL2)
    @iRegister
    D=M;
    @KBD
    D=A-D;
    @CHECKINPUT / /jump to main if it's at or past KBD
    D;JLE
    @iScreenValue
    D=M;
    @iRegister
    A=M;
    M=D;
    @iRegister
    M=M+1;
    @FILL2
    0;JMP
