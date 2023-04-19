#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double
getrand (unsigned int *seed)
{
  return (double)rand_r (seed) / RAND_MAX;
}

double
func (double x, double y)
{
  return exp (x - y);
}

int
main ()
{
  const int n = 100000000;
  int h = 1;
  double r = 0.0;
  for (int threads = 1; threads <= 4; threads++)
    {
      //   printf ("Numerical integration by Monte Carlo method: n = %d\n", n);
      double t = omp_get_wtime ();
      double in = 0;
      double s = 0;
#pragma omp parallel num_threads(threads)
      {
        double s_loc = 0;
        double in_loc = 0;
        unsigned int seed = omp_get_thread_num ();
#pragma omp for nowait
        for (int i = 0; i < n; ++i)
          {
            double x = -getrand (&seed);
            double y = getrand (&seed);
            if (y <= 1)
              {
                in_loc++;
                s_loc += func (x, y);
              }
          }
#pragma omp atomic
        s += s_loc;
#pragma omp atomic
        in += in_loc;
      }

      double v = in / n;
      double res = v * s / in;
      printf ("\nКол-во потоков: %d\n", threads);
      printf ("Result: %.12f, n %d\n", res, n);
      t = omp_get_wtime () - t;
      printf ("Elapsed time (sec.) %.6f\n", t);
      if (h == 1)
        {
          r = t;
          h++;
        }
      else
        {
          printf ("ускорение: %f\n", r / t);
        }
    }
  return 0;
}