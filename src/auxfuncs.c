#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"
#include "cores.h"
void error(char err[]){
    printf(_RED_);
	printf("%s na linha %d\n", err, linha-1);
    printf(_NORMAL_);
	exit(1);
}

void printTokenDados(){
    printf("\nCATEGORIA: %d, CÓDIGO: %d\n", tk.cat, tk.codigo);
}

void printFinalizacao(char msg[]){
    printf("%s na linha %d\n", msg, linha);
}
