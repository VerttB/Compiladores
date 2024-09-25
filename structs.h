typedef enum {
    ID,INT,CHAR,CONST_INT, CONST_CHAR, CONST_ID,INVALID, STRING,FLOAT
}CATEGORIA;

typedef struct {
    CATEGORIA cat;
    union{
        int codigo;
        int valor;
        char lexema[31];
    };
}TOKEN;