/*
 * ============================================================
 *  ATIVIDADE 1.1 - Algoritmo de Carga da Matriz
 * ============================================================
 *  Lê o arquivo dados/matrizDistancias.csv para a memória.
 *
 *  Formato do arquivo:
 *    1ª linha  -> cabeçalho 1,2,...,50 (é descartado)
 *    demais    -> 50 linhas com 50 distâncias separadas por vírgula
 *
 *  Este arquivo é usado pelas atividades 1.2 e 1.3.
 */
#ifndef CARREGA_MATRIZ_H
#define CARREGA_MATRIZ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 50                                   /* quantidade de cores */
#define ARQUIVO_MATRIZ "dados/matrizDistancias.csv"

/* Retorna o horário atual em segundos (usado para medir o tempo) */
static inline double agora(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

/* Lê o CSV e preenche a matriz. Retorna 0 se deu certo, -1 se deu erro. */
static int carregaMatriz(const char *nomeArquivo, double matriz[N][N])
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror(nomeArquivo);
        return -1;
    }

    char linha[8192];

    /* Descarta o cabeçalho */
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return -1;
    }

    /* Lê as 50 linhas */
    for (int i = 0; i < N; i++) {
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fprintf(stderr, "Arquivo terminou na linha %d (esperado %d).\n", i, N);
            fclose(arquivo);
            return -1;
        }

        /* Separa os 50 valores da linha pela vírgula */
        char *valor = strtok(linha, ",\r\n");
        for (int j = 0; j < N; j++) {
            if (valor == NULL) {
                fprintf(stderr, "Linha %d tem menos de %d valores.\n", i + 1, N);
                fclose(arquivo);
                return -1;
            }
            matriz[i][j] = atof(valor);
            valor = strtok(NULL, ",\r\n");
        }
    }

    fclose(arquivo);
    return 0;
}

#endif
