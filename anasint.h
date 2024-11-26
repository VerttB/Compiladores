#ifndef _ANASINT_
#define _ANASINT_
#include "analex.h"
#include <stdio.h>

extern TOKEN tk;
extern FILE *f;
extern int linha;

void Atrib();
void Expr();
void exprSimples();
void Termo();
void Resto();
void Sobra();
void Fator();
void opRel();

void testeSint();


void declVar();
void varInit(); //Trata das inicializações intcon |realcon | charcon | stringcon
void arrayInit(); //Trata das inicializações de array = {}
#endif