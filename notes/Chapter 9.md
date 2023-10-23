---
tags: notes, investigation, study
---
### Basic Notes
- Lisps have little distinction between data and code, as they use the same structure to represent both
- To utilize this ability, we will have to separate the process of reading and evaluating
- The focus af this chapter will be refactoring to allow ease of adding modularity to the rest of the project
- To store the program this will need an internal symbolic expressions
	- Symbolic Expression: an internal list structure recursively made up of numbers, symbols, and other lists
- Pointers
	- They seem horribly daunting, as they are conceptually simple but have no obvious use case: thankfully we have a couple of ideal use cases that are extremely common in C
	- Functions in C are passed by *value*, meaning that a *copy* of the value to the function.
		- A common problem with this is with large structs containing sub structs: each time the function is called, there is another copy made, which makes a large amount of data quickly
		- As well, a struct has a fixed size, meaning that calling a function on a list of various structs would not be able to function properly
	- Pointers are bytes of data that are given a global index, similar to a house address. 
		- Using this allows us to pass through the starting point of the data, which would allow the function to work on any amount of the actual data without copying or restricting
	- Since the total size of computer memory is fixed, the number of bytes an address has is always the same. That means that a variable sized data structure can be passed to a function
	- The type of the pointer shows what data is available at the address: pointer addresses are found with &, and pointer types are created with \*
	- To get the data at an address (dereferencing), use \* on a pointer address, to get the data at the field of a pointer to a struct, use -> 
- Stack and Heap
	- Stack
		- The memory of a program: Stores temporary variables, data structures, and bookkeeping data such as who the caller was, and what to do when finished. Every time a function is called, a new area of the stack is used. 
		- When the function completes, the memory is unallocated
	- Heap
		- A section of memory put aside for objects with a longer lifespan
		- [[malloc]]: Takes the bytes required, returns back a pointer to the new block of memory. [[free]]: Used to unallocate the memory at a given pointer
		- Trickier to use, as the allocations must be manually freed, or lead to a memory leak: an easy rule to avoid this is to ensure that each [[malloc]] must have a corresponding [[free]] 
- Parsing Expressions
	- Refactoring the existing code will need a change of the operator rule to symbol, and the addition of the S-Expression
- Expression Structure
	- S-Expressions are variable length lists of other values, meaning that they will need pointers to be used as structs can't be variable lengths
	- `cell` will point to a list of `lval` pointers, which means it will need to be a double pointer
	- Symbols and errors will be represented by a string, meaning errors can now store a unique error message rather than just an error code
	- `lval` will need to include itself in the original definiton, and therefore needs to have the definition before the curly braces, otherwise the size of the struct would refer to itself
	- A struct can refer to its own type, but it must only contain pointers to its own type, not its own type directly: if this is done, the size of the struct wil refer to itself, and become infinite
- Constructors & Destructors
	- `lval` construction functions should return pointers, which will make keeping track of lval variables easier. (why)
		- `lval` fields may contain pointers to other things that have been allocated on the heap, meaning that when an `lval` is freed, all the things it points to must also be freed
	- [[NULL]] is used within this script.
	- C strings are [[NULL]] terminated, meaning the final character is always the zero character (`\0`)
		- A convention in C to signal the end of a string. Strings need to be stored as `strlen(s) + 1` to accomodate this
	- `lval_del` - a special function to delete `lval*`: it frees the pointer itself, but then recursively looks into the pointer to see if it has more memory pointers
		- This needs to be called for each of the construction functions to ensure there are no memory leaks
- Reading Expressions
	- The order of operations goes: 
		- Read in the program and construct an `lval*`
		- Evaluate the `lval*` to get the result of the program
	- Read will need to convert the abstract syntax tree into an S-Expression, and Evaluate will evaluate the S-Expressin with the normal Lisp rules
	- Constructors will return a different result depending on the node type:
		- `number` + `symbol`: will return an `lval*` directly for those types
		- `root` + `sexpr`: will create an empty `lval` and slowly add each valid sub-expression
	- `lval_add`: increases the count of the expression list by one, then reallocates memory via the count, and appends the new `lval`. 
	- Typical implementations of Lisps S-Expressions are defined as `atom` (for symbols or numbers) and `cons` (two other S-Expressions joined together)
		- This leads to using linked lists as opposed to a variable sized array, which is subtly different.
- Printing Expressions
	- `lval_print`: prints out an S-Expression by looping over all the sub-expressions and prints these the same way they are input.
- Evaluating Expressions
	- Now that `lval`'s data type has changed, the evaluation functions will need to be changed. There will need to be both an expression and s-expression analyzer, as well as an operation function and a function for moving values from the heap to the stack (`lval_pop`)
	- `lval_eval_sexpr`: 
		- evaluates the children of the s-expression
		- checks if any are errors
		- checks if any have empty expressions
		- returns the s-expression if it is empty
		- creates a variable by popping the lval from heap to memory
		- ensures the first element is a symbol, if not return an error
		- runs operation logic on lval and saves it
		- deletes the popped lval and returns the result
	- `lval_eval`: checks if `lval` is a s-expression and returns the result of s-expression evaluation, or returns the value
	- `lval_pop`: 
		- takes an `lval` from the `lval` array ([[heap]]) to a variable ([[stack]])
		- shortens the size of the `lval` array, and lowers the count in the array
		- returns the `lval`
	- `lval_take`: pops out a specific value from an `lval`, then deletes the `lval`, and returns the value
	- `builtin_op`:
		- ensures that all the arguments are numbers
		- `lval_pop`s the first element
		- if given "-" operator and no other arguments, negate the value
		- while there are elements
			- `lval_pop`s the next element
			- confirms which operator is being used and performs function into this element
				- if operator is division and second element is 0, return DBZ error
			 - delete the secondary element 
		 - delete the operator `lval`, return the operated on element
