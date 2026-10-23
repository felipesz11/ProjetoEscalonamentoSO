#include <stdio.h>

struct Processo
{
    int id;
    int chegada;
    int burst;
    int prioridade;
    int concluido;
    int espera;
    int retorno;
};

struct Fila
{
    struct Processo itens[100];
    int inicio;
    int fim;
};

void iniciar(struct Fila *f)
{
    f->inicio = 0;
    f->fim = 0;
}

void inserir(struct Fila *f, struct Processo p)
{
    f->itens[f->fim] = p;
    f->fim++;
}

void prioridade(struct Fila *f)
{
    int tempo = 0;
    int concluidos = 0;

    printf("\nOrdem de execucao:\n");

    while (concluidos < f->fim)
    {
        int escolhido = -1;

        for (int i = f->inicio; i < f->fim; i++)
        {
            if (f->itens[i].concluido == 0 &&
                f->itens[i].chegada <= tempo)
            {

                if (escolhido == -1 ||
                    f->itens[i].prioridade <
                        f->itens[escolhido].prioridade)
                {
                    escolhido = i;
                }
            }
        }

        if (escolhido == -1)
        {
            tempo++;
            continue;
        }

        struct Processo *p = &f->itens[escolhido];

        p->espera = tempo - p->chegada;
        tempo += p->burst;
        p->retorno = tempo - p->chegada;

        printf("P%d ", p->id);

        p->concluido = 1;
        concluidos++;
    }

    printf("\n\nPID\tEspera\tRetorno\n");

    for (int i = 0; i < f->fim; i++)
    {
        printf("P%d\t%d\t%d\n",
               f->itens[i].id,
               f->itens[i].espera,
               f->itens[i].retorno);
    }
}

int main()
{
    struct Fila fila;
    struct Processo p;
    int n;

    iniciar(&fila);

    printf("Quantidade de processos (1 a 100): ");
    scanf("%d", &n);

    if (n < 1 || n > 100)
    {
        printf("Quantidade invalida!\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        p.id = i + 1;

        printf("\nProcesso P%d\n", p.id);

        printf("Tempo de chegada: ");
        scanf("%d", &p.chegada);

        printf("Tempo de execucao: ");
        scanf("%d", &p.burst);

        printf("Prioridade (1 = maior): ");
        scanf("%d", &p.prioridade);

        if (p.chegada < 0 || p.burst <= 0 ||
            p.prioridade <= 0)
        {
            printf("Valores invalidos!\n");
            return 1;
        }

        p.concluido = 0;
        p.espera = 0;
        p.retorno = 0;

        inserir(&fila, p);
    }

    prioridade(&fila);

    return 0;
}