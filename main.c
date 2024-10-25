#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "analex.h"
#define TAM_LEXEMA 50
#define TAM_NUM 20
TOKEN analex(FILE *f){
     int estado = 0; 
     int tamL = 0; 
     int tamD = 0; 
     char lexema[TAM_LEXEMA] = ""; 
     char digitos[TAM_NUM] = ""; 
     TOKEN tk;

     while(true){
        char c = fgetc(f);

        switch (estado)
        {
        
        case 0:
            if(c == ' ' || c == '\t') estado = 0;
            else if(c == '_'){
                estado = 1;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                estado = 2;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else if(c >= '0' && c <= '9'){
                estado = 4;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else if(c == '"'){
                estado = 9;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else if(c == '\''){
                estado = 11;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';

            }
            else if(c == '>') estado = 17;
            else if(c == '<') estado = 20;
            else if(c == '!') estado = 23;
            else if(c == '=') estado = 26;
            else if(c == '|') estado = 29;
            else if(c == ',') estado = 31;
            else if(c == '('){
                estado = 32;
                tk.cat = SN;
                tk.codigo = PARENTESEABERTO;
                return tk;
            }
            else if(c == ')'){
                estado = 33;
                tk.cat = SN;
                tk.codigo = PARENTESEFECHADO;
                return tk;
            }
            else if(c == '['){
                estado = 34;
                tk.cat = SN;
                tk.codigo = COLCHETEABERTO;
                return tk;
            }
            else if(c == ']'){
                estado = 35;
                tk.cat = SN;
                tk.codigo = COLCHETEFECHADO;
                return tk;
            }
            else if(c == '&') estado = 36;
            else if(c == '+'){
                estado = 39;
                tk.cat = SN;
                tk.codigo = ADICAO;
                return tk;
            }
            else if(c == '-'){
                estado = 40;
                tk.cat = SN;
                tk.codigo = SUB;
                return tk;
            }
            else if(c == '*'){
                estado = 41;
                tk.cat = SN;
                tk.codigo = MULT;
                return tk;
            }
            else if(c == '/') estado = 42;

            
            break;
        case 1:
            if(c == '_'){
                estado = 1;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else{
                estado = 2;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';    
            }

            break;
        case 2:
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' ){
                estado = 2;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else{
                estado = 3;
                ungetc(c, f);
                tk.cat = ID;
                strcpy(tk.lexema, lexema);
                return tk;
            }
            break;
        case 4:
            if(c >= '0' && c <= '9'){
                estado = 4;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else if(c == '.'){
                estado = 6;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else{
                estado = 5;
                ungetc(c, f);
                tk.cat = CT_I;
                tk.valor = atoi(digitos);
                return tk;
            }
            break;
        case 6:
            if(c >= '0' && c <= '9'){
                estado = 7;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            break;
        case 7:
             if(c >= '0' && c <= '9'){
                estado = 7;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else{
                ungetc(c, f);
                tk.cat = CT_R;
                tk.valor_r = atof(digitos);
                return tk;
            }
        break;
        case 9:
            if(c != '"'){
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else{
                estado = 10;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
                tk.cat = LT;
                strcpy(tk.lexema, lexema);
                return tk;
                
            }
        break;
        case 11:
            if(c == 92){
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
                estado = 12;
            }
            else{
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
                estado = 13;
            }
        break;
        case 12:
            if(c == '0'){
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
                estado = 15;
            }
            else if(c == 'n'){
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
                estado = 14;
            }
        break;
        case 13:
            if(c == '\''){
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
                estado = 16;
            }
        break;
        case 14:
            if(c == '\''){
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
                estado = 16;
            }
        break;
        case 15:
            if(c == '\''){
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
                estado = 16;
            }
        break;
        case 16:
            tk.cat = CT_C;
            return tk;
            break;  
        case 17:
            if(c == '='){
                estado = 19;
                tk.cat = SN;
                tk.codigo = MAIORIGUAl;
                return tk;
            }
            else{
                estado = 18;
                ungetc(c, f);
                tk.cat = SN;
                tk.codigo = MAIORQUE;
                return tk;
            }
            break;
        }
     }
}

int main()
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
                break;

            case MULT:
                printf("<SN, MULT>\n");
                break;

            case SUB:
                printf("<SN, SUB>\n");
                break;

            case DIV:
                printf("<SN, DIV>\n");
                break;

            case MAIORQUE:
                printf("<SN, MAIORQUE>\n");
                break;

            case MAIORIGUAl:
                printf("<SN, MAIORIGUAL>\n");
                break;

            case MENORQUE:
                printf("<SN, MENORQUE>\n");
                break;

            case MENORIGUAl:
                printf("<SN, MENORIGUAL>\n");
                break;

            case ATRIBUICAO:
                printf("<SN, ATRIBUICAO>\n");
                break;

            case IGUALDADE:
                printf("<SN, IGUALDADE>\n");
                break;

            case PARENTESEABERTO:
                printf("<SN, PARENTESEABERTO>\n");
                break;

            case PARENTESEFECHADO:
                printf("<SN, PARENTESEFECHADO>\n");
                break;

            case COLCHETEABERTO:
                printf("<SN, COLCHETEABERTO>\n");
                break;

            case COLCHETEFECHADO:
                printf("<SN, COLCHETEFECHADO>\n");
                break;

            case OR:
                printf("<SN, OR>\n");
                break;

            case ENDERECO:
                printf("<SN, ENDERECO>\n");
                break;

            case AND:
                printf("<SN, AND>\n");
                break;

            case NEGACAO:
                printf("<SN, NEGACAO>\n");
                break;

            case DIFERENTE:
                printf("<SN, DIFERENTE>\n");
                break;

            case VIR:
                printf("<SN, VIR>\n");
                break;

            case PONTO:
                printf("<SN, PONTO>\n");
                break;
            }
        break;
        case PR:
            switch (token.codigo)
            {
            case CONST:
                printf("<PR, CONST>\n");
                break;

            case PVR:
                printf("<PR, PR>\n");
                break;

            case INIT:
                printf("<PR, INIT>\n");
                break;

            case ENDP:
                printf("<PR, ENDP>\n");
                break;

            case CHAR:
                printf("<PR, CHAR>\n");
                break;

            case INT:
                printf("<PR, INT>\n");
                break;

            case REAL:
                printf("<PR, REAL>\n");
                break;

            case BOOL:
                printf("<PR, BOOL>\n");
                break;

            case DO:
                printf("<PR, DO>\n");
                break;

            case WHILE:
                printf("<PR, WHILE>\n");
                break;

            case ENDW:
                printf("<PR, ENDW>\n");
                break;

            case VAR:
                printf("<PR, VAR>\n");
                break;

            case FROM:
                printf("<PR, FROM>\n");
                break;

            case TO:
                printf("<PR, TO>\n");
                break;

            case BY:
                printf("<PR, BY>\n");
                break;

            case ENDV:
                printf("<PR, ENDV>\n");
                break;

            case IF:
                printf("<PR, IF>\n");
                break;

            case ELIF:
                printf("<PR, ELIF>\n");
                break;

            case ELSE:
                printf("<PR, ELSE>\n");
                break;

            case ENDI:
                printf("<PR, ENDI>\n");
                break;

            case GETINT:
                printf("<PR, GETINT>\n");
                break;

            case GETCHAR:
                printf("<PR, GETCHAR>\n");
                break;

            case GETREAL:
                printf("<PR, GETREAL>\n");
                break;

            case PUTINT:
                printf("<PR, PUTINT>\n");
                break;

            case PUTCHAR:
                printf("<PR, PUTCHAR>\n");
                break;

            case PUTREAL:
                printf("<PR, PUTREAL>\n");
                break;
            }
            break;
        case CT_I:
            printf("<CT_I, %d>\n", token.valor);
            break;
        case CT_R:
            printf("<CT_R, %f>\n", token.valor_r);
            break;
        case CT_C:
            printf("<CT_C, %c\n", token.c);
            break;
        case LT:
            printf("<LT, %s", token.lexema);
            break;
        case FIM_ARQ:
            printf("<FIM_ARQ, EOF>");
            break;
        }
        if(token.cat == FIM_ARQ) break;

    }
    fclose(f);
    return 0;
}