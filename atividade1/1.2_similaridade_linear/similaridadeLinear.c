/*
 * ============================================================
 *  ATIVIDADE 1.2 - Algoritmo de Similaridade Linear
 * ============================================================
 *  Escolhida uma cor (1 a 50), encontra as 3 cores mais parecidas
 *  com ela, ou seja, as 3 menores distâncias na linha da matriz.
 *
 *  Medição de tempo: uma busca sozinha é rápida demais para medir
 *  (~0,0000002 s). Por isso o tempo é medido fazendo a busca para
 *  as 50 cores, 200.000 vezes (10 milhões de buscas).
 *
 *  Executar (na pasta trabalho_final):  ./bin/similaridadeLinear 1
 */
#include "../1.1_carga_matriz/carregaMatriz.h"

#define QTD_SEMELHANTES 3
#define REPETICOES      200000

double matriz[N][N];

/*
 * Procura as 3 cores mais próximas de 'cor' (índice 0 a 49).
 * O resultado fica em semelhantes[], da mais parecida para a menos.
 */
void buscaSemelhantes(int cor, int semelhantes[QTD_SEMELHANTES])
{
    double menores[QTD_SEMELHANTES];
    for (int k = 0; k < QTD_SEMELHANTES; k++) {
        semelhantes[k] = -1;
        menores[k] = 1e9;                  /* "infinito" */
    }

    for (int j = 0; j < N; j++) {
        double d = matriz[cor][j];

        if (j == cor)                      /* ignora a própria cor */
            continue;
        if (d >= menores[QTD_SEMELHANTES - 1])
            continue;                      /* não está entre as 3 melhores */

        /* Insere d na posição certa (lista ordenada) */
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
    int semelhantes[QTD_SEMELHANTES];
    buscaSemelhantes(cor - 1, semelhantes);

    printf("Cores mais parecidas com a cor %d:\n", cor);
    for (int k = 0; k < QTD_SEMELHANTES; k++)
        printf("  %d. cor %2d  (distancia %.2f)\n", k + 1,
               semelhantes[k] + 1, matriz[cor - 1][semelhantes[k]]);

    /* 3) Medição de tempo: 200.000 x 50 buscas */
    long conferencia = 0;   /* soma dos resultados: deve dar igual na versão paralela */

    double inicio = agora();
    for (long r = 0; r < REPETICOES; r++) {
        for (int c = 0; c < N; c++) {
            buscaSemelhantes(c, semelhantes);
            conferencia += semelhantes[0] + semelhantes[1] + semelhantes[2];
        }
    }
    double tempo = agora() - inicio;

    printf("\n%d buscas, conferencia = %ld\n", REPETICOES * N, conferencia);
    printf("Tempo: %.6f s\n", tempo);
    return 0;
}
