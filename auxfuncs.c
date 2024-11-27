#include <stdio.h>
#include <stdlib.h>
#include "analex.h"

void consomeEnter(){
    while(tk.cat == FIM_EXPR){
        tk.processado = true;
        tk = analex(f);
    }
}

void printTokenDados(){
    printf("\nCATEGORIA: %d, CÓDIGO: %d\n", tk.cat, tk.codigo);
}

void printFinalizacao(char msg[]){
    printf("%s na linha %d\n", msg, linha);
}