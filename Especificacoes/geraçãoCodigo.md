TODO WHILE gera label LX


## While

|            |
|------------|
| WHILE()    |
| LABEL LX   |
| EXPR       |
| GOFALSE LY |
| CMD        |
| GOTO LX    |
| LABEL LY   |


WHILE - PRINTS TEM QUE SER NESSA ORDEM, GUARDAR LX e LY EM VARIAVEL
LABEL LX 
EXP
GOFALSE LY
CMD
GOTO LX
LABEL LY


WHILE( EXPR1 || EXPR2)
EXPR1
COPY
GOTRUE LX
POP
EXPR2
LABEL LX


WHILE( Expr1 && Expr2)
EXPR1
COPY
GOFALSE LY
POP
EXPR2
LABEL LY

WHILE (!Expr1)
EXPR1
GOTRUE LX
LABEL LY
PUSH 0
LABEL LX


WHILE ( EXPR1 && EXPR2 || EXPR3 && !EXPR4)
EXPR1
COPY
GOFALSE L1
POP
EXPR2
LABEL L1
COPY
GOTRUE L2
POP
EXPR3
COPY
GOFALSE L3
POP
EXP4
GOTRUE L4
PUSH 1
GOTO L5
LABEL L4
PUSH 0
LABEL L5
LABEL L3
LABEL L2

## Código de Pilha If,elif, else

|    if()    |  if() else | if() elif() else |
|:----------:|:----------:|:----------------:|
|    expr    |    EXPR    |       EXPR       |
| GOFALSE LX | GOFALSE LX |    GOFALSE LX    |
|     CMD    |     CMD    |        CMD       |
|  LABEL LX  |   GOTO LY  |      GOTO LY     |
|      -     |  LABEL LX  |     LABEL LX     |
|      -     |     CMD    |       EXPR       |
|      -     |  LABEL LY  |    GOFALSE LZ    |
|      -     |      -     |        CMD       |
|      -     |      -     |      GOTO LY     |
|      -     |      -     |     LABEL LZ     |
|      -     |      -     |        CMD       |
|      -     |      -     |     LABEL LY     |
