#include "analex.h"
#include "anasint.h"
#include <stdio.h>
#include <stdbool.h>

void opRel(){
	tk = analex(f);
	if(tk.cat == SN && (tk.codigo == MAIORQUE || tk.codigo == MAIORIGUAL || 
						tk.codigo == MENORQUE || tk.codigo == MENORIGUAL ||
						tk.codigo == NEGACAO  || tk.codigo == DIFERENTE  ||
						tk.codigo == IGUALDADE)){
		tk.processado = true;
	}
	else{
		error("Operador rel esperado");
	}
}

void Atrib() {
	if (tk.cat != ID) {
		error("Identificador esperado!\n");
	}
	tk.processado = true;
	tk = analex(f);
	if (tk.cat != SN || tk.codigo != ATRIBUICAO) {
		error("Sinal de atribuição esperado!\n");
	}
	tk.processado = true;

	Expr();
	

}

void Expr() {
	exprSimples();
	if(tk.cat == SN && (tk.codigo == MAIORQUE || tk.codigo == MAIORIGUAL || 
						tk.codigo == MENORQUE || tk.codigo == MENORIGUAL ||
						tk.codigo == NEGACAO  || tk.codigo == DIFERENTE  ||
						tk.codigo == IGUALDADE)){
		opRel();
		exprSimples();
	}
}

void exprSimples(){
	Termo();
	Resto();
}
void Termo(){
	Fator();
	Sobra();
}
void Resto() {
	tk = analex(f);
	if (tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUB || tk.codigo == OR)) {
		tk.processado = true;
		Termo();
		Resto();
	}
}

void Sobra() {
	tk = analex(f);
	if(tk.cat == SN && (tk.codigo == MULT || tk.codigo == DIV || tk.codigo == AND)) {
		tk.processado = true;
		Fator();
		Sobra();
	}
}

void Fator() {
	tk = analex(f);
	if (tk.cat == ID) { 
		tk.processado = true;
		/*TRATAR IDENTIFICADOR*/
	}
	else if (tk.cat == CT_I) {
		tk.processado = true; 
		/*TRATAR CONSTANTE INTEIRA*/
	}
	else if (tk.cat == SN && tk.codigo == PARENTESEABERTO) {
		tk.processado = true;
		Expr();
		if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) {
			error("Fecha parênteses esperado!\n");
		}
		else {
			tk.processado = true;
		}
	}
	else {
		error("Identificador, constante inteira ou abre parênteses esperado!\n");
	}
}

void declVar(){
	if(tk.cat != ID){
		error("Identificador esperado\n");
	}
		tk.processado = true;
		tk = analex(f);

		if(tk.cat == SN && tk.codigo == ATRIBUICAO){
			tk.processado = true;
			tk = analex(f);
			varInit();
		}
		else if(tk.cat == SN && tk.codigo == COLCHETEABERTO){
			while(tk.cat == SN && tk.codigo == COLCHETEABERTO){
				tk.processado = true;
				tk = analex(f);
				if(!(tk.cat != CT_I || tk.cat != ID)){
					error("Esperado constante inteira ou identificador \n");
				}
				tk.processado = true;
				tk = analex(f);
				if(tk.cat != SN || tk.codigo != COLCHETEFECHADO){
					error("Fechamento de colchetes esperado\n");
				}
				else{
					tk.processado = true;
					tk = analex(f);
				}
				 arrayInit();
				}
		}
	
	}

void varInit(){
	if(tk.cat == CT_C || tk.cat == CT_I || tk.cat == CT_R || tk.cat == LT){
			tk.processado = true;
			tk = analex(f);
	}
		else{
			error("Inicialização de variável inválida");
		}
}

void arrayInit(){
	if(tk.cat == SN && tk.codigo == ATRIBUICAO){
		tk.processado = true;
		tk = analex(f);
		varInit();
		if(tk.cat == SN && tk.codigo == VIRGULA){
			tk.processado = true;
			while(tk.cat == SN && tk.codigo == VIRGULA){
				tk = analex(f);
				varInit();
				}
			}
		}
}

void testeSint() {
    
    if ((f=fopen("text.text", "r")) == NULL)
        error("Arquivo de entrada da expressão nao encontrado!\n");

    tk.processado = true;

    while (true) {
        tk = analex(f);
        if (tk.cat == FIM_ARQ) {
            printf("\nFim do arquivo fonte encontrado!\n\n");
            break;
        }
        //Atrib();
		declVar();
        if (tk.cat = FIM_EXPR)
            printf("\nLINHA %d: Expressão sintaticamente correta!\n\n", linha - 1);
        else {
            error("Erro de sintaxe!\n");
        }
        tk.processado = true;
    }
    fclose(f);
}