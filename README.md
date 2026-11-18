# Trabalho Final: Programação Paralela

## Organização

```
trabalho_final/
├── dados/                          arquivos do professor (CSV e XLSX)
│
├── atividade1/                     ATIVIDADE 01: cores mais parecidas (usa a matriz pronta)
│   ├── 1.1_carga_matriz/           1.1 carga da matriz
│   ├── 1.2_similaridade_linear/    1.2 busca linear
│   ├── 1.3_similaridade_paralela/  1.3 busca com OpenMP
│   └── 1.4_quadro_comparativo.txt  quadro de tempos
│
├── atividade2/                     ATIVIDADE 02: calcular a matriz a partir do rgb.csv
│   ├── 2.1_distancias_linear/      2.1 cálculo linear
│   ├── 2.2_distancias_paralela/    2.2 cálculo com OpenMP
│   └── 2.3_quadro_comparativo.txt  quadro de tempos
│
├── Makefile                        compila tudo para bin/
└── quadros.sh                      gera os quadros comparativos
```

## O que vai em cada campo do documento

| Campo do documento | Arquivo |
|---|---|
| **1.1** Algoritmo de Carga da Matriz | `atividade1/1.1_carga_matriz/carregaMatriz.h` |
| **1.2** Algoritmo de Similaridade Linear | `atividade1/1.2_similaridade_linear/similaridadeLinear.c` |
| **1.3** Similaridade com Programação Paralela | `atividade1/1.3_similaridade_paralela/similaridadeParalela.c` |
| Print do quadro (Atividade 1) | saída de `./quadros.sh 1` |
| **2.1** Algoritmo Linear | `atividade2/2.1_distancias_linear/distanciasLinear.c` |
| **2.2** Algoritmo com Paralelismo | `atividade2/2.2_distancias_paralela/distanciasParalela.c` |
| **2.3** Quadro comparativo | saída de `./quadros.sh 2` |

## Como rodar (sempre dentro da pasta `trabalho_final`)

```bash
make                    # compila tudo

./quadros.sh 1          # mostra o quadro da Atividade 1 -> tirar print
./quadros.sh 2          # mostra o quadro da Atividade 2 -> tirar print

./bin/similaridadeLinear 7                      # cores parecidas com a cor 7
OMP_NUM_THREADS=8 ./bin/similaridadeParalela 7  # mesma coisa, com 8 threads
```

---

## Atividade 1: as 3 cores mais parecidas

- **1.1** lê o `matrizDistancias.csv` para uma matriz 50x50.
- **1.2** escolhida uma cor, percorre a linha dela na matriz e guarda as 3 menores distâncias. Exemplo: as cores mais parecidas com a **cor 1** são a **6**, a **49** e a **50**.
- **1.3** mesmo algoritmo. As 10 milhões de buscas da medição são divididas entre as threads com `#pragma omp parallel for collapse(2) schedule(static) reduction(+:conferencia)`.

**Resultado:** cerca de **8,5x mais rápido** com 8 threads. As buscas são independentes, então o paralelismo funciona muito bem. Com 16 threads quase não melhora, porque o processador tem 8 núcleos físicos.

## Atividade 2: calcular a matriz de distâncias (OpenMP)

- **2.1** calcula `raiz((R1-R2)² + (G1-G2)² + (B1-B2)²)` para todos os pares de cores. O resultado é igual ao CSV do professor.
- **2.2** faz o mesmo cálculo de 3 jeitos com OpenMP:
  - **A) Por linhas** (`schedule(static)`): cada thread calcula um bloco de linhas.
  - **B) `collapse(2)`**: os dois laços viram um só.
  - **C) Triangular** (`schedule(dynamic, 8)`): calcula só metade da matriz, porque ela é simétrica.

**Resultado:** o melhor foi **A, 3,2x mais rápido** com 8 threads. O ganho é menor que na Atividade 1 porque uma matriz 50x50 é calculada em cerca de 3 microssegundos, e as threads precisam se sincronizar a cada repetição. Esse custo de sincronização pesa muito em um trabalho tão pequeno. Com 16 threads a sincronização fica mais cara ainda e o ganho some.
# Trabalho-Final-de-Programa-o-Paralela
