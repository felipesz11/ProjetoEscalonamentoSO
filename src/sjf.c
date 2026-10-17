#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{  // Criação do processo
    int pid;
    int chegada;
    int burst;
} processo;


typedef struct{
    processo itens[100]; // Limite de itens na fila
    int inicio; // posição de quem sai primeiro
    int fim;    // posição onde entra o próximo
} fila;
