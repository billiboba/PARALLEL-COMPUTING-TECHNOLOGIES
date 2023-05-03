#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

double
func (double x)
{
  return pow (x, 4) / (0.5 * pow (x, 2) + x + 6);
}

int
main (int argc, char **argv)
{
  int h = 1;
  double number1 = 0.0;
  for (int l = 1; l <= 4; l++)
    {
      double t = omp_get_wtime ();
      const double eps = 1E-5;
      const double a = 0.4;
      const double b = 1.5;
      const int n0 = 100000000;
      printf ("кол-во потоков: %d\n", l);
      printf ("Numerical integration: [%f, %f], n0 = %d, EPS = %f\n", a, b, n0,
              eps);
      double sq[2];
#pragma omp parallel num_threads(l)
      {
        int n = n0, k;
        double delta = 1;
        for (k = 0; delta > eps; n *= 2, k ^= 1)
          {
            double h = (b - a) / n;
            double s = 0.0;
            sq[k] = 0;
#pragma omp barrier
#pragma omp for nowait
            for (int i = 0; i < n; i++)
              s += func (a + h * (i + 0.5));
#pragma omp atomic
            sq[k] += s * h;
// Ждем пока все потоки обновят sq[k]
#pragma omp barrier
            if (n > n0)
              delta = fabs (sq[k] - sq[k ^ 1]) / 3.0;
#if 0
#endif
          }
#pragma omp master
        printf ("Result Pi: %.12f; Runge rule: EPS %e, n %d\n", sq[k] * sq[k],
                eps, n / 2);
      }
      t = omp_get_wtime () - t;
      if (h == 1)
        {
          printf ("Elapsed time (sec.): %.6f\n", t);
          number1 = t;
          h++;
        }
      else
        {
          printf ("time = %.12f\n", t);
          printf ("speedup = %.12f\n", number1 / t);
        }
    }
  return 0;
}
