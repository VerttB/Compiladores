#include "struct/token.h"
#include "lexFunctions.h"
#include <stdio.h>
#include <string.h>

bool isALetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isADigit(char c)
{
    return c >= '0' && c <= '9';
}
bool isStartIdentifier(char c)
{
     return c == '_';
}
bool isIdentifier(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool isAFloat(char c)
{
    return c == '.';
}

bool isAComment(char c){
    return c == '/' || c == '*';
}


bool isKeyWord(char *c){
    const char *keywords[] = {"int", "char", "real", "bool" };
    int size = sizeof(keywords) / sizeof(keywords[0]);
    for(int i =0;i<size;i++){
        if(strcmp(c, keywords[i]) == 0) return true;
    }

    return false;
}

void printTokenCategory(int category)
{
    switch (category)
    {
    case ID:
        printf("Identificador: ID\n");
        break;
    case INT:
        printf("Identificador: INT\n");
        break;
    case CHAR:
        printf("Identificador: CHAR\n");
        break;
    case CONST_INT:
        printf("Identificador: CONST_INT\n");
        break;
    case CONST_CHAR:
        printf("Identificador: CONST_CHAR\n");
        break;
    case CONST_ID:
        printf("Identificador: CONST_ID\n");
        break;
    case INVALID:
        printf("Identificador: INVALID\n");
        break;
    case STRING:
        printf("Identificador: STRING\n");
        break;
    case FLOAT:
        printf("Identificador: FLOAT\n");
        break;
    case COMMENT:
        printf("Identificador: COMMENT\n");
        break;
    case KEYWORD:
        printf("Identificador KEYWORD\n");
        break;
    default:
        printf("Identificador: Desconhecido\n");
    }
}


TOKEN analex(const char *c){
    TOKEN t;
    int estado = 0;
    int tam = strlen(c);
    int i = 0;
    bool isValid = true;
    while(i < tam && isValid){
    switch (estado)
    {
    case 0:
    if(isStartIdentifier(c[i])){
       estado = 1;
    }
    else if(isIdentifier(c[i])){
        estado = 2;
    }
    else if(isADigit(c[i])){
        estado = 3;
    }
    else if(isAComment(c[i])){
        estado = 5;
    }
    else{
        isValid = false;
        t.cat = INVALID;
        break;
    }
        break;
    case 1:
        t.lexema[i] = c[i];
        i++;
        estado = 2;
        break;
    case 2:
        while(i < tam){
            if(isIdentifier(c[i])) t.lexema[i] = c[i];
            else if(isADigit(c[i])) t.lexema[i] = c[i];
            else{
                isValid = false;
                break;
            }
            i++;
        }
        t.cat = ID;
        break;
   
    case 3:
        while(i < tam){
            if(isADigit(c[i])){
                t.lexema[i] = c[i];
                i++;

            }
            else if(isAFloat(c[i])){
                t.lexema[i] = c[i];
                i++;
                if (i < tam && isADigit(c[i])) {
                    estado = 4; 
            } else {
                isValid = false;
                break;
            }
            }
            else{
                isValid = false;
                break;
            }
        }
         t.cat = isValid ? INT : INVALID;
        break;
    case 4:
        while(i < tam){
            if(isADigit(c[i])){ 
                t.lexema[i] = c[i];
                i++;
                }
            else{
                isValid = false;
                break;
            }
         
        }
        t.cat = FLOAT;
        break;
     default:
            isValid = false;
            t.cat = INVALID;
            break;
    }
    }
    t.lexema[i] = '\0';
    return t;

}
int main()
{
    const char *b[] = { "Batata", "_Azul","4321" ,"12.321", "123.32", "@!", "100.", "// testando", "intst", "char"};

    int numStrings = sizeof(b) / sizeof(b[0]);

    for (int i = 0; i < numStrings; i++)
    {
        TOKEN tok = analex(b[i]);
        printf("Cadeia: %s \n", tok.lexema);
        printTokenCategory(tok.cat);
    }

    return 0;
}
