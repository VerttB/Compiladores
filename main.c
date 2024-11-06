#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analex.h"
#define TAM_LEXEMA 50
#define TAM_NUM 20

void	error(char err[])
{
	printf("%s na linha %d", err, linha);
	exit(1);
}
TOKEN	analex(FILE *f)
{
	int		estado;
	int		tamL;
	int		tamD;
	char    lexema[TAM_LEXEMA] = "";
	char	digitos[TAM_NUM] = "";
	TOKEN	tk;
	char	c;
	estado = 0;
	tamL = 0;
	tamD = 0;
	while (true)
	{
		c = fgetc(f);
		switch (estado)
		{
		case 0:
			if (c == ' ' || c == '\t')
				estado = 0;
			else if (c == '_')
			{
				estado = 1;
				lexema[tamL] = c;
				lexema[++tamL] = '\0';
			}
			else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			{
				estado = 2;
				lexema[tamL] = c;
				lexema[++tamL] = '\0';
			}
			else if (c >= '0' && c <= '9')
			{
				estado = 4;
				digitos[tamD] = c;
				digitos[++tamD] = '\0';
			}
			else if (c == '"')
			{
				estado = 9;
			}
			else if (c == '\'')
			{
				estado = 11;
			}
			else if (c == '>')
				estado = 17;
			else if (c == '<')
				estado = 20;
			else if (c == '!')
				estado = 23;
			else if (c == '=')
				estado = 26;
			else if (c == '|')
				estado = 29;
			else if (c == ',')
				estado = 31;
			else if (c == '(')
			{
				estado = 32;
				tk.cat = SN;
				tk.codigo = PARENTESEABERTO;
				return (tk);
			}
			else if (c == ')')
			{
				estado = 33;
				tk.cat = SN;
				tk.codigo = PARENTESEFECHADO;
				return (tk);
			}
			else if (c == '[')
			{
				estado = 34;
				tk.cat = SN;
				tk.codigo = COLCHETEABERTO;
				return (tk);
			}
			else if (c == ']')
			{
				estado = 35;
				tk.cat = SN;
				tk.codigo = COLCHETEFECHADO;
				return (tk);
			}
			else if (c == '&')
				estado = 36;
			else if (c == '+')
			{
				estado = 39;
				tk.cat = SN;
				tk.codigo = ADICAO;
				return (tk);
			}
			else if (c == '-')
			{
				estado = 40;
				tk.cat = SN;
				tk.codigo = SUB;
				return (tk);
			}
			else if (c == '*')
			{
				estado = 41;
				tk.cat = SN;
				tk.codigo = MULT;
				return (tk);
			}
			else if (c == '/')
				estado = 42;
			else if (c == EOF)
			{
				tk.cat = FIM_ARQ;
				return (tk);
			}
			else if (c == '\n')
			{
				linha++;
			}
			else
			{
				error("CARACTER INVALIDO ENCONTRADO");
			}
			break ;
		case 1:
			if (c == '_')
			{
				estado = 1;
				lexema[tamL] = c;
				lexema[++tamL] = '\0';
			}
			else
			{
				estado = 2;
				lexema[tamL] = c;
				lexema[++tamL] = '\0';
			}
			break ;
		case 2:
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
					&& c <= '9') || c == '_')
			{
				estado = 2;
				lexema[tamL] = c;
				lexema[++tamL] = '\0';
			}
			else
			{
				ungetc(c, f);
				estado = 3;
				if (strcmp(lexema, "while") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = WHILE;
				}
				else if (strcmp(lexema, "const") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = CONST;
				}
				else if (strcmp(lexema, "pr") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = PR;
				}
				else if (strcmp(lexema, "init") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = INIT;
				}
				else if (strcmp(lexema, "endp") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = ENDP;
				}
				else if (strcmp(lexema, "char") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = CHAR;
				}
				else if (strcmp(lexema, "int") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = INT;
				}
				else if (strcmp(lexema, "real") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = REAL;
				}
				else if (strcmp(lexema, "bool") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = BOOL;
				}
				else if (strcmp(lexema, "do") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = DO;
				}
				else if (strcmp(lexema, "endw") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = ENDW;
				}
				else if (strcmp(lexema, "if") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = IF;
				}
				else if (strcmp(lexema, "elif") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = ELIF;
				}
				else if (strcmp(lexema, "else") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = ELSE;
				}
				else if (strcmp(lexema, "endi") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = ENDI;
				}
				else if (strcmp(lexema, "getint") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = GETINT;
				}
				else if (strcmp(lexema, "getchar") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = GETCHAR;
				}
				else if (strcmp(lexema, "getreal") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = GETREAL;
				}
				else if (strcmp(lexema, "putint") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = PUTINT;
				}
				else if (strcmp(lexema, "putchar") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = PUTCHAR;
				}
				else if (strcmp(lexema, "putreal") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = PUTREAL;
				}
				else if (strcmp(lexema, "var") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = VAR;
				}
				else if (strcmp(lexema, "from") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = FROM;
				}
				else if (strcmp(lexema, "to") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = TO;
				}
				else if (strcmp(lexema, "by") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = BY;
				}
				else if (strcmp(lexema, "endv") == 0)
				{
					tk.cat = PV_R;
					tk.codigo = ENDV;
				}
				else
				{
					tk.cat = ID;
					strcpy(tk.lexema, lexema);
				}
				return (tk);
			}
			break ;
		case 4:
			if (c >= '0' && c <= '9')
			{
				estado = 4;
				digitos[tamD] = c;
				digitos[++tamD] = '\0';
				if(tamD > TAM_NUM){
					error("Tamanho máximo de inteiro alcançado");
				}
			}
			else if (c == '.')
			{
				estado = 6;
				digitos[tamD] = c;
				digitos[++tamD] = '\0';
			}
			else
			{
				estado = 5;
				ungetc(c, f);
				tk.cat = CT_I;
				tk.valor = atoi(digitos);
				return (tk);
			}
			break ;
		case 6:
			if (c >= '0' && c <= '9')
			{
				estado = 7;
				digitos[tamD] = c;
				digitos[++tamD] = '\0';
				if(tamD > TAM_NUM){
					error("Tamanho máximo de real alcançado");
				}
			}
			else{
				error("Real Inválido");
			}
			break ;
		case 7:
			if (c >= '0' && c <= '9')
			{
				estado = 7;
				digitos[tamD] = c;
				digitos[++tamD] = '\0';
				if(tamD > TAM_NUM){
					error("Tamanho máximo de real alcançado");
				}
			}
			else
			{
				ungetc(c, f);
				tk.cat = CT_R;
				tk.valor_r = atof(digitos);
				return (tk);
			}
			break ;
		case 9:
			if (c == '"')
			{
				estado = 10;
				tk.cat = LT;
				strcpy(tk.lexema, lexema);
				return (tk);
			}
			else
			{
				lexema[tamL] = c;
				lexema[++tamL] = '\0';
				if(tamL > TAM_LEXEMA){
					error("Tamanho de string inválido");
				}
			}
			break ;
		case 11:
			if (c == '\\')
			{
				estado = 12;
			}
			else if(isprint(c))
			{
				tk.c = c;
				estado = 13;
			}
			else{
				error("Erro char");
			}
			break ;
		case 12:
			if (c == '0')
			{
				tk.c = '\0';
				estado = 15;
			}
			else if (c == 'n')
			{
				tk.c = '\n';
				estado = 14;
			}
			break ;
		case 13:
			if (c == '\'')
			{
				estado = 16;
			}
			else
			{
				error("ERRO NO CHAR NA LINHA");
			}
			break ;
		case 14:
			if (c == '\'')
			{
				estado = 46;
			}
			else
			{
				error("ERRO NO CHAR NA LINHA");
			}
			
			break ;
		case 15:
			if (c == '\'')
			{
				estado = 45;
			}
			else
			{
				error("ERRO NO CHAR NA LINHA");
			}
			break ;
		case 16:
			tk.cat = CT_C;
			return (tk);
			break ;
		case 17:
			if (c == '=')
			{
				estado = 19;
				tk.cat = SN;
				tk.codigo = MAIORIGUAl;
				return (tk);
			}
			else
			{
				estado = 18;
				ungetc(c, f);
				tk.cat = SN;
				tk.codigo = MAIORQUE;
				return (tk);
			}
			break ;
		case 20:
			if (c == '=')
			{
				estado = 22;
				tk.cat = SN;
				tk.codigo = MENORIGUAl;
				return (tk);
			}
			else
			{
				ungetc(c, f);
				estado = 21;
				tk.cat = SN;
				tk.codigo = MENORQUE;
				return (tk);
			}
			break ;
		case 23:
			if (c == '=')
			{
				estado = 25;
				tk.cat = SN;
				tk.codigo = DIFERENTE;
				return (tk);
			}
			else
			{
				ungetc(c, f);
				estado = 24;
				tk.cat = SN;
				tk.codigo = NEGACAO;
				return (tk);
			}
			break ;
		case 26:
			if (c == '=')
			{
				estado = 28;
				tk.cat = SN;
				tk.codigo = IGUALDADE;
				return (tk);
			}
			else
			{
				ungetc(c, f);
				estado = 27;
				tk.cat = SN;
				tk.codigo = ATRIBUICAO;
				return (tk);
			}
			break ;
		case 29:
			if (c == '|')
			{
				estado = 30;
				tk.cat = SN;
				tk.codigo = OR;
				return (tk);
			}
			else{
				error("Caracter inválido");
			}
			break ;
		case 36:
			if (c == '&')
			{
				estado = 38;
				tk.cat = SN;
				tk.codigo = AND;
				return (tk);
			}
			else
			{
				ungetc(c, f);
				tk.cat = SN;
				tk.codigo = ENDERECO;
				return (tk);
			}
			break ;
		case 42:
			if (c == '/')
			{
				estado = 44;
			}
			else
			{
				ungetc(c, f);
				estado = 43;
				tk.cat = SN;
				tk.codigo = DIV;
				return (tk);
			}
			break ;
		case 44:
			if (c == '\n')
			{
				linha++;
				estado = 0;
			}
			break ;
		case 45:
			tk.cat = CT_C;
			return tk;
		break;
		case 46:
			tk.cat = CT_C;
			return tk;
		break;
		}
	}
}

