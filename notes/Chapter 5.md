---
tags: notes, investigation, study, c, c-projects, buildyourownlisp
---
### Basic Notes
- Both programming and spoken languages have a structure, and rules. 
- Grammar
	- Code blocks can recursively contain code blocks
	- Code needs a formal description of grammar
	- To write a Lisp, we will need to understand grammars
- `mpc`
	- `mpc` is a *parser combinator*: a library that can build programs  that understand and process particular languages
	- Many parser Combinator libraries cork by parsing cobe that looks like a grammar, but not specify a grammar directly, which can get complicated.
	- `mpc` allows for both this method and to write grammar directly
	- `mpc_parser_t*` allows the creation of parsers, with functions that allow several options to be set 
```C 
mpc_parser_t* Adjective = mpc_or(4,
								mpc_sym("wow"),mpc_sym("many"),
								mpc_sym("so"),mpc_sym("such")
								);
mpc_parser_t* Noun = mpc_or(5,
							mpc_sym("lisp"),mpc_sym("language"),
							mpc_sym("book"),mpc_sym("build"),
							mpc_sym("c")
							);
```
*Define `Adjective` and `Noun`, with dictionaries that contain the options*

```c
mpc_parser_t Phrase = mpc_and(2, mpcf_strfold,
							 Adjective, Noun, free);
```
*Define `Phrase`, using the arguments previously defined, as well as join and delete fucntions*

```C
mpc_parser_t* Doge = mpc_many(mpc_strfold, Phrase);
```
*Define `Doge` (the language), specifying that zero or more of a parser is required, as well as a join function*

- A parser that looks for zero or more occurrences of another parser means that its language is infinite.
- Using more `mpc` functions, more and more complicated languages can be parsed 
- `mpc` also allows grammars to also be written with a multiline string, removing the need to include join and delete functions

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

/* Do some parsing here... */

mpc_cleanup(4, Adjective, Noun, Phrase, Doge);
```
*Defining `Doge` language with multiline string, requiring two functions: `mpca_lang` and `mpc_cleanup`*

|Special Symbol| Rule|
|---|---|
|`"ab"`|The string `ab` is required|
|`'a'`|The character `a` is required|
|`'a' 'b'`|First `a` is required, then `b` is required|
|`'a'|'b'`|Either `a` is required, or `b` is required|
|`'a'*`|Zero or more `a` are required|
|`'a'+`|One or more `a` are required|
|`<abba>`|The rule called `abba` is required|
### Bonus
*Why are there back slashes in front of the quote marks and at the end of the line in the grammar?*
- Escape character to allow string to continue without causing compiler issues
- Multiline strings use these to separate lines for readability  without causing compiler issues

*Describe textually a grammar for Decimal numbers, Web URLs, and Simple English sentences.*
- Decimals:
	- Have at either zero or number, then a decimal point, and then one or more numbers after the decimal point.
- URLs:
	- Have either `http` or `https`, then `://www.`, then a string of one or more characters, and then `.com`
- English: 
	- Have one or more adjectives, require a noun, and then require a verb.  
### Analysis
Not horribly interested in the topic of grammars, but this seems like vital information for the project, as the lisp needs same kind of parsing.
### Resources
https://buildyourownlisp.com/chapter5_languages