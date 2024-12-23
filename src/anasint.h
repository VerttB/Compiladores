#ifndef _ANASINT_
#define _ANASINT_
#include "analex.h"
#include <stdio.h>

extern TOKEN tk;
extern FILE *f, *f_out;
extern int linha;
void testeSint(char *);


//Lidam com expressão
void Atrib();
void Expr();
void exprSimples();
void Termo();
void Resto();
void Sobra();
void Fator();
void opRel();


//Lidam com procedimentos e seus parâmetros
void prog();
void declDefProc();
void param();

// Lidam com os comandos
void cmd();
void cmdWhile();
void cmdDo();
void cmdGets();
void cmdPuts();
void cmdVar();
void cmdIf();
//Lidam com inicialização de variaveis e arrays
void declVar();
void declListVar();
void tipo();
void varInit(); 
void validarVarInit();
void atribuirConst();
void arrayInit(); 
void insereDimensaoArray(int);
void defineTipoArray(int);

#endif