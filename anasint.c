#include "analex.h"
#include "anasint.h"
#include "auxfuncs.h"
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
	consomeEnter();
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
			while(tk.cat == SN && tk.codigo == VIRGULA){
			tk.processado = true;
				tk = analex(f);
				varInit();
				}
			}
		}
}

void declListVar(){
	if(tk.cat == PV_R && tk.codigo == CONST){
		tk.processado = true;
		tk = analex(f);
	}
	tipo();
	declVar();
		while(tk.cat == SN && tk.codigo == VIRGULA){
			tk.processado = true;
			tk = analex(f);
			declVar();
		}
	
}
void tipo(){
	if(tk.codigo == INT || tk.codigo == REAL || tk.codigo == BOOL || tk.codigo == CHAR){
		tk.processado = true;
		tk = analex(f);
	}
	else{
		error("Esperado declaração de tipo de variável\n");
	}
}
//---------------------------------
void cmd(){
	printTokenDados();
	if(tk.cat != PV_R && tk.cat != ID) error("CMD - Identificador ou palavra chave esperado");
	if(tk.cat == PV_R && (tk.codigo == PROT || tk.codigo == DEF)) error("CMD - Palavra Chave Inválida");
	if(tk.codigo == DO){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("Esperado ID do procedimento\n");
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Esperado abertura de parenteses\n");
		tk.processado = true;
		tk = analex(f);
		if(tk.cat == ID){
			tk.processado = true;
			tk = analex(f);
			if(tk.cat == SN && tk.codigo == VIRGULA){
				while(tk.cat == SN && tk.codigo == VIRGULA){
					tk.processado = true;
					tk = analex(f);
					if(!tk.cat == ID) error("Identificador Válido Esperado");
					tk.processado = true;
					tk = analex(f);
				}
			}
		}
		if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado\n");
	}
	else if(tk.codigo == WHILE){
		printf("While Iniciado e ");
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Esperado abertura de parenteses\n");
		tk.processado = true;
		Expr();
		if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado\n");
		tk.processado = true;
		tk = analex(f);
		while(tk.codigo != ENDW){
			  if (tk.cat == FIM_ARQ) {
                error("Fim do arquivo inesperado dentro do loop WHILE");
            }
			tk.processado = true;
			cmd();
			tk = analex(f);
		}
		if(tk.codigo != ENDW) error("ENDW esperado\n");
		printFinalizacao("While finalizado corretamente");
		tk.processado = true;
	}
	else if(tk.cat == ID) Atrib();
	else if(tk.codigo == GETOUT){
		tk.processado = true;
	}
	else if(tk.codigo == GETINT){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("ID esperado para funcionamento da função");
		tk.processado = true;
	}
	else if(tk.codigo == GETREAL){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("ID esperado para funcionamento da função");
		tk.processado = true;
	}
	else if(tk.codigo == GETCHAR){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("ID esperado para funcionamento da função");
		tk.processado = true;
	}
	else if(tk.codigo == GETSTR){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("ID esperado para funcionamento da função");
		tk.processado = true;
	}
	else if(tk.codigo == PUTINT){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID && tk.cat != CT_I) error("ID ou constante inteira esperado para funcionamento da função");
		tk.processado = true;
	}
	else if(tk.codigo == PUTREAL){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID && tk.cat != CT_R) error("ID ou constante real esperado para funcionamento da função");
		tk.processado = true;
	}
	else if(tk.codigo == PUTCHAR){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID && tk.cat != CT_C) error("ID ou constante char esperado para funcionamento da função");
		tk.processado = true;
	}
	else if(tk.codigo == PUTSTR){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID && tk.cat != LT) error("ID ou constante string esperado para funcionamento da função");
		tk.processado = true;
	}
	else if(tk.codigo == VAR){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("Identificador Esperado");
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != PV_R && tk.codigo != FROM) error("Palavra chave from esperado");
		tk.processado = true;
		Expr();
		if(tk.cat != PV_R && (tk.codigo != DT || tk.codigo != TO)) error("Esperado DT ou TO");
		tk.processado = true;
		Expr();

		if(tk.cat == PV_R && tk.codigo == BY){
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID && tk.cat != CT_I) error("VAR: Identificador ou constante inteira esperada");
			tk.processado = true;
			tk = analex(f);
		} 
		tk.processado = true;
		while(tk.codigo != ENDV){
			 if (tk.cat == FIM_ARQ) {
                error("Fim do arquivo inesperado dentro do loop WHILE");
            }
			//consomeEnter();
			tk.processado = true;
			cmd();
			tk = analex(f);
		}
		if(tk.cat != PV_R && tk.codigo != ENDV) error("VAR: Fim do comando esperado");
		printFinalizacao("Expressão Var finalizada com sucesso na linha");
		tk.processado = true;
		}
		else if(tk.codigo == IF){
			printf("If iniciado e ");
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Esperado abertura de parenteses\n");
			tk.processado = true;
			Expr();
			if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado\n");
			tk.processado = true;
			tk = analex(f);
			while(tk.codigo != ELSE && tk.codigo != ELIF && tk.codigo != ENDI){
				 if (tk.cat == FIM_ARQ) {
                	error("Fim do arquivo inesperado dentro do loop WHILE");
            }
				tk.processado = true;
				cmd();
				tk = analex(f);
			}
			while(tk.codigo == ELIF){
				printf("Elif iniciado e ");
				tk.processado = true;
				tk = analex(f);
				if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Esperado abertura de parenteses\n");
				tk.processado = true;
				Expr();
				if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado\n");
				tk.processado = true;
				tk = analex(f);
				while(tk.codigo != ELIF && tk.codigo != ELSE && tk.codigo != ENDI){
					 if (tk.cat == FIM_ARQ) {
                	error("Fim do arquivo inesperado dentro do loop WHILE");
           			 }
	
					tk.processado = true;
					cmd();
					tk = analex(f);
				}
				printFinalizacao("Elif finalizado ");	
			}
			if(tk.codigo == ELSE){
				printf("Else iniciado e ");
				tk.processado = true;
				tk = analex(f);
				while(tk.codigo != ENDI){
					 if (tk.cat == FIM_ARQ) error("Fim do arquivo inesperado dentro do loop WHILE");
					tk.processado = true;
					cmd();
					tk = analex(f);
				}
			}
			printFinalizacao("Finalização de Else");
			if(tk.codigo != ENDI) error("Esperada finalização de If");
			printFinalizacao("Finalização de If ");
			tk.processado = true;
		}
		else if(tk.codigo == ENDI || tk.codigo == ENDW || tk.codigo == ENDP 
		|| tk.codigo == ENDV || tk.codigo == ELSE || tk.codigo == ELIF) error("Finalização de comando inesperada");
	
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
		//declVar();
		//declListVar();
		cmd();
        tk.processado = true;
    }
    fclose(f);
}