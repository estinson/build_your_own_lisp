---
tags: notes, investigation, study, c, c-projects, buildyourownlisp
---
### Basic Notes
- C is very simple, but not easy
- C is made of only function definitions and structure definitions: functions and types
- Functions can be called from other libraries or datatypes
- Functions in C manipulate variables, items of data with names
- Varibles
	- Every variable has an explicit type
	- types are either built into the language, or declared by the coder
	- varibles can be declared with or without values
- Built-in Types

|`void`|Empty type| Examples |Value Range| Memory|
|---|---|---|---|---|
|`char`| Single Character/Byte|`char last_initial = 'H'`| -128 to 127 or 0 to 255 |1 byte
|`int`| Integer | `int age = 23`|-32768 to 32767 or -2147483648 to 2147483647|2-4 bytes
|`long`| Integer that can hold longer values | `age_of_the_universe = 13798000000` |-9223372036854775908 to 9223372036854775907 | 8 bytes
|`float`| Decimal number |`float liters_per_pint = 0.568f` |1.2E-38 to 3.4E+38 (6 decimal places)| 4 bytes
|`double`| Decimal Number with more precision |`double speed_of_swallow = 0.01072896` |2.3E-308 to 1.7E+308 (15 decimal places)| 8 bytes

- Functions
	- A computation that manipulates variables, and optionally changes the state of the program
	- Takes variables as input and returns a singular variable as output

|declare | *return-type function_name(list, of, variables) {code block to execute; return statement}*| 
|---|---|
|call | *function_name(arguments)* | 

- Structures
	- Used to declare new types: several variables bundled together into a single space
	- Represents more complex data types

|declare | *typdef struct_name{list; of; variables;}*| 
|---|---|
|call | *struct_name struct_variable; struct_variable.list = 1; struct_variable.of = 2;* | 

- Strings
	- Represented by pointer type `char*` terminated by a null terminator
	- Can be declared literally by putting text between quotation marks
	- `char* string = "Hello World!"`
- Conditionals 
	- Lets the program perform some code only if conditions are met

|`if`| condition that executes code if value is true| 
|---|---|
|`else`| condition that executes code if value is false| 
``` C
if (condition) {
	do this if true;
} else {
	do this if false; 
}
```
- Loops 
	- Allow code to be repeated until a condition becomes false or a counter elapses

|`for` | `while` |
|---|---|
|`for(initializer; condition; incrementer){looped code}` | `while(condition) {looped code}` |
### Analysis
Just the basics of coding, but good to see the proper syntax of all these in C.

### Bonus
Questions 1-3
```c
#include <stdio.h>
int main(void){
	
	int w_loop_condition = 5;
	
	for(int i=0; i<6; i++){
		puts("Hello World!");
	}
	
	while(w_loop_condition > 0){
		puts("Hello World!");
		w_loop_condition -= 1;
	}
	
	print_hello_world(5);
}

void print_hello_world(int times){
	for(int i=0; i<times+1; i++){
		puts("Hello World!")
	}
}
```
*What built in types are there other than the ones listed?*

|`unsigned char`|Single Character without sign bit | `unsigned char i = 1` |0 to 255| 1 byte|
|---|---|---|---|---|
|`signed char`|Single Character with sign bit | `signed char i = -1` |-128 to 127| 1 byte|
|`unsigned int`|Integer without sign bit|`unsigned int i = 65000`|0 to 65,535 or 0 to 4,294,967,295| 2-4 bytes|
|`short`|Integer with consistent memory|`short i = 32000`| -32,768 to 32,767 | 2 bytes |
|`unsigned short`|Integer with consistent memory|`unsigned short i = 65000`| 0 to 65,535 | 2 bytes |
|`unsigned long`| Long without sign bit| `unsigned long i = 18446744073709551615`| 0 to 18446744073709551615| 8 bytes
|`long double`| Double with even more precision|`long double i = 1.1E-4000`|3.4E-4932 to 1.1E+4932 (19 decimal places)| 10 bytes

*What other conditional operators are there other than > and < ? * 
- == , != , >= , <=

*What other mathematical operators are there other than - and + ?*
- * , / , % , ++ , --

*What is the -= operator, and how does it work?*
- Subtract and assgnment operator. It subtracts the right operand from the left operand and assigns the result to the left operand

*What is the `do` loop, and how does it work?*
- `do` is a loop that has the conditional expression at the end of the loop, causing the internal code to execute at least once before the condition is tested.

*What is the `switch` statement and how does it work?*
- `switch` is a conditional check that allows several options to be branched to via an integral or enumerated type. 

*What is the `break` statement and how does it work?*
- `break` is the statement that causes a `switch` statement to terminate, otherwise it will continue through to subsequent cases.

*What is the `continue` statement and how does it work?*
- `continue` is similar to `break`, but instead of causing termination, it skips the iteration and goes directly to the conditional test.

*What does the `typedef` keyword do exactly?*
- Can give a type a new name
- Can create a user defined type using built in types
- Limited to giving symbolic names to types, where \#define can define alias for values as well
- Interpretation is performed by the compiler, \#define is processed by the pre-processor
### Resources
https://buildyourownlisp.com/chapter3_basics
https://www.tutorialspoint.com/cprogramming/c_data_types.htm
https://www.tutorialspoint.com/cprogramming/c_operators.htm