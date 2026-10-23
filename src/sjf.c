#include <stdio.h>

typedef struct
{
    int pid;
    int chegada;
    int burst;
    int espera;
    int retorno;
    int concluido;
} Processo;

typedef struct
{
    Processo itens[100];
    int inicio;
    int fim;
} Fila;

void iniciar(Fila *f)
{
    f->inicio = 0;
    f->fim = 0;
}

void inserir(Fila *f, Processo p)
{
    f->itens[f->fim] = p;
    f->fim++;
}

void sjf(Fila *f)
{
    int tempo = 0;
    int executados = 0;

    printf("\nOrdem de execucao:\n");

    while (executados < f->fim)
    {
        int menor = -1;

        for (int i = f->inicio; i < f->fim; i++)
        {
            if (f->itens[i].concluido == 0 &&
                f->itens[i].chegada <= tempo)
            {

                if (menor == -1 ||
                    f->itens[i].burst < f->itens[menor].burst)
                {
                    menor = i;
                }
            }
        }

        if (menor == -1)
        {
            tempo++;
            continue;
        }

        Processo *p = &f->itens[menor];

        p->espera = tempo - p->chegada;
        tempo += p->burst;
        p->retorno = tempo - p->chegada;

        printf("P%d ", p->pid);

        p->concluido = 1;
        executados++;
    }

    printf("\n\nPID\tEspera\tRetorno\n");

    for (int i = 0; i < f->fim; i++)
    {
        printf("P%d\t%d\t%d\n",
               f->itens[i].pid,
               f->itens[i].espera,
               f->itens[i].retorno);
    }
}

int main()
{
    Fila fila;
    Processo p;
    int n;

    iniciar(&fila);

    printf("Quantidade de processos: ");
    scanf("%d", &n);

    if (n < 1 || n > 100)
    {
        printf("Quantidade invalida!\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        p.pid = i + 1;

        printf("\nChegada do P%d: ", p.pid);
        scanf("%d", &p.chegada);

        printf("Execucao do P%d: ", p.pid);
        scanf("%d", &p.burst);

        if (p.chegada < 0 || p.burst <= 0)
        {
            printf("Valores invalidos!\n");
            return 1;
        }

        p.espera = 0;
        p.retorno = 0;
        p.concluido = 0;

        inserir(&fila, p);
    }

    sjf(&fila);

    return 0;
}