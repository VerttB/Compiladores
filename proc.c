#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"
#include "anasint.h"
#include "tabela_simbolos.h"

int linha;
FILE *f;
TOKEN tk;

int main(int argc, char *argv[]){
    printf("%s\n", argv[0]);
    for(int i = 0; i< argc-1; i++){
        printf("\n------------------------------------\n");
        printf("Lendo %s", argv[i+1]);
        printf("\n------------------------------------\n");
        //testeAnalex(argv[i+1]);
        linha = 1;
        testeSint(argv[i+1]);
        printarTabela();
        limparTabela();
    }
}