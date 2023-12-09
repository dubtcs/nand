
# Jack Compiler

A compiler for the JACK language.

## How To Use

Compiler is a simple console application. Call `start jcom <filename/directory>` from inside the translator directory, and it will compile a jack program.

The compiler can compile both individual files and entire directories. One *.vm file is creating for each *.jack file.

Two versions of the compiler are present:
 - `jcom-xml` outputs a formatted *.xml file of the supplied code.
 - `jcom` outputs a *.vm file of the supplied code that works with the JACK virtual machine.

## Download + Build

### Requirements 

 - Windows OS
 - C++20
 - (Suggested) Visual Studio 2022

### Downloading 

1. Download the repository with `git clone https://github.com/dubtcs/nand`

2. Run `GenerateSolutions.bat` to create VS2022 solution and project files.
    - You can use this any time you make changes to the premake5.lua file or just want new solutions.
