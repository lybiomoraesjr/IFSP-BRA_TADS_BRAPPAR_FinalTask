#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int processo_atual, quantidade_de_processos, token, processo_anterior, processo_seguinte, tag = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &quantidade_de_processos);
    MPI_Comm_rank(MPI_COMM_WORLD, &processo_atual);

    // Definir os processos anterior e seguinte
    processo_anterior = (processo_atual - 1 + quantidade_de_processos) % quantidade_de_processos;
    processo_seguinte = (processo_atual + 1) % quantidade_de_processos;

    if (processo_atual == 0)
    {
        // Processo 0 inicia o token com o valor 0
        token = 0;
        MPI_Send(&token, 1, MPI_INT, processo_seguinte, tag, MPI_COMM_WORLD);
        printf("Processo %d enviou token %d para processo %d\n", processo_atual, token, processo_seguinte);

        // Receber o token de volta do último processo
        MPI_Recv(&token, 1, MPI_INT, processo_anterior, tag, MPI_COMM_WORLD, &status);
        printf("Processo %d recebeu token %d do processo %d\n", processo_atual, token, processo_anterior);
    }
    else
    {
        // Receber o token do processo anterior
        MPI_Recv(&token, 1, MPI_INT, processo_anterior, tag, MPI_COMM_WORLD, &status);
        printf("Processo %d recebeu token %d do processo %d\n", processo_atual, token, processo_anterior);

        // Incrementar o token
        token++;

        // Enviar o token para o próximo processo
        MPI_Send(&token, 1, MPI_INT, processo_seguinte, tag, MPI_COMM_WORLD);
        printf("Processo %d enviou token %d para processo %d\n", processo_atual, token, processo_seguinte);
    }

    MPI_Finalize();
    return 0;
}
