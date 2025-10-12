#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "analex.h"
#include "cores.h"
#include "anasint.h"
#include "auxfuncs.h"


int quantidadeIntrucoes = 0;
extern char instrBuffer[MAX_INTRUCOES][TAM_MAX_INSTRUCOES];


    void error(char *err, ...){
        printf(_RED_);
        va_list args;
        va_start(args, err);
        printf("Erro: ");
        vprintf(err,args);
        va_end(args);
        printf("Na linha %d\n", linha);
        printf(_NORMAL_);
        exit(1);
    }

void printTokenDados(){
    printf("\nCATEGORIA: %d, CÓDIGO: %d\n", tk.cat, tk.codigo);
}

void printFinalizacao(char msg[]){
    printf("%s na linha %d\n", msg, linha);
}

void escreveCodigoPilha(const char* format, ...) {
    va_list args;                    
    char buffer[256];                
    va_start(args, format);          
    vsnprintf(buffer, sizeof(buffer), format, args); 
    va_end(args);                    
    fputs(buffer, f_out);            
}
void bufferIntrucoes(char *inst,...){
    va_list args;
    va_start(args, inst);
        if (quantidadeIntrucoes < MAX_INTRUCOES) {
            vsnprintf(instrBuffer[quantidadeIntrucoes], TAM_MAX_INSTRUCOES, inst, args);
            quantidadeIntrucoes++;
        } else {
            fprintf(stderr, "Erro: Buffer de instruções cheio!\n");
        }

        va_end(args);
    }
