

#include <stdio.h>
#include <math.h>

#include "librockflight/include/state.h"
#include "engine.h"


/* Add engine component of force and moment */
int engine(double *y, state_t *s)
{

  s->Feng[0] = 1000.0;
  s->Feng[1] = 0.0;
  s->Feng[2] = 0.0;

  s->Meng[0] = 0.0;
  s->Meng[1] = 0.0;
  s->Meng[2] = 0.0;

  return 0;
}
