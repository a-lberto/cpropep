

#include <stdio.h>
#include <math.h>

#include "librockflight/include/state.h"
#include "engine.h"

int evaluate_function(function_t *f, state_t *s, double time, double *ans);

/* Add engine component of force and moment */
int engine(rocket_t *r, double *t)
{
  int i;
  float tref = 0.0;
  float trel;

  float thrust = 0.0;
  float norm = 1.0;

  double tmp_val;
  
  state_t *s;
  engine_t *e = NULL;
  
  s = &(r->state);

  for (i = 0; i < s->s; i++)
    tref +=  r->stage_properties[i].active_time;

  for (i = 0; i < r->stage_properties[s->s].n_engine; i++)
  {
    e = r->stage_properties[s->s].engines + i;

    norm = sqrt( pow(e->direction[0], 2) +
                 pow(e->direction[1], 2) +
                 pow(e->direction[2], 2) );

    trel = *t - tref - e->start_time;
    
    if ((trel >= 0.0) && (*t < tref + e->drop_time))
    {
      evaluate_function(&(e->thrust), s, trel, &tmp_val);
      thrust += (float)tmp_val; 
    }
  }
 
  s->Feng[0] = thrust * e->direction[0] / norm;
  s->Feng[1] = thrust * e->direction[1] / norm;
  s->Feng[2] = thrust * e->direction[2] / norm;
      
  s->Meng[0] = 0.0;
  s->Meng[1] = 0.0;
  s->Meng[2] = 0.0;

  return 0;
}

