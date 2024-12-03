#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"
#include "auxfuncs.h"

Tabela tabela;

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



void inserirNaTabela(TokenInfo token){
   // buscaDeclRep(token.lexema); // Verifica Repetição de lexema
    token.endereco = tabela.topo;
    tabela.tokensTab[tabela.topo] = token;
    tabela.topo++;
}

void buscaDeclRep(char *lexema){
    for(int i = 0;i<tabela.topo;i++){
        if(strcmp(lexema, tabela.tokensTab[i].lexema) == 0) error("Declaração repetida");
    }
}
void printarTabela(){
    TokenInfo aux;
    printf(" tamanho = %d\n", tabela.topo);
    printf("┌───────────────────────────────┬────┬──────┬────────┬─────┬─────────┬─────┬───────┬────────────────┬────────┬\n");
    printf("│\t\tLexema\t\t│tipo│escopo│passagem│zumbi│  array  │dimUm│dimDois│ehConst│valConst│endereço│\n");
    for(int i = 0 ; i < tabela.topo;i++){
        aux = tabela.tokensTab[i];
        printf("│%-31s",aux.lexema);
        printf("│%-4s", T_tipo[aux.tipo]);
        printf("│%-6s", T_escopo[aux.escopo]);
        printf("│%-8s", T_passagem[aux.passagem]);
        printf("│%-5s", T_zumbi[aux.zumbi]);
        printf("│%-9s", T_array[aux.array]);
        printf("│%-5d", aux.DimUm);
        printf("│%-7d", aux.DimDois);
        printf("│%-7s", T_ehConst[aux.ehConst]);
        if(aux.ehConst == CONST_){
            if(aux.tipo == INT_) printf("│%-8d", aux.valConst.int_const);
            else if(aux.tipo == CHAR_)printf("│%-8c", aux.valConst.char_const);
            else if(aux.tipo == REAL_) printf("│%-8f", aux.valConst.float_const);   
            else printf("│%-8s", aux.valConst.bool_const);
        }
        else{
            printf("│%-8s", "N/A");
        }
        printf("│%-8d│", aux.endereco);
        printf("\n");

    }
    printf("└───────────────────────────────┴────┴──────┴────────┴─────┴─────────┴─────┴───────┴───────┴────────┴────────┘\n");

 }

 void removerDaTabela(){
      if (tabela.topo > 0) {
        tabela.topo--;
    } else {
        printf("Tabela já está vazia.\n");
    }
}
 void limparTabela() {
    printf("Limpando tabela");
    memset(tabela.tokensTab, 0, sizeof(tabela.tokensTab)); // Preenche com zeros
    tabela.topo = 0; // Reseta o topo
}