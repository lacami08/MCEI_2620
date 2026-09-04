#include <iostream>
#include <iomanip>
#include <cmath>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

double f(double x, void *params)
{
  return std::exp(-x) - x;
}

double df(double x, void *params)
{
  return -std::exp(-x) - 1.0;
}
void fdf(double x, void *params, double *f_val, double *df_val)
{
  *f_val = f(x, params);
  *df_val = df(x, params);
}

int main()
{
  const gsl_root_fdfsolver_type *T;
  gsl_root_fdfsolver *s;

  T = gsl_root_fdfsolver_newton; // Steffensen

  gsl_function_fdf FDF;
  FDF.f = &f;
  FDF.df = &df;
  FDF.fdf = &fdf;
  FDF.params = nullptr;

  double x = 0.0; // Valor de inicio (x0)
  double x0;

  s = gsl_root_fdfsolver_alloc(T);
  gsl_root_fdfsolver_set(s, &FDF, x);

  std::cout << std::setw(6) << "iter"
            << std::setw(16) << "raíz"
            << std::setw(16) << "error" << "\n";

  int status;
  int iter = 0;
  int max_iter = 100;

  do
  {
    iter++;
    status = gsl_root_fdfsolver_iterate(s);
    x0 = x;
    x = gsl_root_fdfsolver_root(s);
    status = gsl_root_test_delta(x, x0, 0.0, 1e-8);

    std::cout << std::setw(6) << iter
              << std::setw(16) << x
              << std::setw(16) << std::abs(x - x0) << "\n";

  } while (status == GSL_CONTINUE && iter < max_iter);

  std::cout << "\nRaíz encontrada = " << x << std::endl;
  gsl_root_fdfsolver_free(s);
  return 0;
}