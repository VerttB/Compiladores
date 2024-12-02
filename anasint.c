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
	}
	else if (tk.cat == CT_I) {
		tk.processado = true; 
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
		if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("DO - Fechamento de parenteses esperado\n");
		tk.processado = true;
		tk = analex(f);
	}
	else if(tk.codigo == WHILE){
		printf("While Iniciado\n");
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
		tk = analex(f);
	}
	else if(tk.cat == ID) Atrib();
	else if(tk.codigo == GETOUT){
		tk.processado = true;
		tk = analex(f);
	}
	else if(tk.codigo == GETINT){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("ID esperado para funcionamento da função");
		tk.processado = true;
		tk = analex(f);
	}
	else if(tk.codigo == GETREAL){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("ID esperado para funcionamento da função");
		tk.processado = true;
		tk = analex(f);
	}
	else if(tk.codigo == GETCHAR){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("ID esperado para funcionamento da função");
		tk.processado = true;
		tk = analex(f);
	}
	else if(tk.codigo == GETSTR){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("ID esperado para funcionamento da função");
		tk.processado = true;
		tk = analex(f);
	}
	else if(tk.codigo == PUTINT){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID && tk.cat != CT_I) error("ID ou constante inteira esperado para funcionamento da função");
		tk.processado = true;
		tk = analex(f);
	}
	else if(tk.codigo == PUTREAL){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID && tk.cat != CT_R) error("ID ou constante real esperado para funcionamento da função");
		tk.processado = true;
		tk = analex(f);
	}
	
	else if(tk.codigo == PUTCHAR){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID && tk.cat != CT_C) error("ID ou constante char esperado para funcionamento da função");
		tk.processado = true;
		tk = analex(f);
	}
	else if(tk.codigo == PUTSTR){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID && tk.cat != LT) error("ID ou constante string esperado para funcionamento da função");
		tk.processado = true;
		tk = analex(f);
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
			printf("If iniciado\n");
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
				printf("Elif iniciado\n");
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
				printf("Else iniciado\n");
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
			tk = analex(f);
		}
		else if(tk.codigo == ENDI || tk.codigo == ENDW || tk.codigo == ENDP 
		|| tk.codigo == ENDV || tk.codigo == ELSE || tk.codigo == ELIF) error("Finalização de comando inesperada");
	
}

void declDefProc(){

	if(tk.cat != PV_R) error("Inicializador de Função esperado");
	tk.processado = true;
	if(tk.codigo == PROT){
		printf("Prot incializado e ");
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != ID) error("Identificador esperado");
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Abertura de parenteses esperado");
		tk.processado = true;
		tk = analex(f);
		if(!(tk.cat != SN && tk.codigo != PARENTESEFECHADO)){
			param();
			while(tk.codigo == COLCHETEABERTO){
				tk = analex(f);
				if(tk.codigo != COLCHETEFECHADO) error("Fechamento de colchetes esperado");
				tk.processado = true;
				tk = analex(f);
			}
			while(tk.codigo == VIRGULA){
				tk.processado = true;
				tk = analex(f);
				param();
				while(tk.codigo == COLCHETEABERTO){
				tk.processado = true;
				tk = analex(f);
				if(tk.codigo != COLCHETEFECHADO) error("Fechamento de colchetes esperado");
				tk.processado = true;
				tk = analex(f);
			}
			}
		}
		if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado");
		printFinalizacao("Prot finalizado ");
	} 
	else if(tk.codigo == DEF){
		tk.processado = true;
		tk = analex(f);
		if(tk.cat == PV_R && tk.codigo == INIT){
			printf("Init inicializado");
			tk.processado = true;
			tk = analex(f);
			while(tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == BOOL || tk.codigo == REAL)){
				declListVar();
			}
			while(tk.cat == PV_R || tk.cat == ID){
				if(tk.codigo == ENDP) break;
				tk.processado = true;
				cmd();
				tk = analex(f);
				tk.processado = true;
			}
		if(tk.codigo != ENDP) error("Finalização de procedimento init esperado");
		tk.processado = true;
		printFinalizacao("Init finalizado ");
		}
		else if(tk.cat == ID){
			printf("Def Id inicializado\n");
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != SN && tk.codigo != PARENTESEABERTO) error("Abertura de parenteses esperado");
			tk.codigo = true;
			tk = analex(f);
			if(!(tk.cat != SN && tk.codigo != PARENTESEFECHADO)){
				tk.processado = true;
				tk = analex(f);
				param();
				tk.processado = true;
				if(tk.cat != ID) error("PROC ID = Identificador Esperado");
				tk.processado = true;
				tk = analex(f);
				while(tk.codigo == COLCHETEABERTO){
					tk = analex(f);
					if(tk.codigo != COLCHETEFECHADO) error("Fechamento de colchetes esperado");
					tk.processado = true;
					tk = analex(f);
				}
				while(tk.codigo == VIRGULA){
					tk.processado = true;
					tk = analex(f);
					param();
					tk.processado = true;
					if(tk.cat != ID) error("Identificador Esperado");
					tk.processado = true;
					tk = analex(f);
					while(tk.codigo == COLCHETEABERTO){
					tk.processado = true;
					tk = analex(f);
					if(tk.codigo != COLCHETEFECHADO) error("Fechamento de colchetes esperado");
					tk.processado = true;
					tk = analex(f);
				}
			}
		}
		if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado");
		tk.processado = true;
			tk = analex(f);
			while(tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == BOOL || tk.codigo == REAL)){
				declListVar();
			}
			while(tk.cat == PV_R || tk.cat == ID){
				if(tk.codigo == ENDP) break;
				tk.processado = true;
				cmd();
				tk = analex(f);
				tk.processado = true;
			}

		if(tk.codigo != ENDP) error("Finalização de procedimento id esperado");
			tk.processado = true;
			printFinalizacao("Finalização de DEF ID");
		}
		else error("Inicializador ou identificador esperado");

		
	}
}
void param(){
	if(tk.codigo == ENDERECO){
				tk.processado = true;
				tk = analex(f);
			}
	tipo();
			
}

void prog(){
	while(tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == BOOL || tk.codigo == REAL)){
		declListVar();
	}
	while(tk.cat == PV_R && (tk.codigo == PROT || tk.codigo == DEF)){
		declDefProc();
	}
}
void testeSint(char *p) {
    
    if ((f=fopen(p, "r")) == NULL)
        error("Arquivo de entrada da expressão nao encontrado!\n");

    tk.processado = true;

    while (true) {
        tk = analex(f);
        if (tk.cat == FIM_ARQ) {
            printf("\n------------------------------------\n");
        	printf("Fim do arquivo %s", p);
        	printf("\n------------------------------------\n");
            break;
        }
		prog();
        tk.processado = true;
    }
    fclose(f);
}