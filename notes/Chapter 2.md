---
tags: notes, investigation, study, c, c-projects, buildyourownlisp
---
### Basic Notes
- Compilers transform C into a program a PC can use
- `sudo apt-get install build-essential` - used to install compiler for ubuntu
- Code
	- `header` - Allows a library to be used for functions
	- `int main` - The necessary function for all C programs
	- `puts` - *put string*: prints a string to the stdout
	- `return 0` - Completes the running of the program
 - Compile 
	- `-o` - *output*: arg becomes output file name
	- `-Wall` - *warnings,all*: reports all warnings at compile
- Errors
	- Errors will usually be simple enough to understand
	- Errors will stem from a single source often -- always follow from first error to last
	- Compiler may finish, but the program may crash -- debugging is hard in C
		- Try to isolate the source through puts, etc
		- use `gdb` 
- Documentation
	- Standard library is explained online in the documentation (Resources)
- Bonus Marks
	- If no main function is given, the compiler throws an error
	- `puts` - int puts( const char \*str ); 
		- Writes every character from a null-terminated string and a newline character `\n`. 
	- [[gdb]]: To review -- study docs included
### Analysis
Gives a very basic explanation of the basics of the MVP.
There's quite a good find with the debugger in the bonus, would be worth a read.
### Resources
[[C Projects/Build Your Own Lisp/Plan]]
https://buildyourownlisp.com/chapter2_installation 
http://web.archive.org/web/20140910051410/http://www.dirac.org/linux/gdb/ 
http://en.cppreference.com/w/c
https://en.cppreference.com/w/cpp/io/c/puts