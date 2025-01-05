#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/analex.h"
#include "src/anasint.h"
#include "src/tabela_simbolos.h"
#include "src/ansi_escape.h"

int linha;
FILE *f, *f_out;
TOKEN tk;

int main(int argc, char *argv[]){
    printf("%s\n", argv[0]);
    for(int i = 0; i< argc-1; i++){
        printf("\n------------------------------------\n");
        printf("Lendo %s", argv[i+1]);
        printf("\n------------------------------------\n");
       // testeAnalex(argv[i+1]);
        linha = 1;
        setupConsole();
        testeSint(argv[i+1]);
        printarTabela(-1);
        limparTabela();
        restoreConsole();
    }
}
