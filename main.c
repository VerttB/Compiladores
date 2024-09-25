#include "structs.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool isALetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isADigit(char c)
{
    return c >= '0' && c <= '9';
}
// bool isStartIdentifier(char c)
// {
//     return c == '_';
// }
bool isIdentifier(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
bool isAFloat(char c)
{
    return c == '.';
}

const char *keywords[] = {"int", "char", "real", "bool" };

TOKEN isValid(const char *c)
{
    int tam = strlen(c);
    TOKEN t;
    int i = 0;

    if (isIdentifier(c[i]))
    {
        t.cat = ID;
        t.lexema[0] = c[i];
        i++;
        while (i < tam && isIdentifier(c[i]))
        {
            t.lexema[i] = c[i];
            i++;
        }
        
    }

    else if (isADigit(c[i]))
    {
        t.cat = INT;
        t.lexema[0] = c[i];
        i++;
        while (i < tam && isADigit(c[i]))
        {
            t.lexema[i] = c[i];
            i++;
        }
        if (i < tam)
        {
            if (isAFloat(c[i]))
            {
                t.cat = FLOAT;
                t.lexema[i] = c[i];
                i++;
            if(isADigit(c[i + 1])){
                while (i < tam && isADigit(c[i]))
                {
                    t.lexema[i] = c[i];
                    i++;
                }
            }
            else
            {
                t.cat = INVALID;
            }
            }
        }
    }
    else
    {
        t.cat = INVALID;
    }

    t.lexema[i+1] = '\0';

    return t;
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
    default:
        printf("Identificador: Desconhecido\n");
    }
}

int main()
{
    const char *b[] = {"Laranja", "Batata", "_Azul", "12321", "123.32", "@!", "100."};

    int numStrings = sizeof(b) / sizeof(b[0]);

    for (int i = 0; i < numStrings; i++)
    {
        TOKEN tok = isValid(b[i]);
        printf("Cadeia: %s ", tok.lexema);
        printTokenCategory(tok.cat);
    }

    return 0;
}
