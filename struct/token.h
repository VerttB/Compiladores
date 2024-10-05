#include "categoria.h"

typedef struct {
    CATEGORIA cat;
    union{
        int codigo;
        int valor;
        char lexema[31];
    };
}TOKEN;