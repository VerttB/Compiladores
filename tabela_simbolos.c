#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"
#include "auxfuncs.h"

Tabela tabela;
int topo = 0;

char *T_escopo[] = {
    [GLOBAL] = "Global",
    [LOCAL] = "Local"
};

char *T_tipo[] = {
    [INT_] = "Int",
    [REAL_] = "Real",
    [CHAR_] = "Char",
    [BOOL_] = "Bool",
    [NA_TIPO] = "N/A"
};

char *T_IdCategoria[] = {
    [VAR_GLOBAL] = "Var_G",
    [VAR_LOCAL] = "Var_L",
    [PROC] = "Proc",
    [PROC_PAR] = "Param",
};

char *T_passagem[] = {
    [COPIA] = "Copia",
    [REFERENCIA] = "Ref",
    [NA_PASSAGEM] = "N/A"
};

char *T_zumbi[] = {
    [VIVO] = "Vivo",
    [ZUMBI_] = "Zumbi",
    [NA_ZUMBI] = "N/A",
};

char *T_array[] = {
    [SIMPLES] = "Simples",
    [VETOR] = "Vetor",
    [MATRIZ] = "Matriz",
    [NA_ARRAY] = "N/A"
};  

char *T_ehConst[] = {
    [CONST_] = "Const",
    [NORMAL] = "Normal"
};



void inserirNaTabela(int tipo, char *lexema, ESCOPO escopo,PASSAGEM passagem, ZUMBI zumbi,ARRAY array ,int dimUm, int dimDois, EHCONST ehconst, ValConst valconst , int endereco){
    // buscaDeclRep(lexema); // Verifica Repetição de lexema

    TabelaS t;
    strcpy(t.lexema, lexema);
    t.tipo = tipo;
    t.escopo = escopo;
    t.passagem = passagem;
    t.zumbi = zumbi;
    t.array = array;
    t.DimUm = dimUm;
    t.DimDois = dimDois;
    t.ehConst = ehconst;
    t.valConst = valconst;
    t.endereco = endereco;
    tabela.tabelaS[topo] = t;
    topo++;
}

void buscaDeclRep(char *lexema){
    for(int i = 0;i<topo;i++){
        if(strcmp(lexema, tabela.tabelaS[i].lexema) == 0) error("Declaração repetida");
    }
}
void printarTabela(){
    strcpy(tabela.tabelaS[0].lexema, "Eaidaduawhdiuawhdiawhdaw");
    tabela.tabelaS[0].tipo = CHAR_;
    tabela.tabelaS[0].escopo = GLOBAL;
    TabelaS aux;
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("|\t\tLexema\t\t|tipo|escopo|passagem|zumbi|  array  |dimUm|dimDois|ehConst|valConst|endereço|\n");
    for(int i = topo; i >= 0;i--){
        aux = tabela.tabelaS[i];
        printf("|%-31s",aux.lexema);
        printf("|%-4s", T_tipo[aux.tipo]);
        printf("|%-6s", T_escopo[aux.escopo]);
        printf("|%-8s", T_passagem[aux.passagem]);
        printf("|%-5s", T_zumbi[aux.zumbi]);
        printf("|%-9s", T_array[aux.zumbi]);
        printf("|%-5d", aux.DimUm);
        printf("|%-7d", aux.DimDois);
        printf("|%-7s", T_ehConst[aux.ehConst]);
        if(aux.ehConst == CONST_){
            if(aux.tipo == INT_) printf("|%-8d", aux.valConst.int_const);
            else if(aux.tipo == CHAR_)printf("|%-8c", aux.valConst.char_const);
            else printf("|%-8s", aux.valConst);
        }
        printf("|%-8d", aux.endereco);


    }
    printf("\n");
 }