#ifndef _ANALEX_
#define _ANALEX_
#include <stdio.h>
#include <stdbool.h>
typedef enum {
    ID, CT_I, CT_R,CT_C, LT, FIM_ARQ, SN, PV_R, FIM_EXPR
}CATEGORIA;

typedef enum {
    ADICAO,MULT, SUB,DIV,MAIORQUE,MAIORIGUAL,MENORQUE,MENORIGUAL,ATRIBUICAO,IGUALDADE,
    PARENTESEABERTO,PARENTESEFECHADO,COLCHETEABERTO,COLCHETEFECHADO,OR,ENDERECO,AND,
    NEGACAO,DIFERENTE,VIRGULA,
} SINAL;

typedef enum{
    CONST, PR, INIT, ENDP, CHAR, INT, REAL, BOOL, DO, WHILE,ENDW, VAR, FROM, TO,BY,ENDV,IF,ELIF,ELSE,ENDI
    ,GETINT,GETCHAR,GETREAL,PUTINT,PUTCHAR,PUTREAL,DT, GETOUT, PUTSTR, GETSTR, PROT, DEF
}PVR;

extern char keywords[32][20];

typedef struct {
    CATEGORIA cat;
    bool processado;
    union{
        int codigo;
        int valor;
        float valor_r;
        char c;
        char lexema[31];
    };
}TOKEN;

extern int linha;    
extern TOKEN tk;        
extern FILE *f;  

TOKEN analex(FILE *f);
void testeAnalex();
void error(char []);

#endif