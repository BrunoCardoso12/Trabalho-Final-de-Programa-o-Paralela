# Compila tudo para a pasta bin/   ->   make
# Gera os quadros comparativos     ->   ./quadros.sh 1   e   ./quadros.sh 2

CC     = gcc
CFLAGS = -O2 -Wall
A1     = atividade1
A2     = atividade2

all: bin bin/carregaMatriz bin/similaridadeLinear bin/similaridadeParalela \
     bin/distanciasLinear bin/distanciasParalela

bin:
	mkdir -p bin

# ---------- Atividade 1 ----------
bin/carregaMatriz: $(A1)/1.1_carga_matriz/carregaMatriz.c $(A1)/1.1_carga_matriz/carregaMatriz.h
	$(CC) $(CFLAGS) -o $@ $<

bin/similaridadeLinear: $(A1)/1.2_similaridade_linear/similaridadeLinear.c $(A1)/1.1_carga_matriz/carregaMatriz.h
	$(CC) $(CFLAGS) -o $@ $<

bin/similaridadeParalela: $(A1)/1.3_similaridade_paralela/similaridadeParalela.c $(A1)/1.1_carga_matriz/carregaMatriz.h
	$(CC) $(CFLAGS) -fopenmp -o $@ $<

# ---------- Atividade 2 ----------
bin/distanciasLinear: $(A2)/2.1_distancias_linear/distanciasLinear.c
	$(CC) $(CFLAGS) -o $@ $< -lm

bin/distanciasParalela: $(A2)/2.2_distancias_paralela/distanciasParalela.c
	$(CC) $(CFLAGS) -fopenmp -o $@ $< -lm

clean:
	rm -rf bin

.PHONY: all clean
