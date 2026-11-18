/*
 * ============================================================
 *  ATIVIDADE 2.2 - Cálculo das distâncias com Paralelismo (OpenMP)
 * ============================================================
 *  Mesmo cálculo da 2.1, feito de 3 jeitos com OpenMP para comparar:
 *
 *  Versão A - Por linhas:   #pragma omp for schedule(static)
 *             Cada thread calcula um bloco de linhas da matriz.
 *
 *  Versão B - collapse(2):  #pragma omp for collapse(2) schedule(static)
 *             Os laços i e j viram um só (2500 iterações) e são
 *             divididos entre as threads.
 *
 *  Versão C - Triangular:   #pragma omp for schedule(dynamic, 8)
 *             Como distancia(i,j) = distancia(j,i), calcula só metade
 *             da matriz e copia para a outra metade. As linhas têm
 *             tamanhos diferentes, por isso o escalonamento dinâmico
 *             (as threads pegam blocos de 8 linhas conforme ficam livres).
 *
 *  Em todas:
 *    - #pragma omp parallel fica FORA do laço de repetições, para as
 *      threads serem criadas uma vez só (e não 200.000 vezes);
 *    - reduction(+:soma) junta as somas parciais de cada thread;
 *    - shared(soma): variável compartilhada; dr, dg, db são privadas
 *      (declaradas dentro do laço).
 *
 *  Executar (na pasta trabalho_final):
 *    OMP_NUM_THREADS=8 ./bin/distanciasParalela
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N          50
#define REPETICOES 200000
#define CHUNK      8

int    rgb[N][3];
double dist[N][N];

/* Igual à atividade 2.1 */
int carregaRGB(void)
{
    FILE *arquivo = fopen("dados/rgb.csv", "r");
    if (arquivo == NULL) {
        perror("dados/rgb.csv");
        return -1;
    }

    char linha[256];
    int id, i = 0;
    while (i < N && fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (sscanf(linha, "%d,%d,%d,%d", &id, &rgb[i][0], &rgb[i][1], &rgb[i][2]) == 4)
            i++;
    }
    fclose(arquivo);
    return (i == N) ? 0 : -1;
}

/* ---------------- Versão A: por linhas ---------------- */
double calculaPorLinhas(void)
{
    double soma = 0.0;

    #pragma omp parallel shared(soma)
    for (long r = 0; r < REPETICOES; r++) {

        #pragma omp for schedule(static) reduction(+:soma)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                double dr = rgb[i][0] - rgb[j][0];
                double dg = rgb[i][1] - rgb[j][1];
                double db = rgb[i][2] - rgb[j][2];
                dist[i][j] = sqrt(dr * dr + dg * dg + db * db);
                soma += dist[i][j];
            }
        }
    }
    return soma;
}

/* ---------------- Versão B: collapse(2) ---------------- */
double calculaCollapse(void)
{
    double soma = 0.0;

    #pragma omp parallel shared(soma)
    for (long r = 0; r < REPETICOES; r++) {

        #pragma omp for collapse(2) schedule(static) reduction(+:soma)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                double dr = rgb[i][0] - rgb[j][0];
                double dg = rgb[i][1] - rgb[j][1];
                double db = rgb[i][2] - rgb[j][2];
                dist[i][j] = sqrt(dr * dr + dg * dg + db * db);
                soma += dist[i][j];
            }
        }
    }
    return soma;
}

/* ---------------- Versão C: triangular + dynamic ---------------- */
double calculaTriangular(void)
{
    double soma = 0.0;

    #pragma omp parallel shared(soma)
    for (long r = 0; r < REPETICOES; r++) {

        #pragma omp for schedule(dynamic, CHUNK) reduction(+:soma)
        for (int i = 0; i < N; i++) {
            dist[i][i] = 0.0;
            for (int j = i + 1; j < N; j++) {
                double dr = rgb[i][0] - rgb[j][0];
                double dg = rgb[i][1] - rgb[j][1];
                double db = rgb[i][2] - rgb[j][2];
                double d = sqrt(dr * dr + dg * dg + db * db);
                dist[i][j] = d;
                dist[j][i] = d;                /* copia para a outra metade */
                soma += 2 * d;
            }
        }
    }
    return soma;
}

int main(void)
{
    if (carregaRGB() != 0)
        return 1;

    printf("Matriz %dx%d calculada %d vezes com %d threads\n\n",
           N, N, REPETICOES, omp_get_max_threads());

    double inicio, tempo, soma;

    inicio = omp_get_wtime();
    soma   = calculaPorLinhas();
    tempo  = omp_get_wtime() - inicio;
    printf("A) Por linhas   soma = %.4f   Tempo: %.6f s\n", soma / REPETICOES, tempo);

    inicio = omp_get_wtime();
    soma   = calculaCollapse();
    tempo  = omp_get_wtime() - inicio;
    printf("B) collapse(2)  soma = %.4f   Tempo: %.6f s\n", soma / REPETICOES, tempo);

    inicio = omp_get_wtime();
    soma   = calculaTriangular();
    tempo  = omp_get_wtime() - inicio;
    printf("C) Triangular   soma = %.4f   Tempo: %.6f s\n", soma / REPETICOES, tempo);

    printf("\nDistancia cor 1 -> cor 2 = %.6f\n", dist[0][1]);
    return 0;
}
