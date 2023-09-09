---
tags: notes, investigation, study
---
### Basic Notes
- Abstract Syntax Tree - represents the structure  af the program based on the input entered
```
lispy> * 10 (+ 1 51)
>
  regex
  operator|char:1:1 '*'
  expr|number|regex:1:3 '10'
  expr|>
    char:1:6 '('
    operator|char:1:7 '+'
    expr|number|regex:1:9 '1'
    expr|number|regex:1:11 '51'
    char:1:13 ')'
  regex 
```
- Looking at the internal structure, this reflects the internal state
```c
typedef struct mpc_ast_t {
  char* tag;
  char* contents;
  mpc_state_t state;
  int children_num;
  struct mpc_ast_t** children;
} mpc_ast_t;
```
- Fields
	- `tag` - information that precedes the contents of the node, contained in a string of all rules used to parse the input. this allows us to see what rules have been used to parse the node
	- `contents` - contains the actual content of the node
	- `state` - contains the state the parser was in when it found the node. unused in this program.
	- `children_num` - contains the number of child nodes
	- `child` - an array of child nodes
		- accessible using array notation `[0]`
		- `mpc_ast_t**` - double pointer type: explained in later chapters. 
			- `mpc_ast_t*` - pointer to a struct: accessed via arrow `->` 
- Internal structure utilizes recursion via referencing its own data type within itself
- Recursion - a structure or functior that calls itself as part of its calculation
	- Building a recursive structure requires two parts: a base case, which marks the end of the recursive run, and a recursive case, which marks that the recursion can continue
```c
int number_of_nodes(mpc_ast_t* t) {
  # base case
  if (t->children_num == 0) { return 1; } 
  # if the node has no children, return 1
  # recursive case
  if (t->children_num >= 1) {
  # if the node has children...
    int total = 1;
    for (int i = 0; i < t->children_num; i++) {
      total = total + number_of_nodes(t->children[i]);
    }
    # loop through the remaining nodes...
    return total;
    # return the final count 
  }
  return 0;
}
```
- Building the evaluation function
	- `number` - never contains children, can be evaluated immediately
	- `expr` - first child will always be `(`, meaning evaluation should start with the second child, and end before the `)`
		- this constitutes the recursive case: there will be more expressions within the parenthesis
	- String Functions
		- `atoi` - converts a `char*` to `int`
		- `strcmp` - takes two `char*` and returns `0` if equal
		- `strstr` - takes two `char*` and returns a pointer to the location of the second in the first, or `0` if not contained 
- printing will be changed from the abstract tree to an evaluated results
```C
long eval(mpc_ast_t* t) {
  /* If tagged as number return it directly. */
  if (strstr(t->tag, "number")) {
    return atoi(t->contents);
  }
  /* The operator is always second child. */
  char* op = t->children[1]->contents;
  /* We store the third child in `x` */
  long x = eval(t->children[2]);
  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}
```

```C
/* Use operator string to see which operation to perform */
long eval_op(long x, char* op, long y) {
  if (strcmp(op, "+") == 0) { return x + y; }
  if (strcmp(op, "-") == 0) { return x - y; }
  if (strcmp(op, "*") == 0) { return x * y; }
  if (strcmp(op, "/") == 0) { return x / y; }
  return 0;
}
```
### Bonus
*Write a recursive function to compute the number of leaves of a tree.*
```c
int number_of_leaves(mpc_ast_t* t){
	if (t->children_num == 0) { return 1; }
	if (t->children_num >= 1) {
		int total = 0;
		for (int i = 0; i < t->children_num; i++) {
			total = total + number_of_nodes(t->children[i]);
		}
		return total;
	}
	return 0;
}
```
*Write a recursive function to compute the number of branches of a tree.*
```c
int number_of_branches(mpc_ast_t* t){
	if (t->children_num == 0) { return 0; }
	if (t->children_num >= 1) {
		int total = 0;
		for (int i = 0; i < t->children_num; i++) {
			total = total + number_of_nodes(t->children[i]);
			total += 1;
		}
		return total;
	}
	return 0;
}
```
*Write a recursive function to compute the most number of children spanning from one branch of a tree.*
```c
int number_of_children_max(mpc_ast_t* t){
	if (t->children_num == 0) { return 0; }
	if (t->children_num >= 1) {
		int total = 0;
		for (int i = 0; i < t->children_num; i++) {
			if (t->children_num > total) {
				total = t->children_num;
			}
		}
		return total;
	}
	return 0;
}
```
*How would you use `strstr` to see if a node was tagged as an `expr`?*
```c
strstr(t->tag, "expr")
```
*How would you use `strcmp` to see if a node had the contents `(` or `)`?*
```c 
strcmp(t->contents, "(") or strcmp(t->contents, ")")
```
### Analysis
A full understanding of the struct for mpca allows an understanding of the functions well. Including recursion is a good way to show the extendability of code 
### Resources
https://buildyourownlisp.com/chapter7_evaluation