typedef enum {
    ID, CT_I, CT_R,CT_C, LT, FIM_ARQ, SN, PR
}CATEGORIA;

typedef enum {
    ADICAO,MULT, SUB,DIV,MAIORQUE,MAIORIGUAl,MENORQUE,MENORIGUAl,ATRIBUICAO,IGUALDADE,
    PARENTESEABERTO,PARENTESEFECHADO,COLCHETEABERTO,COLCHETEFECHADO,OR,ENDERECO,AND,
    NEGACAO,DIFERENTE,VIR,PONTO,
} SINAL;

typedef enum{
    CONST, PVR, INIT, ENDP, CHAR, INT, REAL, BOOL, DO, WHILE,ENDW, VAR, FROM, TO,BY,ENDV,IF,ELIF,ELSE,ENDI,GETINT,GETCHAR,GETREAL,PUTINT,PUTCHAR,PUTREAL
}PV_R;

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

bool isDigit(char);
bool isALetter(char);
bool isKeyWord(char *);
bool isIdentifier(char);
bool isAComment(char);
bool isAFloat(char);
TOKEN analex(FILE *f);