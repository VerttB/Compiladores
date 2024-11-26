typedef enum{
    GLOBAL, LOCAL
}ESCOPO;
typedef enum{
    INT, REAL, CHAR, BOOL, NA
}TIPO;

typedef enum{
    VAR_GLOBAL, VAR_LOCAL, PROC, PROC_PAR
}IDCATEGORIA;

typedef enum{
    COPIA, REFERENCIA, NA
}PASSAGEM;

typedef enum{
    VIVO, ZUMBI, NA
}ZUMBI;