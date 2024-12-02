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

typedef enum{
    SIMPLES, VETOR, MATRIZ, NA
}ARRAY;

typedef enum{
    CONST, NORMAL
}EHCONST;

typedef struct valconst{
    union 
    {
        int int_const;
        float float_const;
        char char_const;
        bool bool_const;
    };
    
}ValConst;

typedef struct tabelaS{
    char lexema[31];
    ESCOPO escopo;
    TIPO tipo;
    IDCATEGORIA idcategoria;
    PASSAGEM passagem;
    ZUMBI zumbi;
    ARRAY array;
    int DimUm;
    int DimDois;
    EHCONST ehConst;
    ValConst valConst;
    int endereco;
}TabelaS;


typedef struct tabela{
    int topo;
    TabelaS tabelaS[1024];
}Tabela;

extern int topo;
extern Tabela tabela;

void inserirNaTabela(char *, ESCOPO,PASSAGEM, ZUMBI,ARRAY,int, int, EHCONST, valconst, int);
void removerDaTabela();
void printarTabela();