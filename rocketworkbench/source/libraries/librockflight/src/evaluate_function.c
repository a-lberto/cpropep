#include <stdlib.h>
#include <stdio.h>

#include "librockflight/include/state.h"

int interpolate(int n_point, float *x, float *y, double *ans, double xval);

int evaluate_function(function_t *f, state_t *s, double time, double *ans)
{
  double independant_var_value = 0.0;
  
  switch (f->type)
  {
    case _CONSTANT:
        *ans = f->constant_value;
        break;

    case _TABLE:
        switch (f->independant_var)
        {
          case TIME:
              independant_var_value = time;
              break;
              
          case ALT:
              independant_var_value = s->alt;
              break;

          case MACH:
              independant_var_value = s->mach;
              break;
              
          default:

        }
        
        interpolate(f->n_point, f->x, f->y, ans, independant_var_value);
        
        break;
        
    case _FUNCTION:
        printf("Analytical functions are not supported yet.\n");
        break;
        
  }
  return 0;
}


int interpolate(int n_point, float *x, float *y, double *ans, double xval)
{
  int i;
  
  if (xval < x[0])
  {
    *ans = (y[0]/x[0])*xval;
  }
  else
  {
    
    for (i = 0; i < n_point; i++)
    {
      if (xval < x[i])
      {
        *ans = y[i-1] + ( (y[i]-y[i-1])/(x[i]-x[i-1]) ) * (xval - x[i-1]);
        return 0;
      }
    }

    /* use the last segment of curve to extrapolate out of range value */
    *ans = y[n_point-1];
  }

  return 0;
}

