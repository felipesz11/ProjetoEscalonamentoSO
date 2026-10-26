#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct{  
    int pid;
    int chegada;
    int burst;
} processo;


typedef struct{
    processo itens[100]; 
    int inicio; 
    int fim;  
} fila;

void iniciar(fila *f){   
    f->inicio = 0;
    f->fim = 0;
}

int vazio (fila *f){
    return f->inicio == f->fim; 
}

void preencherprox (fila *f, processo p){      
    f->itens[f->fim] = p;                   
    f->fim++;
}

processo despreencher (fila *f) {         
    processo p = f->itens[f->inicio];
    f->inicio++;
    return p;
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


void fcfs (fila *f){        
    int tempo = 0;
    int c = 0;     

    printf("\n%-4s | %7s | %5s | %6s | %3s | %6s | %5s\n",
       "PID", "Chegada", "Burst", "Inicio", "Fim", "Espera", "Clock");

    while (!vazio(f)){
        processo p = despreencher(f);

        if(tempo < p.chegada)
        tempo = p.chegada;
        

        int inicio = tempo;
        int fim = inicio + p.burst;
        int espera = inicio - p.chegada;


        while (c < inicio) {
         printf("PID %-4s | Chegada %7s | Burst %5s | Inicio %6s | Fim %3s | Espera %6s | Clock %5d\n", "-", "-", "-", "-", "-", "-", c);

        fflush(stdout);
        sleep(2);
        c++;
}

            while(c < fim){
            if (c == fim - 1){
                printf("PID %-4d | Chegada %7d | Burst %5d | Inicio %6d | Fim %3d | Espera %6d | Clock %5d\n", p.pid, p.chegada, p.burst, inicio, fim, espera, c);
                printf("\n");
            }else {
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

    printf("Quantidade de processos: ");
    scanf ("%d", &n);

    for (int i =0; i < n; i++){
        int numAleatorio = (1000 + i) + (rand() % 9000);      
        v[i].pid = numAleatorio;
        printf("Processo %d - Digite a chegada e burst do processo: ", v[i].pid);
        scanf("%d %d", &v[i].chegada, &v[i].burst);  

    }

    ordenar_chegada(v, n);

    fila f;
    iniciar(&f);
    for (int i = 0; i < n; i++)
        preencherprox(&f, v[i]);  

    fcfs(&f);

    return 0;

}