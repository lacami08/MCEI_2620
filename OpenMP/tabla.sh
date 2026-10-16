#!/bin/bash
MAX=$(nproc)
REP=3
OUT=resultados.md

mejor() {  # $1=programa $2=hilos $3=texto de la línea de tiempo
  for i in $(seq $REP); do
    OMP_NUM_THREADS=$2 ./$1 | awk -v pat="$3" '$0 ~ pat {print $(NF-1)}'
  done | sort -g | head -1
}

res() {  # $1=programa $2=hilos $3=texto de la línea de resultado
  OMP_NUM_THREADS=$2 ./$1 | awk -v pat="$3" '$0 ~ pat {print $NF}' | head -1
}

err() { awk -v r=$1 'BEGIN{e=r-2; if(e<0)e=-e; printf "%.2e", e}'; }

{
echo "# Comparación secuencial vs paralelo (OpenMP)"
echo
echo "- n = 10,000,000 subintervalos, f(x) = sin(x), intervalo [0, π]"
echo "- CPU: AMD Ryzen 5 5500U, núcleos disponibles en WSL: $MAX"
echo "- Cada tiempo es el mejor de $REP ejecuciones"
echo
echo "## 1. Tiempos y speedup según el número de hilos"
echo
echo "| Hilos | Simpson sec (s) | Simpson par (s) | Simpson speedup | Trapecio sec (s) | Trapecio par (s) | Trapecio speedup |"
echo "|---|---|---|---|---|---|---|"
} > $OUT

for h in 1 2 4 6 8; do
  [ $h -gt $MAX ] && break
  SS=$(mejor simpson $h "Tiempo secuencial")
  SP=$(mejor simpson $h "Tiempo paralelo")
  TS=$(mejor trapecio $h "Tiempo secuencial")
  TP=$(mejor trapecio $h "Tiempo paralelo")
  SU_S=$(awk -v a=$SS -v b=$SP 'BEGIN{printf "%.2f", a/b}')
  SU_T=$(awk -v a=$TS -v b=$TP 'BEGIN{printf "%.2f", a/b}')
  echo "| $h | $SS | $SP | ${SU_S}x | $TS | $TP | ${SU_T}x |" >> $OUT
done

# Segunda tabla: comparación directa con el máximo de hilos
SS=$(mejor simpson $MAX "Tiempo secuencial");  SP=$(mejor simpson $MAX "Tiempo paralelo")
TS=$(mejor trapecio $MAX "Tiempo secuencial"); TP=$(mejor trapecio $MAX "Tiempo paralelo")
SRS=$(res simpson $MAX "Resultado \\(secuencial\\)");  SRP=$(res simpson $MAX "Resultado \\(paralelo\\)")
TRS=$(res trapecio $MAX "Resultado \\(secuencial\\)"); TRP=$(res trapecio $MAX "Resultado \\(paralelo\\)")
AHS=$(awk -v a=$SS -v b=$SP 'BEGIN{printf "%.1f", (a-b)/a*100}')
AHT=$(awk -v a=$TS -v b=$TP 'BEGIN{printf "%.1f", (a-b)/a*100}')

{
echo
echo "## 2. Secuencial vs paralelo con $MAX hilos"
echo
echo "| Método | Tiempo sec (s) | Tiempo par (s) | Tiempo ahorrado | Resultado sec | Resultado par | Error del paralelo vs 2 |"
echo "|---|---|---|---|---|---|---|"
echo "| Simpson | $SS | $SP | ${AHS}% | $SRS | $SRP | $(err $SRP) |"
echo "| Trapecio | $TS | $TP | ${AHT}% | $TRS | $TRP | $(err $TRP) |"
echo
} >> $OUT

[ -f conclusiones.md ] && cat conclusiones.md >> $OUT

cat $OUT
