
	#include <stdio.h>
	#include <stdbool.h>
	#include <string.h>
	#include "analex.h"
	#include "anasint.h"
	#include "auxfuncs.h"
	#include "tabela_simbolos.h"
	#include "cores.h"

	TokenInfo tokenInfo, aux;
	bool exprValida = true;
	int tokenEndereco = 0;
	int tipoExpressao;

	int opRel(){
		tk = analex(f);
		if(tk.cat == SN && (tk.codigo == MAIORQUE || tk.codigo == MAIORIGUAL || 
							tk.codigo == MENORQUE || tk.codigo == MENORIGUAL ||
							tk.codigo == NEGACAO  || tk.codigo == DIFERENTE  ||
							tk.codigo == IGUALDADE)){
			tk.processado = true;
			return tk.codigo;
		}
		error("Operador rel esperado");
		return -1;
	
	}

	void Atrib() {
		
		int dimensaoArray = 0;
		if (tk.cat != ID) {
			error("Identificador esperado!\n");
		}
		aux = buscaDecl(tk.lexema);
		if(aux.idcategoria == PROC || aux.idcategoria == PROT) error("Chamada incorreta de procedimento");
		tk.processado = true;
		tk = analex(f);
		while(tk.cat == SN && tk.codigo == COLCHETEABERTO){
			dimensaoArray++;
			if(dimensaoArray > 2) error("Vetor de tamanho inválido");
			if(dimensaoArray == 2 && aux.array != MATRIZ) error("Array de dimensão inválido");
			tk.processado = true;
			tk = analex(f);
			Expr();
			if(tk.cat != SN && tk.codigo != COLCHETEFECHADO) error("Fechamento de colchetes esperado");
			tk.processado = true;
			tk = analex(f);
			
		}
		if(dimensaoArray == 1 && aux.array == VETOR) error("Array de dimensão inválida");
		if (tk.cat != SN || tk.codigo != ATRIBUICAO) {
			error("Sinal de atribuição esperado!\n");
		}
		tk.processado = true;

		Expr();
		escreveCodigoPilha("STOR %d,%d\n", (aux.escopo == GLOBAL ? 0 : 1) ,aux.endereco);
		

	}

	void Expr() {
		int codigoOp;
		char rotulo1[20],rotulo2[20], rotulo3[20];
		exprSimples();
		while (tk.cat == SN && (tk.codigo == MAIORQUE || tk.codigo == MAIORIGUAL || 
							tk.codigo == MENORQUE || tk.codigo == MENORIGUAL ||
							tk.codigo == DIFERENTE  ||
							tk.codigo == IGUALDADE)){
			
			codigoOp = opRel();
			exprSimples();
			strcpy(rotulo1, geraRotulo());
			strcpy(rotulo2, geraRotulo());
			escreveCodigoPilha("SUB\n");
			switch (codigoOp){
			case MAIORQUE:
				escreveCodigoPilha("GOTRUE %s\nPUSH 0\nGOTO %s\nLABEL %s\nPUSH 1\nLABEL %s\n", rotulo1, rotulo2, rotulo1, rotulo2);
				break;
			case MAIORIGUAL:
				strcpy(rotulo3, geraRotulo());
				escreveCodigoPilha("COPY\nGOFALSE %s\nGOTRUE %s\nPUSH 0\nGOTO %s\nLABEL %s\nPOP\nLABEL %s\nPUSH 1\nLABEL %s\n", rotulo1,rotulo2,rotulo3, rotulo2, rotulo3);
				break;
			case MENORQUE:
				strcpy(rotulo3, geraRotulo());
				escreveCodigoPilha("COPY\nGOTRUE %s\nGOFALSE %s\nPUSH 1\nGOTO %s\nLABEL %s\nPOP\nLABEL %s\nPUSH 0\nLABEL %s\n", rotulo1, rotulo2, rotulo3, rotulo1, rotulo2, rotulo3);
				break;
			case DIFERENTE:
				escreveCodigoPilha("GOFALSE %s\nPUSH 1\nGOTO %s\nLABEL %S\nPUSH 0\nLABEL %s", rotulo1,rotulo2, rotulo1, rotulo2);
				break;
			case IGUALDADE:
				escreveCodigoPilha("GOFALSE %s\nPUSH 0\nGOTO %s\nLABEL %s\nPUSH 1\nLABEL %s\n", rotulo1,rotulo2,rotulo1,rotulo2);
				break;
			case MENORIGUAL:
				escreveCodigoPilha("GOTRUE %s\nPUSH 1\nGOTO %s\nLABEL %s\nPUSH 0\nLABEL %s\n", rotulo1,rotulo2,rotulo1,rotulo2);
				break;
			default:
				error("Operador relacional desconhecido");
				break;
			}
			exprValida = true;
	}

	if(!(strcmp(aux.lexema, "while") == 0 || strcmp(aux.lexema, "if") == 0 || strcmp(aux.lexema,"var") == 0 || aux.tipo == BOOL_)) exprValida = true;
	
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
		int codigoOp;
		char orRotulo[20];
		tk = analex(f);
		if (tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUB || tk.codigo == OR)) {
			codigoOp = tk.codigo;
			tk.processado = true;
			if(codigoOp == OR){
				strcpy(orRotulo, geraRotulo());
				escreveCodigoPilha("COPY\nGOTRUE %s\nPOP\n", orRotulo);
				}
			Termo();
				if(codigoOp == ADICAO) escreveCodigoPilha("ADD\n");
				else if(codigoOp == SUB) escreveCodigoPilha("SUB\n");
			Resto();
			if(codigoOp == OR) escreveCodigoPilha("LABEL %s\n", orRotulo);
		}
	}

	void Sobra() {
		int codigoOp;
		char andRotulo[20];
		tk = analex(f);
		if(tk.cat == SN && (tk.codigo == MULT || tk.codigo == DIV || tk.codigo == AND)) {
			codigoOp = tk.codigo;
			tk.processado = true;
			if(codigoOp == AND){
				strcpy(andRotulo, geraRotulo());
				escreveCodigoPilha("COPY\nGOFALSE %s\nPOP\n", andRotulo);
			}
			Fator();
				if(codigoOp == MULT) fputs("MULT\n", f_out);
				else if(codigoOp == DIV) fputs("DIV\n", f_out);
			Sobra();
			if(codigoOp == AND) escreveCodigoPilha("LABEL %s\n", andRotulo);
		}
	}

	void Fator() {
		tk = analex(f);
		if (tk.cat == ID) { 
			TokenInfo TypesAux;
			TypesAux = buscaDecl(tk.lexema);
			if(aux.tipo == REAL_ && TypesAux.tipo != REAL_) error("Erro semântico, atribuição de tipo inválido. Esperado %s, recebido %s. ", T_tipo[aux.tipo], T_tipo[TypesAux.tipo]);
			if((aux.tipo == CHAR_ || aux.tipo == INT_ || aux.tipo == BOOL_) && TypesAux.tipo == REAL_) error("Erro semântico, atribuição de tipo inválido. Esperado %s, recebido %s. ", T_tipo[aux.tipo], T_tipo[TypesAux.tipo]);
			//if(aux.tipo == BOOL_ && TypesAux.tipo != INT_) error("Erro semãntico, atribuição de tipo inválida. Esperado %s, recebido %s. ", T_tipo[aux.tipo], T_tipo[TypesAux.tipo]);
			if(TypesAux.tipo == CHAR_ || TypesAux.tipo == REAL_) exprValida = false;

			escreveCodigoPilha("LOAD %d,%d\n", (TypesAux.escopo == GLOBAL ? 0 : 1), TypesAux.endereco);
			
			tk.processado = true;
			tk = analex(f);
			int dimensaoArray = 0;
			while(tk.codigo == COLCHETEABERTO){
				dimensaoArray++;
				if(dimensaoArray > 2) error("Expr - Matriz de tamanho inválido");
				tk.processado = true;
				Expr();
				if(tk.cat != SN && tk.codigo != COLCHETEFECHADO) error("Fechamento de colchetes esperado");			
				tk.processado = true;
				tk = analex(f);
			}

		}
		else if (tk.cat == CT_I || tk.cat == CT_R || tk.cat == CT_C) {
			if(tk.cat == CT_I){
				if(aux.tipo == REAL_) error("Erro semântico, atribuição de tipo inválido. Esperado %s ou %s, recebido %s. ", T_tipo[CHAR_], T_tipo[INT_], T_tipo[aux.tipo]);
				escreveCodigoPilha("PUSH %d\n", tk.valor);
			}
			else if(tk.cat == CT_R){
				if(aux.tipo != REAL_ && !exprValida) error("Erro semântico, atribuição de tipo inválido. Esperado %s, recebido %s. ", T_tipo[aux.tipo], T_tipo[REAL_]);
				if(aux.array != SIMPLES) error("Índices de vetor devem ser somente do tipo inteiro");
				exprValida = false;
				escreveCodigoPilha("PUSHF %f\n", tk.valor_r);
			}
			else{
				if(aux.tipo == REAL) error("Erro semântico, atribuição de tipo inválido. Esperado %s, recebido %s. ", T_tipo[aux.tipo], T_tipo[CHAR_]);
				if(aux.array != SIMPLES) error("Índices de vetor devem ser somente do tipo inteiro");
				exprValida = false;
				escreveCodigoPilha("PUSH %d\n", tk.c);
			}
			tk.processado = true; 
			tk = analex(f);
			
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
		else if(tk.cat == SN && tk.codigo == NEGACAO){
			char notRotulo[20], endNotRotulo[20];
			tk.processado = true;
			tk = analex(f);
			Fator();
			strcpy(notRotulo, geraRotulo());
			strcpy(endNotRotulo, geraRotulo());
			escreveCodigoPilha("GOTRUE %s\nPUSH 1\nGOTO %s\nLABEL %s\nPUSH 0\nLABEL %s\n", notRotulo, endNotRotulo, notRotulo, endNotRotulo);
		}
		else {
			error("Identificador, constante inteira ou abre parênteses esperado!\n");
		}
	}

	void declVar(){
		if(tk.cat != ID){
			error("Identificador esperado\n");
		}
			strcpy(tokenInfo.lexema, tk.lexema);
			tk.processado = true;
			tk = analex(f);

		if(tokenInfo.ehConst == CONST_){
			if(tk.cat != SN && tk.codigo != ATRIBUICAO) error("Variável constante %s não inicializada", tokenInfo.lexema);
		}
			if(tk.cat == SN && tk.codigo == ATRIBUICAO){
				tk.processado = true;
				tk = analex(f);

				varInit();
			}
			else if(tk.cat == SN && tk.codigo == COLCHETEABERTO){
				int dimensaoArray = 0;
				while(tk.cat == SN && tk.codigo == COLCHETEABERTO){
					dimensaoArray++;
					if(dimensaoArray > 2) error("PROC ID - Matriz de tamanho inválido");
					tk.processado = true;
					tk = analex(f);
					insereDimensaoArray(dimensaoArray);
					tk.processado = true;
					tk = analex(f);
					if(tk.cat != SN && tk.codigo != COLCHETEFECHADO){
						error("Fechamento de colchetes esperado\n");
					}
					tk.processado = true;
					tk = analex(f);
					}
					arrayInit();
					defineTipoArray(dimensaoArray);
			}
			tokenInfo.endereco = tokenEndereco;
			tokenEndereco++;
			inserirNaTabela(tokenInfo); //Insere as declarações de variável
		}

		void varInit(){
			if(tk.cat == CT_C || tk.cat == CT_I || tk.cat == CT_R){
					validarVarInit();
					if(tokenInfo.ehConst == CONST_){
						atribuirConst();
					}
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
			if(tk.cat != SN && tk.codigo != CHAVEABERTA) error("Abertura de chaves esperado");
			tk.processado = true;
			tk = analex(f);
			varInit();
			while(tk.cat == SN && tk.codigo == VIRGULA){
				tk.processado = true;
					tk = analex(f);
					varInit();
				}
				

			
			if(tk.cat != SN && tk.codigo != CHAVEFECHADA) error("Fechamento de chaves esperado");
			tk.processado = true;
			tk = analex(f);
		}
	}

	int declListVar(){
		int countVar = 0;
		tokenInfo.ehConst = NORMAL;
		tokenInfo.passagem = NA_PASSAGEM;
		tokenInfo.zumbi = NA_ZUMBI;
		tokenInfo.array = SIMPLES;
		tokenInfo.arrayDim[0] = 0;
		tokenInfo.arrayDim[1] = 0;
		if(tk.cat == PV_R && tk.codigo == CONST){
			tk.processado = true;
			tk = analex(f);
			tokenInfo.ehConst = CONST_;
		}
		tipo();
		declVar();
		countVar++;
			while(tk.cat == SN && tk.codigo == VIRGULA){
				tk.processado = true;
				tk = analex(f);
				declVar();
				countVar++;
			}
		return countVar;
	}
	void tipo(){
		if(tk.cat == PV_R && (tk.codigo == INT || tk.codigo == REAL || tk.codigo == BOOL || tk.codigo == CHAR)){
			if (tk.codigo == INT) tokenInfo.tipo = INT_;
			else if(tk.codigo == REAL) tokenInfo.tipo = REAL_;
			else if(tk.codigo == CHAR) tokenInfo.tipo = CHAR_;
			else tokenInfo.tipo = BOOL_;
			tk.processado = true;
			tk = analex(f);
		}
		else{
			error("Esperado declaração de tipo de variável\n");
		}
	}
	void cmd(){
		if(tk.cat != PV_R && tk.cat != ID) error("CMD - Identificador ou palavra chave esperado");
		if(tk.cat == PV_R && (tk.codigo == PROT || tk.codigo == DEF)) error("CMD - Palavra Chave Inválida"); //Inválido se for DEF ou PROT
		if(tk.codigo == DO) cmdDo();
		else if(tk.codigo == WHILE) cmdWhile();
		else if(tk.cat == ID) Atrib();
		else if(tk.codigo == GETINT || tk.codigo == GETOUT || tk.codigo == GETREAL ||tk.codigo == GETCHAR || tk.codigo == GETSTR) cmdGets();
		else if(tk.codigo == PUTINT || tk.codigo == PUTREAL || tk.codigo == PUTREAL || tk.codigo == PUTSTR || tk.codigo == PUTCHAR) cmdPuts();
		else if(tk.codigo == VAR) cmdVar();
		else if(tk.codigo == IF) cmdIf();
			else if(tk.cat == PV_R  && (tk.codigo == ENDI || tk.codigo == ENDW || tk.codigo == ENDP 
			|| tk.codigo == ENDV || tk.codigo == ELSE || tk.codigo == ELIF)) error("Finalização de comando inesperada");
		
	}

	void declProc(){
		if(tk.cat != PV_R) error("Inicializador de Função esperado");
			tokenInfo.array = NA_ARRAY;
			tokenInfo.escopo = GLOBAL;
			tokenInfo.ehConst = NORMAL;
			tokenInfo.zumbi = NA_ZUMBI;
			tokenInfo.passagem = NA_PASSAGEM;
			tokenInfo.tipo = NA_TIPO;
			tokenInfo.idcategoria = PROC;
			tk.processado = true;
		if(tk.codigo == PROT) declProt();
		else if(tk.codigo == DEF) declDef();
	}
	void param(){
		tokenInfo.idcategoria = PROC_PAR;
		tokenInfo.escopo = LOCAL;
		tokenInfo.ehConst = NORMAL;
		if(tk.codigo == ENDERECO){
					tk.processado = true;
					tk = analex(f);
					tokenInfo.passagem = REFERENCIA;
				}
		else tokenInfo.passagem = COPIA;
		tipo();
				
	}

	void insereDimensaoArray(int dimensaoArray){
		if(tk.cat == CT_I){
			tokenInfo.arrayDim[dimensaoArray-1] = tk.valor;
		}
		else if(tk.cat == ID){
					aux = buscaDecl(tk.lexema);
					if(aux.ehConst != CONST_) error("Identificador de tamanho de array deve ser constante");
					if(aux.tipo != INT_) error("Identificador deve ser do tipo inteiro");
					tokenInfo.arrayDim[dimensaoArray-1] = aux.valConst.int_const;
				}
			else{
				error("Esperado constante inteira ou identificador \n");
			}
	}

	void defineTipoArray(int dimensaoArray){
		if(dimensaoArray == 1) tokenInfo.array = VETOR;
		else if(dimensaoArray == 2) tokenInfo.array = MATRIZ;
		else tokenInfo.array = SIMPLES;
	}


	void cmdDo(){
			int pos, qtdParam = 0;
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID) error("Esperado ID do procedimento\n");
			aux = buscaDecl(tk.lexema);
			escreveCodigoPilha("CALL %s\n",aux.rotulo);
			pos = buscaLexPos(aux.lexema) + 1;
			contaParam(pos-1, &qtdParam);
			if(aux.idcategoria != PROC && aux.idcategoria != PROT_) error("Esperado %s ou %s, recebido %s", T_IdCategoria[PROC], T_IdCategoria[PROT_],T_IdCategoria[aux.tipo]);
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Esperado abertura de parenteses\n");
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != SN || tk.codigo != PARENTESEFECHADO){
			if(qtdParam > 0 && tabela.tokensTab[pos].idcategoria != PROC_PAR) error("Quantidade inválida de argumentos");
				aux = tabela.tokensTab[pos];
				Expr();
				pos++;
				qtdParam--;
				while(tk.cat == SN && tk.codigo == VIRGULA){
						if(tabela.tokensTab[pos].idcategoria != PROC_PAR) error("Quantidade de argumentos maior que a esperada");
						aux = tabela.tokensTab[pos];
						tk.processado = true;
						tk = analex(f);
						Expr();
						pos++;
						qtdParam--;
					}
			}
			if(qtdParam > 0) error("Quantidade de argumentos passada menor que a esperada");
			if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("DO - Fechamento de parenteses esperado\n");
			tk.processado = true;
			tk = analex(f);
	}
	
	void cmdWhile(){
			char startWhile[20], endWhile[20];
			strcpy(startWhile, geraRotulo());
			escreveCodigoPilha("LABEL %s\n", startWhile);
			strcpy(aux.lexema, "while");
			//aux.tipo = BOOL_;
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Esperado abertura de parenteses\n");
			tk.processado = true;
			Expr();
			if(!exprValida) error("Expressão deve ser de tipo booleano. ");
			if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado\n");
			strcpy(endWhile, geraRotulo());
			escreveCodigoPilha("GOFALSE %s\n", endWhile);
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
			escreveCodigoPilha("GOTO %s\n", startWhile);
			escreveCodigoPilha("LABEL %s\n", endWhile);
			if(tk.codigo != ENDW) error("ENDW esperado\n");
			tk.processado = true;
			tk = analex(f);
	}
	void cmdGets(){
		if(tk.codigo == GETOUT){
			tk.processado = true;
			tk = analex(f);
		}
		else if(tk.codigo == GETINT){
			fputs("GET_I\n", f_out);
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID) error("ID esperado para funcionamento da função");
			aux = buscaDecl(tk.lexema);
			if(aux.tipo == BOOL_ || aux.tipo == REAL_) error("Erro de I/O esperado INT/CHAR, recebido BOOL/REAL");
			tk.processado = true;
			tk = analex(f);
		}
		else if(tk.codigo == GETREAL){
			fputs("GET_F\n", f_out);
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID) error("ID esperado para funcionamento da função");
			aux = buscaDecl(tk.lexema);
			if(aux.tipo != REAL_) error("Erro de I/O esperado REAL , recebido BOOL/INT/CHAR");
			tk.processado = true;
			tk = analex(f);
		}
		else if(tk.codigo == GETCHAR){
			fputs("GET_C\n", f_out);
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID) error("ID esperado para funcionamento da função");
			aux = buscaDecl(tk.lexema);
			if(aux.tipo == BOOL_ || aux.tipo == REAL_) error("Erro de I/O esperado INT/CHAR, recebido BOOL/REAL");
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
		escreveCodigoPilha("STOR %d,%d\n", (aux.escopo == GLOBAL ? 0 : 1), aux.endereco);
	}

	void cmdVar(){
			strcpy(aux.lexema, "var");
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
				tk.processado = true;
				cmd();
				tk = analex(f);
			}
			if(tk.cat != PV_R && tk.codigo != ENDV) error("VAR: Fim do comando esperado");
			tk.processado = true;
			
	}

	void cmdIf(){
		char opFalsa[20], opVerdadeira[20];
		strcpy(aux.lexema, "if");
		
		tk.processado = true;
		tk = analex(f);
		if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Esperado abertura de parenteses. ");
		tk.processado = true;
		Expr();
		if(!exprValida) error("Expressão deve ser de tipo booleano. ");
		if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado. ");
		tk.processado = true;
		tk = analex(f);
		strcpy(opFalsa, geraRotulo());
		escreveCodigoPilha("GOFALSE %s\n", opFalsa);
		while(tk.codigo != ELSE && tk.codigo != ELIF && tk.codigo != ENDI){
			if (tk.cat == FIM_ARQ) error("Fim do arquivo inesperado. ");
				tk.processado = true;
				cmd();
				tk = analex(f);
			}
		strcpy(opVerdadeira, geraRotulo());
		escreveCodigoPilha("GOTO %s\n", opVerdadeira);
			while(tk.codigo == ELIF){
				escreveCodigoPilha("LABEL %s\n", opFalsa);
				strcpy(aux.lexema, tk.lexema);
				aux.tipo = BOOL_;
				tk.processado = true;
				tk = analex(f);
				if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Esperado abertura de parenteses\n");
				tk.processado = true;
				Expr();
				if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado\n");
				tk.processado = true;
				tk = analex(f);
				strcpy(opFalsa, geraRotulo());
				escreveCodigoPilha("GOFALSE %s\n", opFalsa);
				while(tk.codigo != ELIF && tk.codigo != ELSE && tk.codigo != ENDI){
					if (tk.cat == FIM_ARQ) error("Fim do arquivo inesperado dentro do loop WHILE");
						tk.processado = true;
						cmd();
						tk = analex(f);
					}
				escreveCodigoPilha("GOTO %s\n", opVerdadeira);
				}
				if(tk.codigo == ELSE){
					escreveCodigoPilha("LABEL %s\n", opFalsa);
					
					tk.processado = true;
					tk = analex(f);
					while(tk.codigo != ENDI){
						if (tk.cat == FIM_ARQ) error("Fim do arquivo inesperado dentro do loop WHILE");
						tk.processado = true;
						cmd();
						tk = analex(f);
					}
					escreveCodigoPilha("LABEL %s\n", opVerdadeira);
				}
				if(tk.codigo != ENDI) error("Esperada finalização de If");
				tk.processado = true;
				tk = analex(f);
	}
	void cmdPuts(){
		if(tk.codigo == PUTINT){
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID && tk.cat != CT_I) error("ID ou constante inteira esperado para funcionamento da função");
			if(tk.cat == ID){
				aux = buscaDecl(tk.lexema);
				escreveCodigoPilha("LOAD %d, %d\n", aux.escopo,aux.endereco);
			}
			else escreveCodigoPilha("PUSH %d", tk.valor);
			tk.processado = true;
			tk = analex(f);
			escreveCodigoPilha("PUT_I\n");
		}
		else if(tk.codigo == PUTREAL){
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID && tk.cat != CT_R) error("ID ou constante real esperado para funcionamento da função");
			if(tk.cat == ID){
				aux = buscaDecl(tk.lexema);
				escreveCodigoPilha("LOAD %d, %d\n", aux.escopo,aux.endereco);
			}
			else escreveCodigoPilha("PUSHF %f\n", tk.valor_r);
			tk.processado = true;
			tk = analex(f);
			escreveCodigoPilha("PUT_F\n");
		}
		
		else if(tk.codigo == PUTCHAR){
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID && tk.cat != CT_C) error("ID ou constante char esperado para funcionamento da função");
			if(tk.cat == ID){
				aux = buscaDecl(tk.lexema);
				escreveCodigoPilha("LOAD %d, %d\n", aux.escopo,aux.endereco);
			}
			else escreveCodigoPilha("PUSH %d\n", tk.c);
			tk.processado = true;
			tk = analex(f);
			escreveCodigoPilha("PUT_C\n");
		}
		else if(tk.codigo == PUTSTR){
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID && tk.cat != LT) error("ID ou constante string esperado para funcionamento da função");
			tk.processado = true;
			tk = analex(f);
		}
	}
	void validarVarInit(){
		if(tokenInfo.tipo == BOOL_ && tk.cat != CT_I) error("Erro semântico, inicialização inválida. Esperado %s", T_tipo[tokenInfo.tipo]);
		else if((tokenInfo.tipo == INT_ || tokenInfo.tipo == CHAR_) && tk.cat == CT_R) error("Erro semântico, inicialização inválida. Esperado %s", T_tipo[tokenInfo.tipo]);
		else if(tokenInfo.tipo == REAL_ && tk.cat != CT_R) error("Erro semântico, inicialização inválida. Esperado %s", T_tipo[tokenInfo.tipo]);
	}
	void atribuirConst(){
		 switch (tokenInfo.tipo) {
        case CHAR_:
            tokenInfo.valConst.char_const = tk.c;
            break;
        case INT_:
            tokenInfo.valConst.int_const = tk.valor;
            break;
        case REAL_:
            tokenInfo.valConst.float_const = tk.valor_r;
            break;
        case BOOL_:
            tokenInfo.valConst.bool_const = tk.valor > 0 ? 1 : 0;
            break;
        default:
            error("Tipo de constante desconhecido.");
            break;
    }
	}

	void declProt(){
		tokenInfo.idcategoria = PROT_;
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != ID) error("Identificador esperado");
			if(strcmp(tk.lexema,"Init") == 0) error("%s não pode ser utilizado para criação de protótipo", tk.lexema);
			if(buscaLexPos(tk.lexema) != -1) error("Protótipo %s definido após sua definição", tk.lexema);
			strcpy(tokenInfo.lexema, tk.lexema);
			inserirNaTabela(tokenInfo); //Inserção do PROC PROT
			resetTokenInfo(&tokenInfo);
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != SN && tk.codigo != PARENTESEABERTO) error("Abertura de parenteses esperado");
			tk.processado = true;
			tk = analex(f);
			if(!(tk.cat == SN && tk.codigo == PARENTESEFECHADO)){
			do{
				if(tk.cat == SN && tk.codigo == VIRGULA){
					tk.processado = true;
					tk = analex(f);
				}
				tokenInfo.zumbi = NA_ZUMBI;
				param();
				int dimensaoArray = 0;
				while(tk.codigo == COLCHETEABERTO){
					dimensaoArray++;
					if(dimensaoArray > 2) error("PROT - Tamanho de array inválido");
					tk.processado = true;
					tk = analex(f);
					if(tk.codigo != COLCHETEFECHADO) error("Fechamento de colchetes esperado");
					tk.processado = true;
					tk = analex(f);
				}
			defineTipoArray(dimensaoArray);
			inserirNaTabela(tokenInfo);
			}while(tk.cat == SN && tk.codigo == VIRGULA);
		}
			if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("Fechamento de parenteses esperado");
			tk.processado = true;
			tk = analex(f);
	}

	void declDef(){
		
		int qtdVariaveis = 0;
		tokenEndereco = 0;
		resetTokenInfo(&tokenInfo);
		tokenInfo.idcategoria = PROC;
			tk.processado = true;
			tk = analex(f);
			if(tk.cat == PV_R && tk.codigo == INIT){
				char initRotulo[20];
				strcpy(tokenInfo.lexema, "Init");
				inserirNaTabela(tokenInfo);
				strcpy(initRotulo, buscaDecl(tokenInfo.lexema).rotulo);
				tk.processado = true;
				tk = analex(f);
				escreveCodigoPilha("LABEL %s\n", initRotulo);
				escreveCodigoPilha("INIPR 1\n");
				while(tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == BOOL || tk.codigo == REAL)){
					tokenInfo.escopo = LOCAL;
					tokenInfo.idcategoria = VAR_LOCAL;
					qtdVariaveis = qtdVariaveis + declListVar();
				}
				if(qtdVariaveis) escreveCodigoPilha("AMEM %d\n", qtdVariaveis);
				while(tk.cat == PV_R || tk.cat == ID){
					if(tk.codigo == ENDP) break;
					tk.processado = true;
					cmd();
					tk = analex(f);
					tk.processado = true;
				}
			if(tk.codigo != ENDP) error("Finalização de procedimento init esperado");
			retirarLocais();
			tk.processado = true;
			tk = analex(f);
			if(qtdVariaveis) escreveCodigoPilha("DMEM %d\n", qtdVariaveis);

			}
			else if(tk.cat == ID){

				char idRotulo[20];
				char lexema[32];
				int pos, qtdParametros = 0;
				strncpy(lexema, tk.lexema, sizeof(lexema) - 1); 
				lexema[sizeof(lexema) - 1] = '\0';
				if((pos = buscaLexPos(tk.lexema)) == -1){
					strcpy(tokenInfo.lexema, tk.lexema);
					inserirNaTabela(tokenInfo);
					strcpy(idRotulo, buscaDecl(tokenInfo.lexema).rotulo);
				}
				else{
					tabela.tokensTab[pos].idcategoria = PROC;
					strcpy(idRotulo, tabela.tokensTab[pos].rotulo);
				}
				
				escreveCodigoPilha("LABEL %s\n", idRotulo);
				escreveCodigoPilha("INIPR 1\n");
				tk.processado = true;
				tk = analex(f);
				if(tk.cat != SN && tk.codigo != PARENTESEABERTO) error("Abertura de parenteses esperado");
				tk.processado = true;
				tk = analex(f);
				if(!(tk.cat == SN && tk.codigo == PARENTESEFECHADO)){
					do{
					if(tk.cat == SN && tk.codigo == VIRGULA){
							tk.processado = true;
							tk = analex(f);
						}
					tokenInfo.zumbi = VIVO;
					param();
					if(tk.cat != ID) error("PROC ID = Identificador Esperado");
					strcpy(tokenInfo.lexema, tk.lexema);
					tk.processado = true;
					tk = analex(f);
					int dimensaoArray = 0;
					while(tk.codigo == COLCHETEABERTO){
						dimensaoArray++;
						if(dimensaoArray > 2) error("PROC ID - Matriz de tamanho inválido");
						tk.processado = true;
						tk = analex(f);
						insereDimensaoArray(dimensaoArray);
						if(strcmp(tk.lexema, tokenInfo.lexema) == 0) error("Parâmetro não pode ser utilizado para tamanho de seu próprio vetor/matriz");
						tk.processado = true;
						tk = analex(f);
						if(tk.codigo != COLCHETEFECHADO) error("Fechamento de colchetes esperado");
						tk.processado = true;
						tk = analex(f);
					}
					defineTipoArray(dimensaoArray);
					
					if(pos == -1) inserirNaTabela(tokenInfo); //Se a pos é -1 quer dizer que proc foi a ultima coisa inserida
					else{
						inserirVazios(pos, tokenInfo);
					}
					qtdParametros++;
					tokenInfo.arrayDim[0] = 0;
					tokenInfo.arrayDim[1] = 0;
					}while(tk.codigo == VIRGULA && tk.cat == SN);
					aplicaEnderecoParam(pos, qtdParametros);
					verificaFaltaParam(buscaLexPos(lexema));
			}
			if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("PROC ID - Fechamento de parenteses esperado");
			tk.processado = true;
				tk = analex(f);
				while(tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == BOOL || tk.codigo == REAL)){
					tokenInfo.idcategoria = VAR_LOCAL;
					tokenInfo.escopo = LOCAL;
					qtdVariaveis = qtdVariaveis + declListVar();
				}
				if(qtdVariaveis) escreveCodigoPilha("AMEM %d\n", qtdVariaveis);
				while(tk.cat == PV_R || tk.cat == ID){
					if(tk.codigo == ENDP) break;
					tk.processado = true;
					cmd();
					tk = analex(f);
					tk.processado = true;
				}

			if(tk.cat != PV_R && tk.codigo != ENDP) error("Finalização de procedimento id esperado");
				tk.processado = true;
				tk = analex(f);
				matarZumbis(buscaLexPos(lexema));
				retirarLocais();
				if(qtdVariaveis) escreveCodigoPilha("DMEM %d\n", qtdVariaveis);
				escreveCodigoPilha("RET 1,%d\n", qtdParametros);

			}
			
			else error("Inicializador ou identificador esperado");
	}
	void prog(){
		int qtdVariaveis = 0;
		while(tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == BOOL || tk.codigo == REAL)){
		tokenInfo.escopo = GLOBAL;
		tokenInfo.idcategoria = VAR_GLOBAL;
			qtdVariaveis = qtdVariaveis + declListVar();
		}
		resetTokenInfo(&tokenInfo);
		if(qtdVariaveis) escreveCodigoPilha("AMEM %d\n", qtdVariaveis);
		while(tk.cat == PV_R && (tk.codigo == PROT || tk.codigo == DEF)){
			declProc();
		}
		if(qtdVariaveis) escreveCodigoPilha("DMEM %d\n", qtdVariaveis);
	}
	void testeSint(char *p) {
		if ((f=fopen(p, "r")) == NULL)
			error("Arquivo de entrada da expressão nao encontrado!\n");
		if((f_out=fopen("proc.obj", "w")) == NULL) error("Erro ao abrir arquivo proc.obj");
		fputs("INIP\n", f_out);
		tk.processado = true;
		tk = analex(f);
		prog();
		for (int i = 0; i < tabela.topo; i++) {
        if (tabela.tokensTab[i].idcategoria == PROT) {
            error("PROT sem definicao encontrado");
			}
		}
			if (tk.cat == FIM_ARQ) {
			printf("\n------------------------------------\n");
			printf("Fim do arquivo %s", p);
			printf("\n------------------------------------\n");
		}
			else error("Finalização de Arquivo Inválida\n");
		fputs("HALT", f_out);
		fclose(f);
		fclose(f_out);
	}