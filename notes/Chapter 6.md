---
tags: notes, investigation, study, c, c-projects, buildyourownlisp
---
### Basic Notes
- Work out a grammar which describes Polish Notation, explained below:

|Program| The start of input, an `Operator`, one or more `Expression`, and the end of input|
|---|---|
|Expression| eithor a `Number` or `(`, an `Operator`, one or more `Expression`, and an `)`|
|Operator|`+`,`-`,`*`, or `/`|
|Number|an optional `-`, and one or more characters between `0` and `9` |
*The grammar of polish notation*

- Creation of the above rules using learned knowledge, aside from Number and Program
	- These con be expressed in Regular Expressions
- `mpc` grammar allows regex by putting them between /
	- Number rule can be expressed with `/-?[0-9]+/`
- The new program will need to include `mpc` headers and linking to the compiler
- `mpc` inclusion
	- The grammar is included before the interactive prompt, right after `main`
	- A cleanup function is called right before the program closes
	- Previous `printf` is replaced by `mpc` code
- Output from the program returns the processed results of the input given
### Bonus
*Write a regular expression matching strings of all `a` or `b`.*
- `/[ab]+/`
*Write a regular expression matching strings of consecutive `a` or `b`.*
- `/(ab?)+/`
*Write a regular expression matching pit, pot, respite but not peat, spit, or part*
- `/(res)?p[io]?t.?/` 
*Change the grammar to add a new operator such as %.*
```c
"                                                     \
    number   : /-?[0-9]+/ ;                             \
    operator : '+' | '-' | '*' | '/' | '%';                  \
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    lispy    : /^/ <operator> <expr>+ /$/ ;             \
  ",
``` 
*Change the grammar to recognize operators written in textual format `add`, `sub`, `mul`, `div`*
```c
"                                                     \
    number   : /-?[0-9]+/ ;                             \
    operator : '+' | 'add' | '-' | 'sub' | '*' | 'mul' | '/' | 'div' ; \
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    lispy    : /^/ <operator> <expr>+ /$/ ;             \
  ",
```
*Change the grammar to recognize decimal numbers such as `0.01`, `5.21`, or `10.2`*
```c
"                                                    \
    number   : /-?[0-9]+\.?\d?+/ ;                             \
    operator : '+' | '-' | '*' | '/' ;                  \
    expr     : <number> | '(' <operator> <expr>+ ')' ;  \
    lispy    : /^/ <operator> <expr>+ /$/ ;             \
  ",
```
*Change the grammar to make the operators written conventionally, between two expressions.*
```c
mpc_parser_t* Operator = mpc_or(4,
	mpc_sym("+"),mpc_sym("-"),
	mpc_sym("*"),mpc_sym("/")
)
```

*Use the grammar from the previous chapter to parse `Doge`. You must add start and end of input.*
```c
mpc_parser_t* Adjective = mpc_new("adjective");
mpc_parser_t* Noun      = mpc_new("noun");
mpc_parser_t* Phrase    = mpc_new("phrase");
mpc_parser_t* Doge      = mpc_new("doge");

mpca_lang(MPCA_LANG_DEFAULT,
  "                                           \
    adjective : \"wow\" | \"many\"            \
              |  \"so\" | \"such\";           \
    noun      : \"lisp\" | \"language\"       \
              | \"book\" | \"build\" | \"c\"; \
    phrase    : <adjective> <noun>;           \
    doge      : <phrase>*;                    \
  ",
  Adjective, Noun, Phrase, Doge);

    while (1) {
        char* input = readline("lispy> ");
        add_history(input);
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Doge, &r)) {
		  /* On Success Print the AST */
		  mpc_ast_print(r.output);
		  mpc_ast_delete(r.output);
		} else {
		  /* Otherwise Print the Error */
		  mpc_err_print(r.error);
		  mpc_err_delete(r.error);
		}
	}

mpc_cleanup(4, Adjective, Noun, Phrase, Doge);
```
### Analysis
This chapter is based entirely around the mpca library, which has a pretty simple interface for creating parsing formats. 
### Resources
https://buildyourownlisp.com/chapter6_parsing