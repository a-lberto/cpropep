

#include <stdio.h>
#include <math.h>

#include "librockflight/include/state.h"
#include "engine.h"


/* Add engine component of force and moment */
int engine(rocket_t *r, double *t)
{
  int i, j;
  double tref = 0.0;
  double trel;

  double thrust = 0.0;
  double norm = 1.0;
  
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
      switch (e->thrust_type)
      {
        case _CONSTANT:
            thrust += e->thrust[0];
            
            break;

        case _FUNCTION:

            if (trel < e->time[0])
              thrust += (e->thrust[0]/e->time[0])*trel;
            else
            {
              
              for (j = 0; j < e->n_point; j++)
              {
                if (trel < e->time[j])
                {
                  thrust += e->thrust[j-1] +
                    ((e->thrust[j] - e->thrust[j-1])/
                     (e->time[j] - e->time[j-1])) * (trel - e->time[j-1]);
                  break;
                }
              }
              
              /*thrust = 0.0;*/
            }
            
            break;     
      }   
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