int	main(void)
{
	FILE *f;
	TOKEN token;
	if ((f = fopen("text.text", "r")) == NULL)
		exit(1);
	while (true)
	{
		token = analex(f);
		switch (token.cat)
		{
		case SN:
			switch (token.codigo)
			{
			case ADICAO:
				printf("<SN, ADICAO>\n");
				break ;
			case MULT:
				printf("<SN, MULT>\n");
				break ;

			case SUB:
				printf("<SN, SUB>\n");
				break ;

			case DIV:
				printf("<SN, DIV>\n");
				break ;

			case MAIORQUE:
				printf("<SN, MAIORQUE>\n");
				break ;

			case MAIORIGUAl:
				printf("<SN, MAIORIGUAL>\n");
				break ;

			case MENORQUE:
				printf("<SN, MENORQUE>\n");
				break ;

			case MENORIGUAl:
				printf("<SN, MENORIGUAL>\n");
				break ;

			case ATRIBUICAO:
				printf("<SN, ATRIBUICAO>\n");
				break ;

			case IGUALDADE:
				printf("<SN, IGUALDADE>\n");
				break ;

			case PARENTESEABERTO:
				printf("<SN, PARENTESEABERTO>\n");
				break ;

			case PARENTESEFECHADO:
				printf("<SN, PARENTESEFECHADO>\n");
				break ;

			case COLCHETEABERTO:
				printf("<SN, COLCHETEABERTO>\n");
				break ;

			case COLCHETEFECHADO:
				printf("<SN, COLCHETEFECHADO>\n");
				break ;

			case OR:
				printf("<SN, OR>\n");
				break ;

			case ENDERECO:
				printf("<SN, ENDERECO>\n");
				break ;

			case AND:
				printf("<SN, AND>\n");
				break ;

			case NEGACAO:
				printf("<SN, NEGACAO>\n");
				break ;

			case DIFERENTE:
				printf("<SN, DIFERENTE>\n");
				break ;

			case VIR:
				printf("<SN, VIR>\n");
				break ;

			case PONTO:
				printf("<SN, PONTO>\n");
				break ;
			}
			break ;
		case PV_R:
			switch (token.codigo)
			{
			case CONST:
				printf("<PR, CONST>\n");
				break ;

			case PR:
				printf("<PR, PR>\n");
				break ;

			case INIT:
				printf("<PR, INIT>\n");
				break ;

			case ENDP:
				printf("<PR, ENDP>\n");
				break ;

			case CHAR:
				printf("<PR, CHAR>\n");
				break ;

			case INT:
				printf("<PR, INT>\n");
				break ;

			case REAL:
				printf("<PR, REAL>\n");
				break ;

			case BOOL:
				printf("<PR, BOOL>\n");
				break ;

			case DO:
				printf("<PR, DO>\n");
				break ;

			case WHILE:
				printf("<PR, WHILE>\n");
				break ;

			case ENDW:
				printf("<PR, ENDW>\n");
				break ;

			case VAR:
				printf("<PR, VAR>\n");
				break ;

			case FROM:
				printf("<PR, FROM>\n");
				break ;

			case TO:
				printf("<PR, TO>\n");
				break ;

			case BY:
				printf("<PR, BY>\n");
				break ;

			case ENDV:
				printf("<PR, ENDV>\n");
				break ;

			case IF:
				printf("<PR, IF>\n");
				break ;

			case ELIF:
				printf("<PR, ELIF>\n");
				break ;

			case ELSE:
				printf("<PR, ELSE>\n");
				break ;

			case ENDI:
				printf("<PR, ENDI>\n");
				break ;

			case GETINT:
				printf("<PR, GETINT>\n");
				break ;

			case GETCHAR:
				printf("<PR, GETCHAR>\n");
				break ;

			case GETREAL:
				printf("<PR, GETREAL>\n");
				break ;

			case PUTINT:
				printf("<PR, PUTINT>\n");
				break ;

			case PUTCHAR:
				printf("<PR, PUTCHAR>\n");
				break ;

			case PUTREAL:
				printf("<PR, PUTREAL>\n");
				break ;
			}
			break ;
		case ID:
			printf("<ID, %s>\n", token.lexema);
			break ;
		case CT_I:
			printf("<CT_I, %d>\n", token.valor);
			break ;
		case CT_R:
			printf("<CT_R, %f>\n", token.valor_r);
			break ;
		case CT_C:
			printf("<CT_C, %c> \n", token.c);
			break ;
		case LT:
			printf("<LT, %s> \n", token.lexema);
			break ;
		case FIM_ARQ:
			printf("<FIM_ARQ, EOF>");
			break ;
		}
		if (token.cat == FIM_ARQ)
			break ;
	}
	fclose(f);
	return (0);
}