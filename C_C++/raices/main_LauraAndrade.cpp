#include <iostream>
#include <cmath>
#include <iomanip>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>

// --- Funciones del Taller ---
// Función 1: f(x) = x^3 - 5x + 1
double f1(double x, void *params) { return x * x * x - 5.0 * x + 1.0; }
double df1(double x, void *params) { return 3.0 * x * x - 5.0; }
void fdf1(double x, void *params, double *y, double *dy)
{
    *y = f1(x, params);
    *dy = df1(x, params);
}

// Función 2: f(x) = e^(-x) - x
double f2(double x, void *params) { return std::exp(-x) - x; }
double df2(double x, void *params) { return -std::exp(-x) - 1.0; }
void fdf2(double x, void *params, double *y, double *dy)
{
    *y = f2(x, params);
    *dy = df2(x, params);
}

// --- Solucionador por Intervalos (Bisección, Falsa Posición, Brent) ---
void resolver_intervalo(const gsl_root_fsolver_type *T, gsl_function F, double x_lo, double x_hi, const std::string &nombre)
{
    int status;
    int iter = 0, max_iter = 100;
    double r = 0.0;
    gsl_root_fsolver *s = gsl_root_fsolver_alloc(T);
    gsl_root_fsolver_set(s, &F, x_lo, x_hi);

    std::cout << "\n--- Ejecutando: " << nombre << " ---\n";
    do
    {
        iter++;
        status = gsl_root_fsolver_iterate(s);
        r = gsl_root_fsolver_root(s);
        x_lo = gsl_root_fsolver_x_lower(s);
        x_hi = gsl_root_fsolver_x_upper(s);
        status = gsl_root_test_interval(x_lo, x_hi, 0.0, 1e-5);
    } while (status == GSL_CONTINUE && iter < max_iter);

    std::cout << "Raíz encontrada: " << std::setprecision(6) << r
              << " | Iteraciones: " << iter << "\n";
    gsl_root_fsolver_free(s);
}

// --- Solucionador por Derivadas (Newton, Secante, Steffensen) ---
void resolver_derivada(const gsl_root_fdfsolver_type *T, gsl_function_fdf FDF, double x0, const std::string &nombre)
{
    int status;
    int iter = 0, max_iter = 100;
    double x = x0, x0_prev;
    gsl_root_fdfsolver *s = gsl_root_fdfsolver_alloc(T);
    gsl_root_fdfsolver_set(s, &FDF, x);

    std::cout << "\n--- Ejecutando: " << nombre << " ---\n";
    do
    {
        iter++;
        status = gsl_root_fdfsolver_iterate(s);
        x0_prev = x;
        x = gsl_root_fdfsolver_root(s);
        status = gsl_root_test_delta(x, x0_prev, 0.0, 1e-5);
    } while (status == GSL_CONTINUE && iter < max_iter);

    std::cout << "Raíz encontrada: " << std::setprecision(6) << x
              << " | Iteraciones: " << iter << "\n";
    gsl_root_fdfsolver_free(s);
}

int main()
{
    gsl_function F1_gsl = {&f1, nullptr};
    gsl_function_fdf FDF1_gsl = {&f1, &df1, &fdf1, nullptr};

    gsl_function F2_gsl = {&f2, nullptr};
    gsl_function_fdf FDF2_gsl = {&f2, &df2, &fdf2, nullptr};

    int opcion;
    int fn_opcion = 1;

    do
    {
        std::cout << "Selecciona la función a evaluar:\n";
        std::cout << "1. f(x) = x^3 - 5x + 1  (Intervalo [0, 1])\n";
        std::cout << "2. f(x) = e^(-x) - x    (Intervalo [0, 1])\n";
        std::cout << "Selección: ";
        std::cin >> fn_opcion;

        gsl_function F_actual = (fn_opcion == 2) ? F2_gsl : F1_gsl;
        gsl_function_fdf FDF_actual = (fn_opcion == 2) ? FDF2_gsl : FDF1_gsl;

        std::cout << "\nSelecciona el Método:\n";
        std::cout << "1. Bisección\n";
        std::cout << "2. Falsa Posición\n";
        std::cout << "3. Brent\n";
        std::cout << "4. Newton-Raphson\n";
        std::cout << "5. Secante\n";
        std::cout << "6. Steffensen\n";
        std::cout << "7. Ejecutar TODOS los métodos\n";
        std::cout << "0. Salir\n";
        std::cout << "Opción: ";
        std::cin >> opcion;

        switch (opcion)
        {
        case 1:
            resolver_intervalo(gsl_root_fsolver_bisection, F_actual, 0.0, 1.0, "Bisección");
            break;
        case 2:
            resolver_intervalo(gsl_root_fsolver_falsepos, F_actual, 0.0, 1.0, "Falsa Posición");
            break;
        case 3:
            resolver_intervalo(gsl_root_fsolver_brent, F_actual, 0.0, 1.0, "Brent");
            break;
        case 4:
            resolver_derivada(gsl_root_fdfsolver_newton, FDF_actual, 0.5, "Newton");
            break;
        case 5:
            resolver_derivada(gsl_root_fdfsolver_secant, FDF_actual, 0.5, "Secante");
            break;
        case 6:
            resolver_derivada(gsl_root_fdfsolver_steffenson, FDF_actual, 0.5, "Steffensen");
            break;
        case 7:
            resolver_intervalo(gsl_root_fsolver_bisection, F_actual, 0.0, 1.0, "Bisección");
            resolver_intervalo(gsl_root_fsolver_falsepos, F_actual, 0.0, 1.0, "Falsa Posición");
            resolver_intervalo(gsl_root_fsolver_brent, F_actual, 0.0, 1.0, "Brent");
            resolver_derivada(gsl_root_fdfsolver_newton, FDF_actual, 0.5, "Newton");
            resolver_derivada(gsl_root_fdfsolver_secant, FDF_actual, 0.5, "Secante");
            resolver_derivada(gsl_root_fdfsolver_steffenson, FDF_actual, 0.5, "Steffensen");
            break;
        case 0:
            std::cout << "Saliendo del programa...\n";
            break;
        default:
            std::cout << "Opción no válida. Intenta de nuevo.\n";
        }
    } while (opcion != 0);

    return 0;
}