#include <stdio.h>
#include <math.h>
#include <omp.h>

// Función a integrar
double f(double x) {
    return sin(x); 
}

// 1. Función Secuencial (Sin paralelizar)
double simpson_secuencial(double a, double b, int n) {
    if (n % 2 != 0) n++; 
    
    double h = (b - a) / n; 
    double sum = f(a) + f(b); 
    double suma_par = 0.0, suma_impar = 0.0; 
    
    // Bucle normal sin directivas OpenMP
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        if (i % 2 == 0)
            suma_par += f(x);
        else
            suma_impar += f(x);
    }
    return (h / 3.0) * (sum + 2 * suma_par + 4 * suma_impar); 
}

// 2. Función Paralelizada (Basada en el documento)
double simpson_parallel(double a, double b, int n) {
    if (n % 2 != 0) n++; 
    
    double h = (b - a) / n; 
    double sum = f(a) + f(b); 
    double suma_par = 0.0, suma_impar = 0.0; 
    
    // Directiva para paralelizar el ciclo y evitar condiciones de carrera con reduction
    #pragma omp parallel for reduction(+:suma_par, suma_impar) 
    for (int i = 1; i < n; i++) {
        double x = a + i * h; 
        if (i % 2 == 0)
            suma_par += f(x); 
        else
            suma_impar += f(x); 
    }
    return (h / 3.0) * (sum + 2 * suma_par + 4 * suma_impar); 
}

int main() {
    // Variables iniciales del documento
    double a = 0.0, b = M_PI; 
    int n = 10000000; 
    
    printf("Iniciando comparacion de rendimiento para Metodo de Simpson...\n");
    printf("Subintervalos (n) = %d\n\n", n);

    // --- Medición y ejecución Secuencial ---
    double t0_sec = omp_get_wtime();
    double result_sec = simpson_secuencial(a, b, n);
    double t1_sec = omp_get_wtime();
    
    printf("Resultado (secuencial): %.10f\n", result_sec);
    printf("Tiempo secuencial:      %.5f segundos\n\n", t1_sec - t0_sec);

    // --- Medición y ejecución Paralelizada ---
    double t0_par = omp_get_wtime(); 
    double result_par = simpson_parallel(a, b, n); 
    double t1_par = omp_get_wtime(); 
    
    printf("Resultado (paralelo):   %.10f\n", result_par); 
    printf("Tiempo paralelo:        %.5f segundos\n", t1_par - t0_par); 

    double speedup = (t1_sec - t0_sec) / (t1_par - t0_par);
    printf("\nEl paralelo es %.2f x más rapido que el serial\n", speedup);

    return 0;
}
