/*
 * ============================================================
 *  ATIVIDADE 1.1 - Teste da carga da matriz
 * ============================================================
 *  Carrega a matriz, mede o tempo e mostra um pedaço para conferir.
 *
 *  Executar (na pasta trabalho_final):  ./bin/carregaMatriz
 */
#include "carregaMatriz.h"

double matriz[N][N];

int main(void)
{
    double inicio = agora();
    if (carregaMatriz(ARQUIVO_MATRIZ, matriz) != 0)
        return 1;
    double tempo = agora() - inicio;

    printf("Matriz %dx%d carregada em %.6f segundos\n\n", N, N, tempo);

    printf("Canto da matriz (5x5):\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++)
            printf("%9.2f", matriz[i][j]);
        printf("\n");
    }
    return 0;
}
