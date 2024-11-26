#include "analex.h"
#include "anasint.h"
#include <stdio.h>
#include <stdbool.h>

void Atrib() {
	if (tk.cat != ID) {
		error("Identificador esperado!");
	}
	tk.processado = true;
	tk = analex(f);
	if (tk.cat != SN || tk.codigo != ATRIBUICAO) {
		error("Sinal de atribuição esperado!");
	}
	tk.processado = true;
	Expr();  
}

void Expr() {
	Termo();
	Resto();
}

void Termo(){
	Fator();
	Sobra();
}
void Resto() {
	tk = analex(f);
	if (tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUB)) {
		tk.processado = true;
		Termo();
		Resto();
	}
}

void Sobra() {
	tk = analex(f);
	if(tk.cat == SN && (tk.codigo == MULT || tk.codigo == DIV)) {
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
        printf("%d", tk.codigo);
		if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) {
			error("Fecha parênteses esperado!");
		}
		else {
			tk.processado = true;
		}
	}
	else {
		error("Identificador, constante inteira ou abre parênteses esperado!");
	}
}

void dec_list_var(){

}

void testeSint() {
    
    if ((f=fopen("text.text", "r")) == NULL)
        error("Arquivo de entrada da expressão nao encontrado!");

    tk.processado = true;
    while (true) {
        tk = analex(f);
        if (tk.cat == FIM_ARQ) {
            printf("\nFim do arquivo fonte encontrado!\n");
            break;
        }
        Atrib();
        printf("%d", tk.cat);
        if (tk.cat = FIM_EXPR)
            printf("\nLINHA %d: Expressão sintaticamente correta!\n\n", linha - 1);
        else {
            error("Erro de sintaxe!");
        }
        tk.processado = true;
    }
    fclose(f);
}