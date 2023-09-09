---
tags: notes, investigation, study, c, c-projects, buildyourownlisp
---
### Basic Notes
- C uses a compiler, which means if we want to change the program, we have to recompile each time we change something to run the new version
- An interactive prompt allows the option to dynamically interact with the language
- REPL - Read, Evaluate, Print Loop: a common way of interacting with languages
- This can me extended to parse user input and evaluate it as if it were an actual Lisp program
- Interactive Prompt
	- `static char input[2048]` declares a global array of 2048 characters; a reserved block of data accessible anywhere in the program
	- `while (1)` will cause an infinite loop
	- `fputs` is `puts` but does not add the newline character
	- `fgets` takes user input from the command line
		- both f functions require a file to read and write from: `stdout` and `stdin`
	- `printf` allows arguments to match patterns that will interpolate variables via interpretation (%s will take the argument and interpret it as a string)
- Compilation should be run with this command:
```c
cc -std=c99 -Wall prompt.c -ledit -o prompt
```
- `-Wall` enables all compiler warning messages
- `-ledit` links the compiler to embed calls to editline in the program
	- `readline` strips the trailing newline character from input
	- `readline` allocates new memory when it is called, meaning it must be `free`'d
- C Preprocessor
	- C has a general problem with portability, and does not always have a easy or correct solution, but the preprocessor can help
	- The preprocessor is a program that runs before the compiler
	- Any line that starts with an octothorpe (#) will be used by the preprocessor to access the standard library and others
	- It can also detect which OS the code is being compiled on, ond use this to emit different code

|`#def`| Defines an `identifier` to replace all occurences of itself with a replacement list |
|---|---|
|`#ifdef`| Tests a specified condition is defined, then compiles the controlled block if true |
|`#ifndef`| Tests a specified condition is defined, then compiles the controlled block if false|
|`#else`| Compiles controlled block when condition is not met |
|`#endif`| Closes controlled `if` block |

### Bonus
*What does \\n mean in those strings?*
- It's the newline character, which causes terminals to move the following line to be printed on the next line

*What other patterns can be used with `printf`?*

|`Specifier` |Explanation | `Expected Arg Types`|
|---|---|---|
|`c`| Writes a single character  |`int`|
|`s`| Writes a character string |`char*`|
|`d` |Converts a signed integer into a decimal representation |`int`|
|`o` |Converts an unsigned integer into an octal representation |`unsigned int`|
|`x` |Converts an unsigned integer into a hexadecimal representation |`unsigned int`|
|`u` |Converts an unsigned integer into a decimal representation |`unsigned int`|
|`f` |Converts a floating point number into the decimal notation in the style [-]ddd.ddd  |`double`|
|`e` |Converts a floating point number into the decimal exponent notation |`double`|
|`a` |Converts a floating point number into the hexadecimal notation  |`double`|
|`g` |Converts a floating point number into decimal or decimal exponent notation depending on value and precision  |`double`|
|`n` |Returns the number of characters written so far by this call to the function |`int*`|
|`p` |Writes an implementation defined character sequence defining a pointer |`void*`|

*What happens when you pass `printf` a variable that does not match the pattern?*
- Erroneous output, causing undefined behaviour: likely compiler error

*If `_WIN32` is defined on windows, what is defined for Linux and Mac?*
- \__linux\__ ,\__APPLE\__
### Analysis
Preprocessor functions seem like a very deep hole to get into considering the amount of different operating systems. Looking at the patterns page definitely made me understand the value in reading documentation thoroughly.
### Resources
https://buildyourownlisp.com/chapter4_interactive_prompt
https://cplusplus.com/reference/cstdio/printf/ - For an explanation on length modifiers
https://en.cppreference.com/w/c/io/fprintf - For specifiers