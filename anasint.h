#ifndef _ANASINT_
#define _ANASINT_
#include "analex.h"
#include <stdio.h>

extern TOKEN tk;
extern FILE *f;
extern int linha;

void Atrib();
void Expr();
void Termo();
void Resto();
void Sobra();
void Fator();

void testeSint();

#endif