### Analysis
```c
typedef struct lval {
	int type;
	long num;
	char* err;
	char* sym;
	int count;
	struct lval** cell;
} lval;
```
`lval` now saves errors and symbols into the struct, as well as having a `count` and a new `cell`, which contains a double pointer. The double pointer is necessary to get the data out of a pointer to an `lval` (a pointer to a pointer).
*The new definition needs a reference to itself: without an alias (the name value after `struct`), this reference would cause an infinite memory allocation when referring to itself.*
```c
lval* lval_num(long x) {
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_NUM;	
	v->num = x;
	return v;
}

lval* lval_err(char* m) {
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_ERR;
	v->err = malloc(strlen(m) + 1);
	strcpy(v->err, m);
	return v;
}

lval* lval_sym(char* s)) {
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_SYM;
	v->sym = malloc(strlen(s) + 1);
	strcpy(v->sym, s);
	return v;
}

lval* lval_sexpr(void) {
	lval* v = malloc(sizeof(lval));
	v->type = LVAL_SEXPR;
	v->count = 0;
	v->cell = NULL;
	return v;
}
```
The above commands are for analyzing each type of `lval`: they each have a separate way of analyzing the data. 
The general structure is as follows:
- Set a pointer `lval` with the expected data type
- Allocate the memory at the size of the `lval`
- Set the type and the value of the data
- Return the pointer `lval`
```c
void lval_del(lval* v) {
	switch (v->type) {
		case LVAL_NUM: break;	
		case LVAL_ERR: free(v->err); break;
		case LVAL_SYM: free(v->sym); break;
		case LVAL_SEXPR: for (int i = 0; i < v->count; i++) {lval_del(v->cell[i]);} break;
	}
	free(v);
}
```
`lval_del` will analyze the `lval` and remove the string attached to string-dependent types, and recursively run on `sexpr` cells. It then frees the `lval`.
```c
lval* lval_read_num(mpc_ast_t* t) {
	errno = 0;
	long x = strtol(t->contents, NULL, 10);
	return errno != ERANGE ? lval_num(x) : lval_err("invalid number");
}
```
`lval_read_num` will analyze a lisp value that is passed in, and return with either an `lval_error` or a `lval_num`.
```c
lval* lval_add(lval* v, lval* x) {
	v->count++;
	v->cell = realloc(v->cell, sizeof(lval*) * v->count);
	v->cell[v->count-1] = x;
	return v;
}
```
`lval_read` will add an `lval*` to another `lval*`, and assign the memory appropriate for the count of `lval*`s in the lval, then assign the `lval` to the memory
```c
void lval_expr_print(lval* v, char open, char close) {
	putchar(open);
	for (int i = 0; i < v->count; i++) {
		lval_print(v->cell[i]);
		if (i != (v->count-1)) {putchar(' ');} 
	} 
	putchar(close);
}

void lval_print(lval* v) {
	switch (v->type) {
		case LVAL_NUM: printf("%li", v->num); break;
		case LVAL_ERR: printf("Error: %s", v->err); break;
		case LVAL_SYM: printf("%s", v->sym); break;
		case LVAL_SEXPR: lval_expr_print(v, '(', ')'); break;
	}
}
```
`lval_print` will analyze and print with regards to the type.
`lval_expr_print` will recursively read through the cells inside of an `lval_sexpr`, and run `lval_print` on their contents.
### Notes
- On compile, there are a load of `implicit declaration of function` warnings, and a `conflicting types for 'lval_eval'` error, but does not stop compile?
	- turns out that this is a function declaration issue: C99 will assume that any undeclared function has an `int` return type, which of course conflicts with the return. to get around this, i've added function prototypes for both the implicit declarations and `lval_eval`
		- wasn't necessary but warnings are scary
			- turns out it was actually in the chapter on a more thorough read: not for `lval_eval` tho
### Bonus
*Give an example in of a variable in our program that lives on the stack.*
Any value that is used in a function locally and returned, such as `x` in `lval_take`
*Give an example in of a variable in our program that lives on the heap.* 
Nearly all of the `lval` functions are allocated into memory, such as `lval_num`, `lval_err`
*What does the [[strcpy]] function do?*
Created note to answer: [[strcpy]]
*What does the [[realloc]] function do?*
Created note to answer: [[realloc]]
*What does the [[memmove]] function do?*
Created note to answer: [[memmove]]
*How does [[memmove]] differ from [[memcopy]]?*
Memmove has a buffer to ensure that if the source and destination overlap, there is no overlapping copy of data when it is written to pointers.
*Extend parsing and evaluation to support the remainder operator `%`.*
Done. Had some trouble, turns out it was just placed after the `lval_del`. dope
*Extend parsing and evaluation to support decimal types using a `double` field.*
Done. Had to change `%` to modulo formula with int typecast to ensure correct math. 
### Resources
[Conflicting types for function?](https://stackoverflow.com/questions/1549631/getting-conflicting-types-for-function-in-c-why) 
[Chapter 9](https://buildyourownlisp.com/chapter9_s_expressions) 
[Modulo formula](https://math.stackexchange.com/questions/674419/is-there-a-formula-for-modulo)  