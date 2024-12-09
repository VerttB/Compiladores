#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"
#include "auxfuncs.h"
#include "cores.h"

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
    buscaDeclRep(token); // Verifica Repetição de lexema
    token.endereco = tabela.topo;
    tabela.tokensTab[tabela.topo] = token;
    tabela.topo++;
    printarTabela();
    
}

void buscaDeclRep(TokenInfo token){
    for(int i = 0;i<tabela.topo;i++){
        if(strcmp(token.lexema, tabela.tokensTab[i].lexema) == 0){
            if(tabela.tokensTab[i].idcategoria == PROC && token.idcategoria == PROC) error("Redeclaração de procedimento encontrada");
            if(tabela.tokensTab[i].idcategoria == VAR_LOCAL && token.idcategoria == VAR_LOCAL) error("Redaclaração de variável encontrada");
            if(tabela.tokensTab[i].idcategoria == VAR_GLOBAL && token.idcategoria == VAR_GLOBAL) error("Redeclaração de variável global");
        }
    }
}

int buscaLexPos(char *lexema){
     for(int i = 0;i<tabela.topo;i++){
        if(strcmp(lexema, tabela.tokensTab[i].lexema) == 0) return i;
    }
    return -1;
}
void printarTabela(){
    TokenInfo aux;
    printf("┌───────────────────────────────┬────┬──────┬────────┬─────┬─────────┬─────┬───────┬────────────────┬────────┬─────────┬\n");
    printf("│\t\tLexema\t\t│tipo│escopo│passagem│zumbi│  array  │dimUm│dimDois│ehConst│valConst│endereço│categoria│\n");
    printf("├───────────────────────────────┼────┼──────┼────────┼─────┼─────────┼─────┼───────┼───────┼────────┼────────┼─────────┤\n");

    for(int i = 0 ; i < tabela.topo;i++){
        printf("│");
        if(i == tabela.topo - 1) printf("%s", _GREEN_);
        aux = tabela.tokensTab[i];
        printf("%-31s",aux.lexema);
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
            else printf("│%-8s", aux.valConst.bool_const == 1 ? "True" : "False");
        }
        else{
            printf("│%-8s", "N/A");
        }
        printf("│%-8d", aux.endereco);
        printf("│%-9s", T_IdCategoria[aux.idcategoria]);
        printf("%s│",_NORMAL_);
    printf("\n├───────────────────────────────┼────┼──────┼────────┼─────┼─────────┼─────┼───────┼───────┼────────┼────────┼─────────┤\n");

    }
   
    printf("└───────────────────────────────┴────┴──────┴────────┴─────┴─────────┴─────┴───────┴───────┴────────┴────────┴─────────┘\n");
    printf("Pressione Enter para continuar...\n");
    getchar();
 }

 void removerDaTabela(){
      if (tabela.topo > 0) {
        tabela.topo--;
    } else {
        printf("Tabela já está vazia.\n");
    }
}
 void limparTabela() {
    memset(tabela.tokensTab, 0, sizeof(tabela.tokensTab)); // Preenche com zeros
    tabela.topo = 0; // Reseta o topo
}

void resetTokenInfo(TokenInfo *token) {
    memset(token, 0, sizeof(TokenInfo));
    memset(token->lexema, 0, sizeof(token->lexema));
}

void inserirVazios(int procPos, TokenInfo tokenInfo){
    TokenInfo aux = tabela.tokensTab[procPos];
    if(aux.idcategoria != tokenInfo.idcategoria) error("Quantidade de parâmetros inválida");
    if(strcmp(aux.lexema, "") != 0) error("Quantidade de argumentos inválida");
    if(aux.tipo != tokenInfo.tipo) error("Tipo dos argumentos inválido. Esperado: %s, recebido: %s", T_tipo[aux.tipo], T_tipo[tokenInfo.tipo]);
    if(aux.array != tokenInfo.array) error("Tipo de argumento incompatível. Esperado: %s, recebido: %s", T_array[aux.array], T_array[tokenInfo.array]);
    if(aux.passagem != tokenInfo.passagem) error("Método de passagem de argumento incompatível. Esperado: %s, recebido: %s",T_passagem[aux.passagem], T_passagem[tokenInfo.passagem]);
    strcpy(tabela.tokensTab[procPos].lexema, tokenInfo.lexema);
    tabela.tokensTab[procPos].zumbi = VIVO;
    printarTabela();
}

void verificaFaltaParam(int procPos){
    procPos++;
    while(1){
        if(tabela.tokensTab[procPos].idcategoria != PROC_PAR) break;
        if(strcmp(tabela.tokensTab[procPos].lexema, "") == 0) error("Quantidade de argumentos inválida");
        procPos++;
    }
}

void matarZumbis(int procPos){
    procPos++;
    while(1){
        if(tabela.tokensTab[procPos].idcategoria != PROC_PAR) break;
        tabela.tokensTab[procPos].zumbi = ZUMBI_;
        procPos++;
        printarTabela();
    }
}

void retirarLocais(){
    while(1){
        if(tabela.tokensTab[tabela.topo-1].idcategoria != VAR_LOCAL) break;
        removerDaTabela();
        printarTabela();
    }
}