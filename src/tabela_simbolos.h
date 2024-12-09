#ifndef _TABELA_SIMBOLOS_
#define _TABELA_SIMBOLOS_
#include <stdbool.h>
#include <stdio.h>

typedef enum{
    GLOBAL, LOCAL
}ESCOPO;



typedef enum{
    INT_, REAL_, CHAR_, BOOL_, NA_TIPO
}TIPO;
typedef enum{
    VAR_GLOBAL, VAR_LOCAL, PROC, PROC_PAR
}IDCATEGORIA;

typedef enum{
    COPIA, REFERENCIA, NA_PASSAGEM
}PASSAGEM;



typedef enum{
    VIVO, ZUMBI_, NA_ZUMBI
}ZUMBI;



typedef enum{
    SIMPLES, VETOR, MATRIZ, NA_ARRAY
}ARRAY;



typedef enum{
    CONST_, NORMAL
}EHCONST;



typedef struct valconst{
    union 
    {
        int int_const;
        float float_const;
        char char_const;
        bool bool_const;
    };
    
}ValConst;

typedef struct tokenInfo{
    char lexema[31];
    ESCOPO escopo;
    TIPO tipo;
    IDCATEGORIA idcategoria;
    PASSAGEM passagem;
    ZUMBI zumbi;
    ARRAY array;
    int arrayDim[2];
    EHCONST ehConst;
    ValConst valConst;
    int endereco;
}TokenInfo;
//Talves seja interessante criar um campo para descobrir quantos parametros proc tem

typedef struct tabela{
    int topo;
    TokenInfo tokensTab[1024];
}Tabela;

extern int topo;
extern Tabela tabela;

void printarTabela();
void inserirNaTabela(TokenInfo );
void removerDaTabela();
void limparTabela();
void verificaFaltaParam(int);
void resetTokenInfo(TokenInfo *);
void buscaDeclRep(TokenInfo );
int buscaLexPos(char *);
void inserirVazios(int, TokenInfo);
void matarZumbis(int);
void retirarLocais();
TokenInfo buscaDecl(TokenInfo);
#endif