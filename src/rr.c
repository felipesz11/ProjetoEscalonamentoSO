#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX 100

typedef struct{  // Criação do processo
    int pid;
    int chegada;
    int burst;
    int restante;   // quanto ainda falta executar
} processo;

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

int main(void) {
    processo v[MAX];
    int n, quantum;

    int fila[MAX];          // fila circular com as posições dos processos em v[]
    int inicio = 0;
    int fim = 0;
    int na_fila = 0;

    int tempo = 0;          // tempo atual do sistema
    int c = 1;              // contador dos clocks
    int prox = 0;           // próximo processo (em v[]) que ainda vai chegar
    int concluidos = 0;

    srand(time(NULL));

    printf("Quantidade de processos (1 a %d): ", MAX);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Quantidade invalida.\n");
        return 1;
    }

    printf("Quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("O quantum deve ser maior que zero.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        v[i].pid = (1000 + i) + (rand() % 9000);    // Define o PID do processo
        printf("Processo %d - Digite a chegada e burst do processo: ", v[i].pid);

        if (scanf("%d %d", &v[i].chegada, &v[i].burst) != 2 ||
            v[i].chegada < 0 || v[i].burst <= 0) {
            printf("Valores invalidos.\n");
            return 1;
        }
        v[i].restante = v[i].burst;
    }

    ordenar_chegada(v, n);

    printf("\n%-4s | %7s | %5s | %8s | %5s | %5s\n",
           "PID", "Chegada", "Burst", "Restante", "Fatia", "Clock");

    while (concluidos < n) {

        /* Coloca na fila quem já chegou. */
        while (prox < n && v[prox].chegada <= tempo) {
            fila[fim] = prox;
            fim = (fim + 1) % MAX;
            na_fila++;
            prox++;
        }

        /* Ninguém na fila: CPU ociosa por 1 clock. */
        if (na_fila == 0) {
            printf("PID %-4s | Chegada %7s | Burst %5s | Restante %8s | Fatia %5s | Clock %5d\n",
                   "-", "-", "-", "-", "-", c);
            fflush(stdout);
            sleep(2);
            c++;
            tempo++;
            continue;
        }

        /* Retira o primeiro processo da fila. */
        int idx = fila[inicio];
        inicio = (inicio + 1) % MAX;
        na_fila--;

        /* Decide quanto tempo ele pode executar nesta vez. */
        int fatia = (v[idx].restante > quantum) ? quantum : v[idx].restante;

        /* Executa a fatia, um clock por vez. */
        for (int k = 1; k <= fatia; k++) {
            v[idx].restante--;
            tempo++;

            printf("PID %-4d | Chegada %7d | Burst %5d | Restante %8d | Fatia %2d/%-2d | Clock %5d",
                   v[idx].pid, v[idx].chegada, v[idx].burst,
                   v[idx].restante, k, fatia, c);

            if (v[idx].restante == 0) {
                int retorno = tempo - v[idx].chegada;
                int espera = retorno - v[idx].burst;
                printf(" | Terminou (Fim %d, Espera %d)\n\n", tempo, espera);
            } else if (k == fatia) {
                printf(" | Quantum esgotado, volta p/ fila\n\n");
            } else {
                printf("\n");
            }

            fflush(stdout);
            sleep(2);
            c++;
        }

        /* Quem chegou durante a fatia entra na fila ANTES de quem foi interrompido. */
        while (prox < n && v[prox].chegada <= tempo) {
            fila[fim] = prox;
            fim = (fim + 1) % MAX;
            na_fila++;
            prox++;
        }

        if (v[idx].restante > 0) {
            fila[fim] = idx;
            fim = (fim + 1) % MAX;
            na_fila++;
        } else {
            concluidos++;
        }
    }

    printf("Todos terminaram no tempo %d.\n", tempo);

    return 0;
}