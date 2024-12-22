
	#include <stdio.h>
	#include <stdbool.h>
	#include <string.h>
	#include "analex.h"
	#include "anasint.h"
	#include "auxfuncs.h"
	#include "tabela_simbolos.h"
	#include "cores.h"

	TokenInfo tokenInfo, aux;

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
		char cmdObj[20];
		if (tk.cat != ID) {
			error("Identificador esperado!\n");
		}
		aux = buscaDecl(tk.lexema);
		if(aux.idcategoria == PROC || aux.idcategoria == PROT) error("Chamada incorreta de procedimento");
		tk.processado = true;
		tk = analex(f);
		while(tk.cat == SN && tk.codigo == COLCHETEABERTO){
			tk.processado = true;
			tk = analex(f);
			Expr();
			if(tk.cat != SN && tk.codigo != COLCHETEFECHADO) error("Fechamento de colchetes esperado");
			tk.processado = true;
			tk = analex(f);
		}
		if (tk.cat != SN || tk.codigo != ATRIBUICAO) {
			error("Sinal de atribuição esperado!\n");
		}
		tk.processado = true;

		Expr();
		snprintf(cmdObj, sizeof(cmdObj), "STOR %d\n", aux.endereco);
		fputs(cmdObj, f_out);

	}

	void Expr() {
		exprSimples();
		while (tk.cat == SN && (tk.codigo == MAIORQUE || tk.codigo == MAIORIGUAL || 
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
		int codigoOp;
		tk = analex(f);
		if (tk.cat == SN && (tk.codigo == ADICAO || tk.codigo == SUB || tk.codigo == OR)) {
			codigoOp = tk.codigo;
			tk.processado = true;
			Termo();
				if(codigoOp == ADICAO) fputs("ADD\n", f_out);
				else if(codigoOp == SUB) fputs("SUB\n", f_out);
			Resto();
		}
	}

	void Sobra() {
		int codigoOp;
		tk = analex(f);
		if(tk.cat == SN && (tk.codigo == MULT || tk.codigo == DIV || tk.codigo == AND)) {
			codigoOp = tk.codigo;
			tk.processado = true;
			Fator();
				if(codigoOp == MULT) fputs("MULT\n", f_out);
				else if(codigoOp == DIV) fputs("DIV\n", f_out);
			Sobra();
		}
	}

	void Fator() {
		char cmdObj[32];
		tk = analex(f);
		if (tk.cat == ID) { 
			TokenInfo TypesAux;
			TypesAux = buscaDecl(tk.lexema);
			if(aux.tipo == REAL_ && TypesAux.tipo != REAL_) error("Erro semântico, atribuição de tipo inválido. Esperado %s, recebido %s", T_tipo[aux.tipo], T_tipo[TypesAux.tipo]);
			snprintf(cmdObj, sizeof(cmdObj), "LOAD %d\n", TypesAux.endereco);
			fputs(cmdObj, f_out);
			
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
				if(aux.tipo == REAL_) error("Erro semântico, atribuição de tipo inválido. Esperado %s ou %s, recebido %s", T_tipo[CHAR_], T_tipo[INT_], T_tipo[aux.tipo]);
				snprintf(cmdObj, sizeof(cmdObj), "PUSH %d\n", tk.valor);
				fputs(cmdObj, f_out);
			}
			else if(tk.cat == CT_R){
				if(aux.tipo != REAL_) error("Erro semântico, atribuição de tipo inválido. Esperado %s, recebido %s", T_tipo[aux.tipo], T_tipo[REAL_]);
				snprintf(cmdObj, sizeof(cmdObj), "PUSHF %f\n", tk.valor_r);
				fputs(cmdObj, f_out);
			}
			else{
				if(aux.tipo == REAL || aux.tipo == BOOL_) error("Erro semântico, atribuição de tipo inválido. Esperado %s, recebido %s", T_tipo[aux.tipo], T_tipo[CHAR_]);
				snprintf(cmdObj, sizeof(cmdObj), "PUSH %d\n", tk.c);
				fputs(cmdObj,f_out);
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
			tk.processado = true;
			tk = analex(f);
			printf("NEGAÇÂO");
			Fator();
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
			
			inserirNaTabela(tokenInfo); //Insere as declarações de variável
		}

		void varInit(){
			if(tk.cat == CT_C || tk.cat == CT_I || tk.cat == CT_R){
					if(tokenInfo.ehConst == CONST_){
						if(tk.cat == CT_C && tokenInfo.tipo == CHAR_) tokenInfo.valConst.char_const = tk.c;
						else if(tk.cat == CT_I && tokenInfo.tipo == INT_) tokenInfo.valConst.int_const = tk.valor;
						else if (tk.cat == CT_R && tokenInfo.tipo == REAL_) tokenInfo.valConst.float_const = tk.valor_r; //Aceita os reais
						else if(tk.cat == CT_I && tokenInfo.tipo == BOOL_) tokenInfo.valConst.bool_const = tk.valor > 0 ? 1 : 0;
						else error("Atribuição de constante inválida.");
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

	void declListVar(){
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
			while(tk.cat == SN && tk.codigo == VIRGULA){
				tk.processado = true;
				tk = analex(f);
				declVar();
			}
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
		char cmdObj[20];
		if(tk.cat != PV_R && tk.cat != ID) error("CMD - Identificador ou palavra chave esperado");
		if(tk.cat == PV_R && (tk.codigo == PROT || tk.codigo == DEF)) error("CMD - Palavra Chave Inválida"); //Inválido se for DEF ou PROT
		if(tk.codigo == DO) cmdDo();
		else if(tk.codigo == WHILE){
			printf("While Iniciado\n");
			snprintf(cmdObj, sizeof(cmdObj), "%s\n", geraRotulo());
			fputs(cmdObj, f_out);
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
		else if(tk.cat == PV_R && (tk.codigo == GETINT || tk.codigo == GETOUT || tk.codigo == GETREAL ||
									tk.codigo == GETCHAR || tk.codigo == GETSTR)){
		trataGets();
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
			else if(tk.cat == PV_R  && (tk.codigo == ENDI || tk.codigo == ENDW || tk.codigo == ENDP 
			|| tk.codigo == ENDV || tk.codigo == ELSE || tk.codigo == ELIF)) error("Finalização de comando inesperada");
		
	}

	void declDefProc(){
		if(tk.cat != PV_R) error("Inicializador de Função esperado");
			tokenInfo.array = NA_ARRAY;
			tokenInfo.escopo = GLOBAL;
			tokenInfo.ehConst = NORMAL;
			tokenInfo.zumbi = NA_ZUMBI;
			tokenInfo.passagem = NA_PASSAGEM;
			tokenInfo.tipo = NA_TIPO;
			tokenInfo.idcategoria = PROC;
			tk.processado = true;
		if(tk.codigo == PROT){
			tokenInfo.idcategoria = PROT_;
			printf("Prot incializado e ");
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
			printFinalizacao("Prot finalizado ");
		} 
		else if(tk.codigo == DEF){
			tokenInfo.idcategoria = PROC;
			tk.processado = true;
			tk = analex(f);
			if(tk.cat == PV_R && tk.codigo == INIT){
				printf("Init inicializado");
				strcpy(tokenInfo.lexema, "Init");
				inserirNaTabela(tokenInfo);
				tk.processado = true;
				tk = analex(f);
				
				while(tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == BOOL || tk.codigo == REAL)){
					tokenInfo.escopo = LOCAL;
					tokenInfo.idcategoria = VAR_LOCAL;
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
			retirarLocais();
			tk.processado = true;
			tk = analex(f);
			printFinalizacao("Init finalizado ");
			}
			else if(tk.cat == ID){
				printf("Def %s inicializado\n", tk.lexema);
				int pos;
				char lexema[32];
				strncpy(lexema, tk.lexema, sizeof(lexema) - 1); //Faz uma cópia do lexema
				lexema[sizeof(lexema) - 1] = '\0';
				if((pos = buscaLexPos(tk.lexema)) == -1){
					strcpy(tokenInfo.lexema, tk.lexema);
					inserirNaTabela(tokenInfo);
				}
				else{
					tabela.tokensTab[pos].idcategoria = PROC;
				}
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
					tokenInfo.arrayDim[0] = 0;
					tokenInfo.arrayDim[1] = 0;
					}while(tk.codigo == VIRGULA && tk.cat == SN);

					verificaFaltaParam(buscaLexPos(lexema));
			}
			if(tk.cat != SN || tk.codigo != PARENTESEFECHADO) error("PROC ID - Fechamento de parenteses esperado");
			tk.processado = true;
				tk = analex(f);
				while(tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == BOOL || tk.codigo == REAL)){
					tokenInfo.idcategoria = VAR_LOCAL;
					declListVar();
				}
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
				printFinalizacao("Finalização de DEF ID");
				matarZumbis(buscaLexPos(lexema));
				retirarLocais();
			}
			
			else error("Inicializador ou identificador esperado");
		}
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
			pos = buscaLexPos(aux.lexema) + 1;
			contaParam(pos-1, &qtdParam);
			if(aux.idcategoria != PROC && aux.idcategoria != PROT_) error("Esperado %s ou %s, recebido %s", T_IdCategoria[PROC], T_IdCategoria[PROT_],T_IdCategoria[aux.tipo]);
			tk.processado = true;
			tk = analex(f);
			if(tk.cat != SN || tk.codigo != PARENTESEABERTO) error("Esperado abertura de parenteses\n");
			tk.processado = true;
			tk = analex(f);
			if(tk.cat == ID || tk.cat == CT_I || tk.cat == CT_R || tk.cat == CT_C){
				if(tabela.tokensTab[pos].idcategoria != PROC_PAR) error("Quantidade inválida de argumentos");
				aux = tabela.tokensTab[pos];
				Expr();
				pos++;
				qtdParam--;
				while(tk.cat == SN && tk.codigo == VIRGULA){
						if(tabela.tokensTab[pos].idcategoria != PROC_PAR) error("Quantidade de argumentos maior que a esperada");
						aux = tabela.tokensTab[pos];
						tk.processado = true;
						tk = analex(f);
						if(!(tk.cat == ID || tk.cat == CT_I || tk.cat == CT_R || tk.cat == CT_C)) error("Identificador, constante inteira, caracter ou real esperado");
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
	
	void trataGets(){
		char cmdObj[20];
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
		snprintf(cmdObj, sizeof(cmdObj), "STOR %d\n", aux.endereco);
		fputs(cmdObj, f_out);
	}

	void prog(){
		
		while(tk.cat == PV_R && (tk.codigo == CONST || tk.codigo == INT || tk.codigo == CHAR || tk.codigo == BOOL || tk.codigo == REAL)){
		tokenInfo.escopo = GLOBAL;
		tokenInfo.idcategoria = VAR_GLOBAL;
			declListVar();
		}
		while(tk.cat == PV_R && (tk.codigo == PROT || tk.codigo == DEF)){
			declDefProc();
		}
	}
	void testeSint(char *p) {
		if ((f=fopen(p, "r")) == NULL)
			error("Arquivo de entrada da expressão nao encontrado!\n");
		if((f_out=fopen("proc.obj", "w")) == NULL) error("Erro ao abrir arquivo proc.obj");
		fputs("INIP\n", f_out);
		tk.processado = true;
		tk = analex(f);
		prog();
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