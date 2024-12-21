#ifndef _ANASINT_
#define _ANASINT_
#include "analex.h"
#include <stdio.h>

extern TOKEN tk;
extern FILE *f, *f_out;
extern int linha;

void Atrib();
void Expr();
void exprSimples();
void Termo();
void Resto();
void Sobra();
void Fator();
void opRel();

void testeSint(char *);


//

void prog();
void declDefProc();
void param();
void cmd();

void trataGets();
//Lidam com inicialização de variaveis
void declVar();
void declListVar();
void tipo();


void varInit(); //Trata das inicializações intcon |realcon | charcon | stringcon
void arrayInit(); //Trata das inicializações de array = {}
void insereDimensaoArray(int);
void defineTipoArray(int);
#endif