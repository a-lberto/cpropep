#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cgi.h>


#include "libcpropep/include/equilibrium.h"
#include "libcpropep/include/performance.h"
#include "libthermo/include/thermo.h"
#include "libcpropep/include/conversion.h"

#include "libthermo/include/load.h"
#include "libcpropep/include/print.h"

#include "liblrd/include/lrd.h"

s_cgi *cgi;

#ifdef SOURCEFORGE
#define THERMO_PATH "thermo.dat"
#define PROPELLANT_PATH "propellant.dat"
#else
#define THERMO_PATH "/home/antoine/projets/rocket/rocketworkbench/data/thermo.dat"
#define PROPELLANT_PATH "/home/antoine/projets/rocket/rocketworkbench/data/propellant.dat"
#endif


int init_equil(void)
{
  if (load_thermo (THERMO_PATH) < 0)
    return -1;
  if (load_propellant (PROPELLANT_PATH) < 0)
    return -1; 

  return 0;
}

void destroy(void)
{
  free (propellant_list);
  free (thermo_list);
}

int eval_cgi(liquid_propellant_t *p, design_param_t *d)
{
  int sp;
  
  char   buffer[64];
  double tmp;
  double tmp1;

  
  strncpy(buffer, cgiGetValue(cgi, "oxidant"), 64);
  sp = atoi(buffer);

  if ((sp >= 0) && (sp < num_propellant))
  {
    p->oxidant = sp;
  }
  
  
  strncpy(buffer, cgiGetValue(cgi, "fuel"), 64);
  sp = atoi(buffer);

  
  if ((sp >= 0) && (sp < num_propellant))
  {
    p->fuel = sp;
  }
  
  strncpy(buffer, cgiGetValue(cgi, "ratio"), 64);
  tmp = atof(buffer);

  if (tmp >= 0.0)
  {
    p->o_f_ratio = tmp;
  }
  
  strncpy(buffer, cgiGetValue(cgi, "pressure"), 64);
  tmp = atof(buffer);

  strncpy(buffer, cgiGetValue(cgi, "pressure_unit"), 64);

  if (tmp > 0.0)
  {
    if (strcmp(buffer, "atm") == 0)
    {
      d->chamber_pressure = tmp;
    }
    else if (strcmp(buffer, "psi") == 0)
    {
      d->chamber_pressure = tmp * PSI_TO_ATM;
    }
  }
  
  strncpy(buffer, cgiGetValue(cgi, "thrust"), 64);
  tmp = atof(buffer);

  strncpy(buffer, cgiGetValue(cgi, "thrust_unit"), 64);

  if (tmp > 0.0)
  {
    if (strcmp(buffer, "newton") == 0)
    {
      d->thrust = tmp;
    }
    else if (strcmp(buffer, "pound-force") == 0)
    {
      d->thrust = tmp * LBF_TO_N;
    }
  }
  
  strncpy(buffer, cgiGetValue(cgi, "lstar"), 64);
  tmp = atof(buffer);

  strncpy(buffer, cgiGetValue(cgi, "length_unit"), 64);

  if (tmp > 0.0)
  {
    if (strcmp(buffer, "m") == 0)
    {
      d->Lstar = tmp;
    }
    else if (strcmp(buffer, "in") == 0)
    {
      d->Lstar = tmp * IN_TO_M;
    }
  }
  
  
  strncpy(buffer, cgiGetValue(cgi, "dc_dt"), 64);
  tmp = atof(buffer);

  d->Dc_Dt = tmp;

  strncpy(buffer, cgiGetValue(cgi, "r_rt"), 64);
  tmp = atof(buffer);
  
  d->R_Rt = tmp;

  strncpy(buffer, cgiGetValue(cgi, "theta"), 64);
  tmp = atof(buffer);
  
  d->theta = tmp;
  
  return 0;
}


int main (int argc, char **argv, char **env)
{
  short i;
  
  char *path_info = NULL;  
  
  char buffer[32];

  double value;
  int val;

  liquid_propellant_t p;
  design_param_t      param;
  motor_t             motor_frozen,
                      motor_shifting;
  
  equilibrium_t      *frozen,
                     *shifting;

  
  errorfile = stdout;
  outputfile = stdout;
    
  cgiDebug(0, 0);
  cgi = cgiInit();
  
  global_verbose = 0;
  
  path_info = getenv("PATH_INFO");

  if (path_info) 
  {
    if (!strcmp(path_info, "/design")) 
    {
      cgiHeader();
	    printf("<pre>");

      frozen   = (equilibrium_t *) malloc (sizeof(equilibrium_t) * 3);
      shifting = (equilibrium_t *) malloc (sizeof(equilibrium_t) * 3);

      for (i = 0; i < 3; i++)
      {
        initialize_equilibrium(frozen + i);
        initialize_equilibrium(shifting + i);
      }

      if (init_equil())
      {
        printf("Problem loading data file.\n");
        return 0;
      }

      eval_cgi(&p, &param);

      add_in_propellant(frozen, p.fuel,    GRAM_TO_MOL(10.0, p.fuel));
      add_in_propellant(frozen, p.oxidant, GRAM_TO_MOL(10.0 * p.o_f_ratio,
                                                     p.oxidant));
      
      copy_equilibrium(shifting, frozen);
      
      printf("Liquid-fuel rocket engine design (lrd).\n\n");
      
      LRD_print_propellant_info(&p);
      LRD_print_design_criteria(&param);
      
      frozen->properties.P = param.chamber_pressure;
      equilibrium(frozen, HP);
      frozen_performance(frozen, PRESSURE, 1);

      LRD_compute_caracteristics(frozen, &param, &motor_frozen, &p);
      
      shifting->properties.P = param.chamber_pressure;
      equilibrium(shifting, HP);
      shifting_performance(shifting, PRESSURE, 1);
      
      LRD_compute_caracteristics(shifting, &param, &motor_shifting, &p);
      
      LRD_print_design_information(&motor_frozen, &motor_shifting);
      
      printf("\nFrozen equilibrium caracteristics.\n\n");
      print_product_properties(frozen, 3);
      print_performance_information(frozen, 3);
      print_product_composition(frozen, 3);
      
      printf("\nShifting equilibrium caracteristics.\n\n");
      print_product_properties(shifting, 3);
      print_performance_information(shifting, 3);
      print_product_composition(shifting, 3);


      free(frozen);
      free(shifting);
      
	    printf("</pre>");
	    
    }
    else 
    {
	    cgiHeader();
	    printf("<br>Bad queries<br>");
    }
  } 
  else 
  {
    cgiHeader();
    printf("<br>Bad PATH_INFO<br>");
  }

  printf ("\n<hr>\n</body>\n</html>\n");
  
  destroy();
  return 0;
}



