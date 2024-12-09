#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "analex.h"
#include "cores.h"
    void error(char *err, ...){
        printf(_RED_);
        va_list args;
        va_start(args, err);
        printf("Erro: ");
        vprintf(err,args);
        va_end(args);
        printf(" Na linha %d\n", linha-1);
        printf(_NORMAL_);
        exit(1);
    }

void printTokenDados(){
    printf("\nCATEGORIA: %d, CÓDIGO: %d\n", tk.cat, tk.codigo);
}

void printFinalizacao(char msg[]){
    printf("%s na linha %d\n", msg, linha);
}
