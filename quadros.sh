#!/usr/bin/env bash
# Gera os quadros comparativos de tempo.
#   ./quadros.sh 1   -> quadro da Atividade 1 (similaridade)
#   ./quadros.sh 2   -> quadro da Atividade 2 (distâncias RGB)
# Cada tempo é o MENOR de 5 execuções (para reduzir ruído).

cd "$(dirname "$0")"
export LC_ALL=C
make -s || exit 1

THREADS="1 2 4 8 16"

# Executa o comando 5x e devolve o menor tempo da linha que contém $1
menorTempo() {
    local filtro=$1; shift
    for _ in 1 2 3 4 5; do
        "$@" | grep "$filtro" | sed -E 's/.*Tempo: ([0-9.]+) s.*/\1/'
    done | sort -g | head -1
}

speedup() { awk -v a="$1" -v b="$2" 'BEGIN{ printf "%.2fx", a/b }'; }

quadro1() {
    echo "ATIVIDADE 1 - Similaridade (3 cores mais parecidas) - 10 milhoes de buscas"
    echo "+--------------------+------------+---------+"
    echo "| Versao             |  Tempo (s) | Speedup |"
    echo "+--------------------+------------+---------+"
    L=$(menorTempo "Tempo" ./bin/similaridadeLinear)
    printf "| %-18s | %10.6f | %7s |\n" "Linear" "$L" "1.00x"
    for t in $THREADS; do
        P=$(OMP_NUM_THREADS=$t menorTempo "Tempo" ./bin/similaridadeParalela)
        printf "| %-18s | %10.6f | %7s |\n" "Paralela $t thr." "$P" "$(speedup "$L" "$P")"
    done
    echo "+--------------------+------------+---------+"
}

quadro2() {
    echo "ATIVIDADE 2 - Matriz de distancias RGB 50x50 - 200.000 repeticoes - tempo em segundos"
    echo "+--------------------+--------------------+--------------------+--------------------+"
    echo "| Versao             | A) Por linhas      | B) collapse(2)     | C) Triangular      |"
    echo "+--------------------+--------------------+--------------------+--------------------+"
    L=$(menorTempo "Tempo" ./bin/distanciasLinear)
    printf "| %-18s | %10.6f %7s | %18s | %18s |\n" "Linear" "$L" "1.00x" "" ""
    for t in $THREADS; do
        A=$(OMP_NUM_THREADS=$t menorTempo "A)" ./bin/distanciasParalela)
        B=$(OMP_NUM_THREADS=$t menorTempo "B)" ./bin/distanciasParalela)
        C=$(OMP_NUM_THREADS=$t menorTempo "C)" ./bin/distanciasParalela)
        printf "| %-18s | %10.6f %7s | %10.6f %7s | %10.6f %7s |\n" "Paralela $t thr." \
            "$A" "$(speedup "$L" "$A")" "$B" "$(speedup "$L" "$B")" "$C" "$(speedup "$L" "$C")"
    done
    echo "+--------------------+--------------------+--------------------+--------------------+"
    echo "Speedup = tempo linear / tempo paralelo"
}

case "$1" in
    1) quadro1 ;;
    2) quadro2 ;;
    *) quadro1; echo; quadro2 ;;
esac
