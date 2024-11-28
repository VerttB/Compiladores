#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"
#include "anasint.h"


int linha;
FILE *f;
TOKEN tk;

int main(int argc, char *argv[]){
    //testeAnalex();
    linha = 1;
    testeSint(argv[1]);
}