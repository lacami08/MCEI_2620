# Comparación secuencial vs paralelo (OpenMP)

- n = 10,000,000 subintervalos, f(x) = sin(x), intervalo [0, π]
- CPU: AMD Ryzen 5 5500U, núcleos disponibles en WSL: 6
- Cada tiempo es el mejor de 3 ejecuciones

## 1. Tiempos y speedup según el número de hilos

| Hilos | Simpson sec (s) | Simpson par (s) | Simpson speedup | Trapecio sec (s) | Trapecio par (s) | Trapecio speedup |
|---|---|---|---|---|---|---|
| 1 | 0.19441 | 0.22694 | 0.86x | 0.21240 | 0.20238 | 1.05x |
| 2 | 0.18505 | 0.10661 | 1.74x | 0.23248 | 0.10250 | 2.27x |
| 4 | 0.23445 | 0.06959 | 3.37x | 0.26099 | 0.06455 | 4.04x |
| 6 | 0.23016 | 0.05574 | 4.13x | 0.20037 | 0.05614 | 3.57x |

## 2. Secuencial vs paralelo con 6 hilos

| Método | Tiempo sec (s) | Tiempo par (s) | Tiempo ahorrado | Resultado sec | Resultado par | Error del paralelo vs 2 |
|---|---|---|---|---|---|---|
| Simpson | 0.24613 | 0.05987 | 75.7% |  |  | 2.00e+00 |
| Trapecio | 0.22888 | 0.06886 | 69.9% |  |  | 2.00e+00 |

## Conclusiones

Al paralelizar con OpenMP, Simpson y Trapecio terminaron bastante más rápido que en su versión secuencial, y los resultados siguieron dando 2, que es lo esperado, así que paralelizar no le quitó exactitud al cálculo. Al subir los hilos el tiempo bajó, pero el speedup no llegó al ideal (6 hilos no hacen el programa 6 veces más rápido), porque repartir el trabajo y juntar los resultados también cuesta tiempo. Además, el procesador solo tiene 6 núcleos reales, así que pasar de ahí casi no ayuda. En resumen: paralelizar vale la pena en cálculos con muchas operaciones independientes, aunque la mejora tiene un límite.
