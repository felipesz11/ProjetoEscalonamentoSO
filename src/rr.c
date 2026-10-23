#include <stdio.h>

#define MAX 100

int main(void) {
    int quantidade, quantum;
    int restante[MAX];
    int fila[MAX];

    int inicio = 0;
    int fim = 0;
    int na_fila = 0;
    int tempo = 0;

    printf("Quantidade de processos (1 a 100): ");
    if (scanf("%d", &quantidade) != 1 ||
        quantidade < 1 || quantidade > MAX) {
        printf("Quantidade invalida.\n");
        return 1;
    }

    printf("Quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("O quantum deve ser maior que zero.\n");
        return 1;
    }

    for (int i = 0; i < quantidade; i++) {
        printf("Tempo de execucao do P%d: ", i + 1);

        if (scanf("%d", &restante[i]) != 1 || restante[i] <= 0) {
            printf("O tempo deve ser maior que zero.\n");
            return 1;
        }

        /* Coloca o processo no final da fila. */
        fila[fim] = i;
        fim = (fim + 1) % MAX;
        na_fila++;
    }

    printf("\nTodos os processos chegam no instante 0.\n");
    printf("\nOrdem de execucao:\n");

    while (na_fila > 0) {
        /* Retira o primeiro processo da fila. */
        int processo = fila[inicio];
        inicio = (inicio + 1) % MAX;
        na_fila--;

        /* Decide quanto tempo ele pode executar nesta vez. */
        int fatia;

        if (restante[processo] > quantum) {
            fatia = quantum;
        } else {
            fatia = restante[processo];
        }

        int comeco = tempo;

        tempo += fatia;
        restante[processo] -= fatia;

        printf("P%d: de %d ate %d | restante: %d",
               processo + 1, comeco, tempo, restante[processo]);

        if (restante[processo] > 0) {
            /* volta ao final da fila. */
            fila[fim] = processo;
            fim = (fim + 1) % MAX;
            na_fila++;

            printf(" | voltou para a fila\n");
        } else {
            printf(" | terminou\n");
        }
    }

    printf("\nTodos terminaram no tempo %d.\n", tempo);

    return 0;
}