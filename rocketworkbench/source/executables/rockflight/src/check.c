#include <stdlib.h>
#include <stdio.h>

#include "librockflight/include/state.h"

extern int stage(rocket_t *r, double *t);
extern int mass(rocket_t *r, double *t);
extern int engine(rocket_t *r, double *t);

/* This function will output rocket state from 0 to t every dt interval.
   It is useful to check if the thrust, masses, engines and stages follow
   the configuration we want. */
int check(rocket_t *r, double tf, double dt)
{
  double t;

  for (t = 0.0; t < tf; t += dt)
  {
    if (stage(r, &t))
    {
      printf("Error in function stage.\n");
      return -1;
    }
    if (mass(r, &t))
    {
      printf("Error in function mass.\n");
      return -1;
    }
    if (engine(r, &t))
    {
      printf("Error in function engine.\n");
      return -1;
    }
    
    printf("Time: %5.2f s -> Stage: %d -> Mass: %7.2f -> Thrust: %7.2f\n",
           t, r->state.s, r->state.m, r->state.Feng[0]);
  }

  return 0;
}
