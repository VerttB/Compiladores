#include <stdio.h>
#include <stdlib.h>
#include "tabela_simbolos.h"

Tabela tabela;
int topo = 0;

void inserirNaTabela(char *lexema, ESCOPO escopo,PASSAGEM passagem, ZUMBI zumbi,ARRAY array ,int dimUm, int dimDois, EHCONST ehconst, ValConst valconst , int endereco){
    strcpy(tabela.tabelaS[topo].lexema, lexema);
    tabela.tabelaS[topo].escopo = escopo;
    
}