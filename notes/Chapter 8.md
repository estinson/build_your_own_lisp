---
tags: notes, investigation, study
---
### Basic Notes
- Program can currently report syntax errors, but no way to report evaluation errors. 
- C is an awkward language to get error handling in place
- Heisenbug - A bug that changes behaviour when observed under scrutiny
- `gdb` and `valgrind` will be utilized to add bugfixing to our toolkit
- Lisp Value
	- In this context, errors will be handled as a result of evaluation.
	- Expressions will either a number or an error
	- To capture this, we will need to create a struct that allows us to evaluate which fields con offer meaningful access
```c
typedef struct {
	int type;
	long num;
	int err;
} lval;
```
- The type of `type` and `err` are `int`: this is done to incorporate an Enum
- Enum
	- `enum { LVAL_NUM, LVAL_ERR };` - lisp value declaration; under the hood, these are automatically assigned integer constant values in order. (LVAL_NUM == 0, LVAL_NUM == 1)
	- `enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM }` - lisp error declaration; three error cases that can be caused in our particular program (dividing by zero, using an unassigned operator, and using a number value that is too large for `long`)
- Lval Functions
	- There are no current methods for creating new instances of the types - these must be declared 
```c  
lval lval_num(long x) { 
	lval v;
	v.type = LVAL_NUM;
	v.num = x; 
	return v;
}

lval lval_err(int x) { 
	lval v;
	v.type = LVAL_ERR;
	v.err = x; 
	return v;
}
```
- Changing the types that are used to run the program require a change to the logic of printing them
```c
void lval_print(lval v) {
	switch (v.type) {
		case LVAL_NUM: printf("%li", v.num); break;
		
		case LVAL_ERR:
			if (v.err == LERR_DIV_ZERO) { printf("Error: Division by Zero!"); }
			if (v.err == LERR_BAD_OP) { printf("Error: Invalid Operator!"); }
			if (v.err == LERR_BAD_NUM) { printf("Error: Invalid Number!"); }
			break;
	}
}

void lval_print(lval v) { lval_print(v); putchar('\n'); }
```
- Now that the printing functionality of the program is adjusted, the `eval_op` function will need to be changed from type `long`
```c 
lval eval_op(lval x, char* op, lval y) {
	if (x.type == LVAL_ERR) { return x; }
	if (y.type == LVAL_ERR) { return y; }
	
	if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
	if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
	if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
	if (strcmp(op, "/") == 0) { return y.num == 0 ? lval_err(ERR_DIV_ZERO) : lval_num(x.num / y.num); }
}
```
- `eval` will also need adjusting, as the return type will be `lval`
```c 
lval eval(mpc_ast_t* t) {
	if (strstr(t->tag, "number")) {
		errno = 0;
		long x = strtol(t->contents, NULL, 10);
		return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
	}
	char* op = t->children[1]->contents;
	lval x = eval(t->children[2]);
	int i = 3;
	while (strstr(t->children[i]->tag, "expr")) {
		x = eval_op(x, op, eval(t->children[i]));
		i++;
	}
	return x;
}
```
- The final change was to print the result with the new functions
```c
lval result = eval(r.output);
lval_println(result);
mpc_ast_delete(r.output);
```
### Bonus
*Run the previous chapter's code through `gdb` and crash it. See what happens.*
- `Program received signal SIGFPE, Arithmetic exception.` 
*How do you give an `enum` a name?*
- `enum name {};`
*What are `union` data types and how do they work?*
- Union data types allow several data types to be stored, and creates memory location with enough space to hold the largest data type contained within the statement
- Only one of the declared data types can be contained within the memory location
*What are the advantages over using a `union` instead of `struct`?*
- Unions technically take less space inside of a program, and only require one declaration for multiple results
*Can you use a `union` in the definition of `lval`?*
```c
typedef struct {
	int type;
	union {
		int err;
		long num;
	};
} lval;
```
*Extend parsing and evaluation to support the remainder operator `%`.*
- As long as the data type given isn't doubles, this is trivial, and just uses `%`
- *With* doubles, it requires a while loop to subtract the two from each other until there is no remainder
*Extend parsing and evaluation to support decimal types using a `double` field.*
- Change all longs to doubles, and change `strtol` to `strtod`, changing arguments to match `strtod` implementation
### Analysis
Jesus christ, have I ever got to pay more attention to how my data types are set up. I had the type set inside of the anonymous union, which caused errors to never print, and set to the num value instead. As well, it seemed that the bonus was the true ballache, with the modulo and double implementations being a sticking point. They were trivial in the end, but gd 

`gdb` was very helpful in determining where the actual error was as well, very handy. will have to analyze the docs a bit more to fully learn the tool.
### Resources
https://buildyourownlisp.com/chapter8_error_handling
https://sourceware.org/gdb/current/onlinedocs/gdb
https://web.eecs.umich.edu/~sugih/pointers/summary.html