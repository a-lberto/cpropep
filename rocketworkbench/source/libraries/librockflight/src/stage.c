

#include <stdio.h>
#include <math.h>

#include "librockflight/include/state.h"
#include "stage.h"


/* find the actual stage */
/* this routine should be optimized ! */
int stage(rocket_t *r, double *t)
{
     int i, j;
     state_t *s;
     double time = 0.0;
     
     s = &(r->state);
     
     for (i = 0; i < r->n_stage; i++)
     {
	  if ( i == r->n_stage - 1)
	  {
	       /* the last stage could not be drop */
	       s->s = i;
	       return 0;
	  }

	  time += r->stage_properties[i].active_time;

	  if (*t < time)
	  {
	       s->s = i;
	       return 0;
	  }
     }
  
  return 0;
}

