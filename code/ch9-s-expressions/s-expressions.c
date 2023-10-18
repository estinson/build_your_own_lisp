#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../mpc-files/mpc.h"

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

char* readline(char* prompt){
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);   
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)}='/0';
    return cpy;
}

void add_history(char* unused){}

#else 
#include <editline/readline.h>
#include <editline/history.h>
#endif

enum {LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM};
enum {LVAL_NUM, LVAL_ERR, LVAL_SYM, LVAL_SEXPR};

typedef struct lval{
    int type;
    long num;
    char* err;
    char* sym;
    int count;
    struct lval** cell;
} lval;

lval* lval_num(double x) {
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

lval* lval_sym(char* s) {
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

void lval_print(lval v) {
    switch (v.type) {
        case LVAL_NUM: printf("%f", v.num); break;

        case LVAL_ERR:
            if (v.err == LERR_DIV_ZERO) { printf("Error: Division by zero!");}
            if (v.err == LERR_BAD_OP) { printf("Error: Invalid Operator!");}
            if (v.err == LERR_BAD_NUM) { printf("Error: Invalid Number!");}
            break;
    }
}

void lval_println(lval v) { lval_print(v); putchar('\n'); }

lval eval_op(lval x, char* op, lval y) {
    if (x.type == LVAL_ERR) { return x; }
    if (y.type == LVAL_ERR) { return y; }

    if (strcmp(op,"+") == 0){ return lval_num(x.num + y.num); }
    if (strcmp(op,"-") == 0){ return lval_num(x.num - y.num); }
    if (strcmp(op,"*") == 0){ return lval_num(x.num * y.num); }
    if (strcmp(op,"/") == 0){ return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num); }
    if (strcmp(op,"%") == 0){ double i = x.num; while (i >= y.num) {i -= y.num;}; return lval_num(i); }
    if (strcmp(op,"^") == 0){ return lval_num(pow(x.num,y.num)); }
    if (strcmp(op,"min") == 0){ if (x.num < y.num ) { return lval_num(x.num);} return lval_num(y.num); }
    if (strcmp(op,"max") == 0){ if (x.num  > y.num ) { return lval_num(x.num);}  return lval_num(y.num); }
    return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t* t) {
    
    if (strstr(t->tag, "number")) {
        errno = 0;
        double x = strtod(t->contents, NULL);
        return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
    }

    char* op = t->children[1]->contents;
    lval x = eval( t->children[2] );

    int i = 3;
    if ((strstr(t->children[3]->tag, "number") == 0) && ( strcmp(op,"-") == 0 )) { return lval_num(x.num  * -1); }
    while (strstr(t->children[i]->tag, "expr")) {
        x = eval_op(x, op, eval(t->children[i]));
        i++;
    }

    return x;
}

int main(int argc, char** argv)
{
    /* Create Some Parsers */
    mpc_parser_t* Number    = mpc_new("number");
    mpc_parser_t* Symbol    = mpc_new("symbol");
    mpc_parser_t* Sexpr     = mpc_new("sexpr");
    mpc_parser_t* Expr      = mpc_new("expr");
    mpc_parser_t* Lispy     = mpc_new("lispy");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
      "                                                               \
        number   : /-?[0-9]+(\\.?[0-9]*)?/ ;                          \
        symbol   : '+' | '-' | '*' | '/' | '%' | \"min\" | \"max\" ;  \
        expr     : <number> | '(' <operator> <expr>+ ')' ;            \
        lispy    : /^/ <operator> <expr>+ /$/ ;                       \
      ",
    Number, Operator, Expr, Lispy);

    puts("Lispy Version 0.0.0.0.4");
    puts("Press Ctrl+C to exit\n");

    while (1) {
        char* input = readline("lispy> ");
        add_history(input);
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            lval result = eval(r.output);
            lval_println(result);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
    }

    mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Lispy);
    return 0;
}