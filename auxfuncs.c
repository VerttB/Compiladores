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
    printf("CATEGORIA: %d, CÓDIGO: %d\n", tk.cat, tk.codigo);
}