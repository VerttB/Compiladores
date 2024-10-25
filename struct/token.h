#include "categoria.h"

typedef struct {
    CATEGORIA cat;
    union{
        int codigo;
        int valor;
        float valor_r;
        char c;
        char lexema[31];
    };
}TOKEN;