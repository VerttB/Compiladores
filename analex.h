typedef enum {
    ID, CT_I, CT_R,CT_C, LT, FIM_ARQ, SN, PV_R, COMENTARIO
}CATEGORIA;

typedef enum {
    ADICAO,MULT, SUB,DIV,MAIORQUE,MAIORIGUAl,MENORQUE,MENORIGUAl,ATRIBUICAO,IGUALDADE,
    PARENTESEABERTO,PARENTESEFECHADO,COLCHETEABERTO,COLCHETEFECHADO,OR,ENDERECO,AND,
    NEGACAO,DIFERENTE,VIRGULA,
} SINAL;

typedef enum{
    CONST, PR, INIT, ENDP, CHAR, INT, REAL, BOOL, DO, WHILE,ENDW, VAR, FROM, TO,BY,ENDV,IF,ELIF,ELSE,ENDI,GETINT,GETCHAR,GETREAL,PUTINT,PUTCHAR,PUTREAL,DT
}PVR;


typedef struct {
    CATEGORIA cat;
    union{
        int codigo;
        int valor;
        float valor_r;
        char c;
        char lexema[31];
    };
}TOKEN;

int linha = 1;
TOKEN analex(FILE *f);