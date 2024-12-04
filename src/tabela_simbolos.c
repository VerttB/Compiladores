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
    resetTokenInfo(&token);
    printarTabela();
    
}

void buscaDeclRep(TokenInfo token){
    for(int i = 0;i<tabela.topo;i++){
        if(strcmp(token.lexema, tabela.tokensTab[i].lexema) == 0){
            if(token.escopo == GLOBAL && tabela.tokensTab[i].escopo == GLOBAL){
                if(token.idcategoria == tabela.tokensTab[i].idcategoria) error("Multiplas declarações do mesmo tipo");
            }
            else{
                error("Declarações com mesmo nome encontrada");
            }
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
    printf(" tamanho = %d\n", tabela.topo);
    printf("┌───────────────────────────────┬────┬──────┬────────┬─────┬─────────┬─────┬───────┬────────────────┬────────┬─────────┬\n");
    printf("│\t\tLexema\t\t│tipo│escopo│passagem│zumbi│  array  │dimUm│dimDois│ehConst│valConst│endereço│categoria│\n");
    printf("├───────────────────────────────┼────┼──────┼────────┼─────┼─────────┼─────┼───────┼───────┼────────┼────────┼─────────┤\n");

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
            else printf("│%-8s", aux.valConst.bool_const == 1 ? "True" : "False");
        }
        else{
            printf("│%-8s", "N/A");
        }
        printf("│%-8d", aux.endereco);
        printf("│%-9s│", T_IdCategoria[aux.idcategoria]);
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
    printf("Limpando tabela");
    memset(tabela.tokensTab, 0, sizeof(tabela.tokensTab)); // Preenche com zeros
    tabela.topo = 0; // Reseta o topo
}

void resetTokenInfo(TokenInfo *token) {
    memset(token, 0, sizeof(TokenInfo));
    memset(token->lexema, 0, sizeof(token->lexema));
}

void inserirVazios(int procPos, TokenInfo tokenInfo){
    TokenInfo aux = tabela.tokensTab[procPos];
    printf("LEXEMA DO TOKEN %s POS A INSERIR %d", tokenInfo.lexema, procPos);
    if(strcmp(aux.lexema, "") != 0) error("Quantidade de argumentos inválida");
    if(aux.tipo != tokenInfo.tipo) error("Tipo dos argumentos inválido");
    if(aux.array != tokenInfo.array) error("Tipo de variavel|vetor|matriz inválido");
    if(aux.passagem != tokenInfo.passagem) error("Passagem de argumento inválido");
    strcpy(tabela.tokensTab[procPos].lexema, tokenInfo.lexema);
    printarTabela();

}
