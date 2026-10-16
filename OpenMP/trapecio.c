#include <stdio.h>
#include <math.h>
#include <omp.h>

// Función a integrar
double f(double x) {
    return sin(x); 
}

// 1. Función Secuencial (Sin paralelizar)
double trapecio_secuencial(double a, double b, int n) {
    double h = (b - a) / n; 
    double sum = 0.5 * (f(a) + f(b)); 
    
    for (int i = 1; i < n; i++) {
        double x = a + i * h; 
        sum += f(x); 
    }
    return h * sum; 
}

// 2. Función Paralelizada (Extraída del documento)
double trapecio_parallel(double a, double b, int n) {
    double h = (b - a) / n; 
    double sum = 0.5 * (f(a) + f(b)); 
    
    #pragma omp parallel for reduction(+:sum) 
    for (int i = 1; i < n; i++) {
        double x = a + i * h; 
        sum += f(x); 
    }
    return h * sum; 
}

int main() {
    // Variables iniciales del documento
    double a = 0.0, b = M_PI; 
    int n = 10000000; 
    
    printf("Iniciando comparacion de rendimiento para Regla del Trapecio...\n");
    printf("Subintervalos (n) = %d\n\n", n);

    // --- Medición y ejecución Secuencial ---
    double t0_sec = omp_get_wtime();
    double resultado_sec = trapecio_secuencial(a, b, n);
    double t1_sec = omp_get_wtime();
    
    printf("Resultado (secuencial): %.10f\n", resultado_sec);
    printf("Tiempo secuencial:      %.5f segundos\n\n", t1_sec - t0_sec);

    // --- Medición y ejecución Paralelizada ---
    double t0_par = omp_get_wtime(); 
    double resultado_par = trapecio_parallel(a, b, n); 
    double t1_par = omp_get_wtime(); 
    
    printf("Resultado (paralelo):   %.10f\n", resultado_par); 
    printf("Tiempo paralelo:        %.5f segundos\n", t1_par - t0_par); 

    double speedup = (t1_sec - t0_sec) / (t1_par - t0_par);
    printf("\nEl paralelo es %.2f x más rapido que el serial\n", speedup);

    return 0;
}
