

#include <stdio.h>
#include <math.h>

#include "librockflight/include/state.h"
#include "mass.h"


/* compute rocket mass */

int mass(rocket_t *r, double *t)
{
  int i, j;
  double tref = 0.0; /* this is the absolute time where the current stage
                        were activate */
  state_t *s;
  engine_t *e;
  
  s = &(r->state);

  s->m = 0;

  /* the active stage is s->s */
  if (s->s > r->n_stage - 1)
  {
    printf("active stage do not exist...\n");
    return -1;
  }

  tref = 0;
  for (i = 0; i < s->s; i++)
    tref +=  r->stage_properties[i].active_time;
  
  /* we start from the last stage down to the active stage to add masses */
  for (i = r->n_stage - 1; i > s->s; i--)
  {
    s->m += r->stage_properties[i].dry_mass;

    for (j = 0; j < r->stage_properties[i].n_engine; j++)
    {
      e = r->stage_properties[i].engines + j;
      s->m += e->dry_mass;
      s->m += e->propellant_mass; 
    }
  }

/*  printf("i = %d, tref = %5.2f\n", i, tref);*/
  
  s->m += r->stage_properties[i].dry_mass;

  for (j = 0; j < r->stage_properties[i].n_engine; j++)
  {    
    e = r->stage_properties[i].engines + j;

    if (*t < tref + e->drop_time)
    {
      s->m += e->dry_mass;
      
      if (*t < tref + e->start_time)
        s->m += e->propellant_mass;
      else /* linear interpolation of the propellant_mass */
        s->m += e->propellant_mass *
          (1 - (*t - tref - e->start_time)/e->burn_time);
    }        
  }
  
  return 0;
}
