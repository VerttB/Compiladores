#ifndef __AUXFUNCS_H
#define ___AUXFUNCS_H

#define MAX_INTRUCOES 2048
#define TAM_MAX_INSTRUCOES 20

void printTokenDados();
void printFinalizacao(char []);
void error(char *, ...);
void escreveCodigoPilha(const char* , ... );
void bufferIntrucoes(char *inst,...);
extern char instrBuffer[MAX_INTRUCOES][TAM_MAX_INSTRUCOES];
extern int quantidadeIntrucoes;
extern int linha;

#endif