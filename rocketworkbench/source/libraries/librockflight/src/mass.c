

#include <stdio.h>
#include <math.h>

#include "librockflight/include/state.h"
#include "mass.h"


/* compute rocket mass */

int mass(rocket_t *r, double *t)
{
  state_t *s;
  s = &(r->state);

  s->m = r->stage_properties[0].dry_mass;

  return 0;
}
