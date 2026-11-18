/*
 * ============================================================
 *  ATIVIDADE 1.3 - Similaridade com Programação Paralela (OpenMP)
 * ============================================================
 *  Mesmo algoritmo da 1.2. A diferença está na medição de tempo:
 *  as 10 milhões de buscas são independentes entre si, então são
 *  divididas entre as threads com OpenMP.
 *
 *  Recursos de OpenMP usados (ver o #pragma no main):
 *    parallel for       -> divide as iterações entre as threads
 *    collapse(2)        -> junta os dois laços (repetições x cores) em um só
 *    schedule(static)   -> divisão em partes iguais (toda busca custa igual)
 *    reduction(+:...)   -> cada thread soma a sua parte e no fim junta tudo
 *    private            -> 'semelhantes' é declarado dentro do laço,
 *                          então cada thread tem a sua cópia
 *
 *  Executar (na pasta trabalho_final):
 *    OMP_NUM_THREADS=8 ./bin/similaridadeParalela 1
 */
#include <omp.h>
#include "../1.1_carga_matriz/carregaMatriz.h"

#define QTD_SEMELHANTES 3
#define REPETICOES      200000

double matriz[N][N];

/* Igual à atividade 1.2 */
void buscaSemelhantes(int cor, int semelhantes[QTD_SEMELHANTES])
{
    double menores[QTD_SEMELHANTES];
    for (int k = 0; k < QTD_SEMELHANTES; k++) {
        semelhantes[k] = -1;
        menores[k] = 1e9;
    }

    for (int j = 0; j < N; j++) {
        double d = matriz[cor][j];

        if (j == cor)
            continue;
        if (d >= menores[QTD_SEMELHANTES - 1])
            continue;

        int p = QTD_SEMELHANTES - 1;
        while (p > 0 && menores[p - 1] > d) {
            menores[p]     = menores[p - 1];
            semelhantes[p] = semelhantes[p - 1];
            p--;
        }
        menores[p]     = d;
        semelhantes[p] = j;
    }
}

int main(int argc, char *argv[])
{
    int cor = (argc > 1) ? atoi(argv[1]) : 1;
    if (cor < 1 || cor > N) {
        printf("Escolha uma cor entre 1 e %d.\n", N);
        return 1;
    }

    /* 1) Carrega a matriz (atividade 1.1) */
    if (carregaMatriz(ARQUIVO_MATRIZ, matriz) != 0)
        return 1;

    /* 2) Resultado para a cor escolhida */
    int resultado[QTD_SEMELHANTES];
    buscaSemelhantes(cor - 1, resultado);

    printf("Cores mais parecidas com a cor %d:\n", cor);
    for (int k = 0; k < QTD_SEMELHANTES; k++)
        printf("  %d. cor %2d  (distancia %.2f)\n", k + 1,
               resultado[k] + 1, matriz[cor - 1][resultado[k]]);

    /* 3) Medição de tempo: 200.000 x 50 buscas, divididas entre as threads */
    long conferencia = 0;

    double inicio = agora();

    #pragma omp parallel for collapse(2) schedule(static) reduction(+:conferencia)
    for (long r = 0; r < REPETICOES; r++) {
        for (int c = 0; c < N; c++) {
            int semelhantes[QTD_SEMELHANTES];      /* privado de cada thread */
            buscaSemelhantes(c, semelhantes);
            conferencia += semelhantes[0] + semelhantes[1] + semelhantes[2];
        }
    }

    double tempo = agora() - inicio;

    printf("\n%d buscas com %d threads, conferencia = %ld\n",
           REPETICOES * N, omp_get_max_threads(), conferencia);
    printf("Tempo: %.6f s\n", tempo);
    return 0;
}
