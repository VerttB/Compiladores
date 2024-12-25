# Gerações de código

## While

|                |
|----------------|
| WHILE(EXPR)    |
| LABEL LX       |
| EXPR           |
| GOFALSE LY     |
| CMD            |
| GOTO LX        |
| LABEL LY       |

## AND , OR, NOT

| EXPR1 \|\| EXPR2 | EXPR1 && EXPR2 |   !EXPR1  |
|:----------------:|:--------------:|:---------:|
|       EXPR1      |      EXPR1     |   EXPR1   |
|       COPY       |      COPY      | GOTRUE LX |
|     GOTRUE LX    |   GOFALSE LY   |   PUSH 1  |
|        POP       |       POP      |  GOTO LY  |
|       EXPR2      |      EXPR2     |  LABEL LX |
|     LABEL LX     |    LABEL LY    |   PUSH 0  |
|         -        |        -       |  LABEL LY 

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


## Código var to e var dt

| var id expr1 to expr2 | var id expr1 dt expr2 |
|:---------------------:|:---------------------:|
|         EXPR1         |         EXPR1         |
|         STOR A        |         STOR A        |
|         EXPR 2        |         EXPR2         |
|        LABEL LX       |        LABEL LX       |
|          COPY         |          COPY         |
|         LOAD A        |         LOAD A        |
|          SUB          |          SUB          |
|       GOTRUE LY       |       GOFALSE LY      |
|          POP          |          CMD          |
|        GOTO LZ        |         LOAD A        |
|        LABEL LY       |         PUSH 1        |
|          CMD          |          SUB          |
|         LOAD A        |         STOR A        |
|         PUSH 1        |        GOTO LX        |
|          ADD          |        LABEL LY       |
|         STOR A        |          POP          |
|        GOTO LX        |           -           |
|        LABEL LZ       |           -           |
|          POP          |           -           |

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
