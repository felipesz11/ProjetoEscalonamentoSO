#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{  // Criação do processo
    int pid;
    int tempoxecut;
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
    return f->inicio == f->fim; // Caso o inicio e o fim sejam os mesmos, significa que a fila está vazia
}

void preencherprox (fila *f, processo p){       // Preencher a fila
    f->itens[f->fim] = p;                   
    f->fim++;
}

processo despreencher (fila *f) {         // Retirar o primeiro anterior pra colocar o posterior desse primeiro conteud,o que sera agora o primeiro
    processo p = f->itens[f->inicio];
    f->inicio++;
    return p;
}


void fcfs (fila *f){        // Função do escalonador
    int tempo = 0;
    int n = 0;
    int c = 0;      // Contador dos clocks no sistema no geral

    printf("\n%-4s | %7s | %5s | %6s | %3s | %6s | %5s\n",
       "PID", "Chegada", "Burst", "Inicio", "Fim", "Espera", "Clock");

    while (!vazio(f)){
        processo p = despreencher(f);

        if(tempo < p.tempoxecut)
        tempo = p.tempoxecut;
        

        int inicio = tempo;
        int fim = inicio + p.burst;
        int espera = inicio - p.tempoxecut;  // quanto tempo o processo ficou aguardando desde sua chegada até o início da execução.


        while (c < p.tempoxecut) {
         printf("PID %-4s | Chegada %7s | Burst %5s | Inicio %6s | Fim %3s | Espera %6s | Clock %5d\n", "-", "-", "-", "-", "-", "-", c);

        fflush(stdout);
        sleep(2);
        c++;
}

            while(c <= fim){
            if (c == fim){
                printf("PID %-4d | Chegada %7d | Burst %5d | Inicio %6d | Fim %3d | Espera %6d | Clock %5d\n", p.pid, p.tempoxecut, p.burst, inicio, fim, espera, c);
                printf("\n");
            }else {
                printf("PID %-4d | Chegada %7d | Burst %5d | Inicio %6d | Fim %3s | Espera %6d | Clock %5d\n", p.pid, p.tempoxecut, p.burst, inicio, "-", espera, c);
            }
            fflush(stdout);
            sleep(2);
            c++;
            }

        n++;
        tempo = fim;  // Até clock chegar ao limite da fila
        

    }

}
    

int main (){

    processo v[100];
    int n;

    printf("Quantidade de processos: ");
    scanf ("%d", &n);

    for (int i =0; i < n; i++){
        int numAleatorio = 1000 + (rand() % 9000);      // Define o PID do processo
        v[i].pid = numAleatorio;
        printf("Processo %d - Digite a chegada e burst do processo: ", v[i].pid);
        scanf("%d %d", &v[i].tempoxecut, &v[i].burst);      // Array que insere os dados no processo

    }

    fila f;
    iniciar(&f);
    for (int i = 0; i < n; i++)
        preencherprox(&f, v[i]);  // O uso do array que foi feito para colocar o processo na fila

    fcfs(&f);

    return 0;

}

