#include <stdlib.h>

#include "gpcp.h"
#include "profiler.h"

double mass_convert;   /* factor to convert user units to SI (kg) */
double length_convert; /* factor to convert user units to SI (m) */

int read_value(Data *d, rocket_t *r)
{
  int i, j, k, l, m, n;

  body_part_t *part;
  fin_t *fin;

  char *str;
  int int_val;
  float float_val;
  char *type = NULL;

  if (GPCP_EnterLevel("global", 0))
  {
    printf("No global value defined.\n");
  }
  else
  {
    if (GPCP_GetValue("length_units", &str) != 0)
    {
      printf("No length units have been specified.\n"
	     "Values are assumed to be in meter.\n");
      length_convert = 1.0;
    }
    else
    {
      printf("Length units used are: %s\n", str);
      if ((strcmp(str, "m") == 0) || (strcmp(str, "meter") == 0))
      {
	length_convert = 1.0;
      }
      else if (strcmp(str, "in") == 0)
      {
	length_convert = 0.0254;
      }
      else
      {
	printf("Unknown length unit.\n"
	       "Values are assumed to be in meter.\n");
	length_convert = 1.0;
      }

    }

    if (GPCP_GetValue("mass_units", &str) != 0)
    {
      printf("No mass units have been specified.\n"
	     "Values are assumes to be in kilograms.\n");
      mass_convert = 1.0;
    }
    else
    {
      printf("Mass units used are  : %s\n", str);  
      if (strcmp(str, "kg") == 0)
      {
	mass_convert = 1.0;
      }
      else if (strcmp(str, "lb") == 0)
      {
	mass_convert = 0.45359237;
      }
      else
      {
	printf("Unknown mass unit.\n"
	       "Values are assumed to be in kilograms.\n");
	mass_convert = 1.0;
      }
    }
  }

  GPCP_ExitLevel();

  l = GPCP_NumParent("rocket");

  for (i = 0; i < l; i++)
  {
    if (GPCP_EnterLevel("rocket", i))
    {
      printf("Strange, should not be here....\n");
    }
    else
    {
      if (GPCP_GetValue("name", &(r->name)) != 0)
      {
      }
      else
      {
	printf("Rocket name : %s\n", r->name);
      }
      
      m = GPCP_NumParent("body");
      for (j = 0; j < m; j++)
      {
	if (GPCP_EnterLevel("body", j))
	{
	  printf("Strange, should not be here....\n");
	}
	else
	{
	  r->body.n_body_parts++;
	  r->body.body_parts = (body_part_t *) 
	    realloc(r->body.body_parts, 
		    r->body.n_body_parts*sizeof(body_part_t));
	  
	  part = r->body.body_parts + j;

	  if (GPCP_GetValue("type", &type) != 0)
	  {
	  }
	  else
	  {
	    if (strcmp(type, "ogive_nose") == 0)
	    {
	      if (r->body.n_body_parts != 1)
	      {
		printf("Only the first part could be a nose.\n");
		return -1;
	      }
	      part->type = OGIVE_NOSE;
	    }
	    else if (strcmp(type, "elliptic_nose") == 0)
	    {
	      if (r->body.n_body_parts != 1)
	      {
		printf("Only the first part could be a nose.\n");
		return -1;
	      }
	      part->type = ELLIPTIC_NOSE;
	    }
	    else if (strcmp(type, "conic_nose") == 0)
	    {
	      if (r->body.n_body_parts != 1)
	      {
		printf("Only the first part could be a nose.\n");
		return -1;
	      }
	      part->type = CONIC_NOSE;
	    }
	    else if (strcmp(type, "parabolic_nose") == 0)
	    {
	      if (r->body.n_body_parts != 1)
	      {
		printf("Only the first part could be a nose.\n");
		return -1;
	      }
	      part->type = PARABOLIC_NOSE;
	    }
	    else if (strcmp(type, "tube") == 0)
	    {
	      if (r->body.n_body_parts == 1)
	      {
		printf("The first part should be a nose.\n");
		return -1;
	      }
	      part->type = TUBE;
	      part->d = r->body.body_parts[j - 1].d;

	    }
	    else if (strcmp(type, "transition") == 0)
	    {
	      if (r->body.n_body_parts == 1)
	      {
		printf("The first part should be a nose.\n");
		return -1;
	      }
	      part->type = TRANSITION;
	    }
	    else
	    {
	      printf("Unknown type of body part.\n");
	    } 
	  }

	  if (GPCP_GetValue("length", &float_val) != 0)
	  {
	  }
	  else
	  {
	    part->l = float_val * length_convert;
	    printf("Length is : %f (m)\n", float_val * length_convert);
	  }

	  if (GPCP_GetValue("diameter", &float_val) != 0)
	  {
	    
	  }
	  else
	  {
	    part->d = float_val * length_convert;
	    printf("Diameter is : %f (m)\n", float_val * length_convert);
	    if (part->d > r->Lr) 
	      r->Lr = part->d;
	  }

	  if (GPCP_GetValue("density", &float_val) != 0)
	  {
	  }
	  else
	  {
	    printf("Density is : %f\n", float_val);
	  }


	}
	GPCP_ExitLevel();
      }     
    
      m = GPCP_NumParent("fin");
      for (j = 0; j < m; j++)
      {
	if (GPCP_EnterLevel("fin", j))
	{
	  printf("Strange, should not be here....\n");
	}
	else
	{
	  r->n_fin_set += 1;
	  r->fin_set = (fin_t *) realloc(r->fin_set, 
					 r->n_fin_set*sizeof(fin_t));

	  fin = r->fin_set + j;

	  if (GPCP_GetValue("num", &int_val) != 0)
	  {
	    printf("Number of fins not specified.\n");
	    return -1;
	  }
	  else
	  {
	    fin->N = int_val;
	  }

	  if (GPCP_GetValue("pos", &float_val) != 0) 
	  {
	    printf("Position of fins not specified.\n");
	    return -1;
	  }
	  else
	  {
	    fin->lt = float_val * length_convert;
	  }

	  if (GPCP_GetValue("root_chord", &float_val) != 0)
	  {
	    printf("Root chord not specified.\n");
	    return -1;
	  }
	  else
	  {
	    fin->cr = float_val * length_convert;
	  }

	  if (GPCP_GetValue("tip_chord", &float_val) != 0)
	  {
	    printf("Tip chord not specified.\n");
	    return -1;
	  }
	  else
	  {
	    fin->ct = float_val * length_convert;
	  }

	  if (GPCP_GetValue("root_tip_d", &float_val) != 0)
	  {
	    printf("Position of tip chord relative to root chord"
		   " not specified.\n");
	    return -1;
	  }
	  else
	  {
	    fin->xt = float_val * length_convert;
	  }

	  if (GPCP_GetValue("semispan", &float_val) != 0)
	  {
	    printf("Semispan not specified.\n");
	    return -1;
	  }
	  else
	  {
	    fin->s = float_val * length_convert;
	  }

	  if (GPCP_GetValue("body_radius", &float_val) != 0)
	  {
	    printf("Body radius not specified.\n");
	    return -1;
	  }
	  else
	  {
	    fin->rt = float_val * length_convert;
	  }

	}
	GPCP_ExitLevel();
      }
     
      if (GPCP_EnterLevel("motor", 0))
      {

      }
      else
      {
	
      }
      GPCP_ExitLevel();

      if (GPCP_EnterLevel("tank", 0))
      {

      }
      else
      {
	
      }
      GPCP_ExitLevel();

      if (GPCP_EnterLevel("other", 0))
      {

      }
      else
      {
	
      }
      GPCP_ExitLevel();

    }  
    GPCP_ExitLevel();      
  }
  return 0;
}
