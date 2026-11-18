/*
 * ============================================================
 *  ATIVIDADE 2.1 - Cálculo das distâncias euclidianas (Linear)
 * ============================================================
 *  Lê as 50 cores de dados/rgb.csv e monta a matriz 50x50:
 *
 *      distancia(i,j) = raiz( (Ri-Rj)² + (Gi-Gj)² + (Bi-Bj)² )
 *
 *  O cálculo da matriz é repetido 200.000 vezes para dar um tempo
 *  mensurável. No fim, confere o resultado com o matrizDistancias.csv
 *  fornecido pelo professor.
 *
 *  Executar (na pasta trabalho_final):  ./bin/distanciasLinear
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define N          50
#define REPETICOES 200000

int    rgb[N][3];         /* rgb[i][0]=R, rgb[i][1]=G, rgb[i][2]=B */
double dist[N][N];        /* matriz de distâncias calculada        */

double agora(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

/* Lê dados/rgb.csv (cabeçalho "id,R,G,B" + 50 linhas) */
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
        /* o cabeçalho não tem números, então o sscanf falha e é ignorado */
        if (sscanf(linha, "%d,%d,%d,%d", &id, &rgb[i][0], &rgb[i][1], &rgb[i][2]) == 4)
            i++;
    }
    fclose(arquivo);
    return (i == N) ? 0 : -1;
}

/* Calcula a matriz inteira. Retorna a soma das distâncias (para conferência). */
double calculaDistancias(void)
{
    double soma = 0.0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double dr = rgb[i][0] - rgb[j][0];
            double dg = rgb[i][1] - rgb[j][1];
            double db = rgb[i][2] - rgb[j][2];
            dist[i][j] = sqrt(dr * dr + dg * dg + db * db);
            soma += dist[i][j];
        }
    }
    return soma;
}

/* Compara com o matrizDistancias.csv do professor. Retorna a maior diferença. */
double comparaComProfessor(void)
{
    FILE *arquivo = fopen("dados/matrizDistancias.csv", "r");
    if (arquivo == NULL)
        return -1;

    char linha[8192];
    double maiorDiferenca = 0.0;

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {    /* cabeçalho */
        fclose(arquivo);
        return -1;
    }
    for (int i = 0; i < N && fgets(linha, sizeof(linha), arquivo); i++) {
        char *valor = strtok(linha, ",\r\n");
        for (int j = 0; j < N && valor; j++) {
            double diferenca = fabs(atof(valor) - dist[i][j]);
            if (diferenca > maiorDiferenca)
                maiorDiferenca = diferenca;
            valor = strtok(NULL, ",\r\n");
        }
    }
    fclose(arquivo);
    return maiorDiferenca;
}

int main(void)
{
    if (carregaRGB() != 0)
        return 1;

    double soma = 0.0;

    double inicio = agora();
    for (long r = 0; r < REPETICOES; r++)
        soma += calculaDistancias();
    double tempo = agora() - inicio;

    printf("Matriz %dx%d calculada %d vezes\n", N, N, REPETICOES);
    printf("Distancia cor 1 -> cor 2 = %.6f\n", dist[0][1]);
    printf("Soma das distancias      = %.4f\n", soma / REPETICOES);
    printf("Maior diferenca para o CSV do professor = %.1e\n", comparaComProfessor());
    printf("Tempo: %.6f s\n", tempo);
    return 0;
}
