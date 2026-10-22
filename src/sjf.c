#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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

void iniciar(fila *f){   // Iniciar a fila
    f->inicio = 0;
    f->fim = 0;
}

int vazio (fila *f){
    return f->inicio == f->fim;
}

void preencherprox (fila *f, processo p){   // Preencher a fila
    f->itens[f->fim] = p;
    f->fim++;
}

void ordenar_chegada(processo v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (v[j].chegada > v[j + 1].chegada) {
                processo aux = v[j];
                v[j] = v[j + 1];
                v[j + 1] = aux;
            }
        }
    }
}

// Procura, entre os processos que já chegaram, o de menor burst.
// Retorna a posição dele na fila, ou -1 se ninguém chegou ainda.
int escolher_menor(fila *f, int tempo){
    int menor = -1;

    for (int i = f->inicio; i < f->fim; i++){
        if (f->itens[i].chegada <= tempo){
            if (menor == -1 || f->itens[i].burst < f->itens[menor].burst)
                menor = i;
        }
    }
    return menor;
}

// Retira da fila o processo da posição idx e fecha o buraco
processo retirar(fila *f, int idx){
    processo p = f->itens[idx];

    for (int i = idx; i < f->fim - 1; i++)
        f->itens[i] = f->itens[i + 1];

    f->fim--;
    return p;
}

void sjf (fila *f){         // Função do escalonador
    int tempo = 0;
    int c = 1;      // Contador dos clocks no sistema no geral

    printf("\n%-4s | %7s | %5s | %6s | %3s | %6s | %5s\n",
       "PID", "Chegada", "Burst", "Inicio", "Fim", "Espera", "Clock");

    while (!vazio(f)){
        int idx = escolher_menor(f, tempo);

        if (idx == -1){     // Ninguém chegou ainda: CPU ociosa até o próximo chegar
            tempo = f->itens[f->inicio].chegada;   // fila está ordenada por chegada
            idx = escolher_menor(f, tempo);
        }

        processo p = retirar(f, idx);

        int inicio = tempo;
        int fim = inicio + p.burst;
        int espera = inicio - p.chegada;

        while (c < inicio) {
            printf("PID %-4s | Chegada %7s | Burst %5s | Inicio %6s | Fim %3s | Espera %6s | Clock %5d\n", "-", "-", "-", "-", "-", "-", c);
            fflush(stdout);
            sleep(2);
            c++;
        }

        while (c <= fim){
            if (c == fim){
                printf("PID %-4d | Chegada %7d | Burst %5d | Inicio %6d | Fim %3d | Espera %6d | Clock %5d\n", p.pid, p.chegada, p.burst, inicio, fim, espera, c);
                printf("\n");
            } else {
                printf("PID %-4d | Chegada %7d | Burst %5d | Inicio %6d | Fim %3s | Espera %6d | Clock %5d\n", p.pid, p.chegada, p.burst, inicio, "-", espera, c);
            }
            fflush(stdout);
            sleep(2);
            c++;
        }

        tempo = fim;
    }
}

int main (){

    processo v[100];
    int n;

    srand(time(NULL));   // PIDs diferentes a cada execução

    printf("Quantidade de processos: ");
    scanf ("%d", &n);

    if (n < 1 || n > 100){
        printf("Quantidade invalida!\n");
        return 1;
    }

    for (int i = 0; i < n; i++){
        int numAleatorio = (1000 + i) + (rand() % 9000);      // Define o PID do processo
        v[i].pid = numAleatorio;
        printf("Processo %d - Digite a chegada e burst do processo: ", v[i].pid);
        scanf("%d %d", &v[i].chegada, &v[i].burst);
    }

    ordenar_chegada(v, n);

    fila f;
    iniciar(&f);
    for (int i = 0; i < n; i++)
        preencherprox(&f, v[i]);

    sjf(&f);

    return 0;
}