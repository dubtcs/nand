
# Jack Compiler

A compiler for the JACK language.

## How To Use

Translator is a simple console application. Call `start jcom <filename/directory>` from inside the translator directory, and it will compile a jack program.

The compiler can compile both individual files and entire directories. One *.xml file will be output for each *.jack file present.

## Download + Build

### Requirements 

 - Windows OS
 - C++20
 - (Suggested) Visual Studio 2022

### Downloading 

1. Download the repository with `git clone https://github.com/dubtcs/nand`

2. Run `GenerateSolutions.bat` to create VS2022 solution and project files.
    - You can use this any time you make changes to the premake5.lua file or just want new solutions.
