#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "libthermo/include/load.h"
#include "libcpropep/include/print.h"
#include "libthermo/include/thermo.h"

#include "lrd_load.h"
#include "liblrd/include/lrd.h"

#define THERMO_FILE     "../../data/thermo.dat"
#define PROPELLANT_FILE "../../data/propellant.dat"


int main(int argc, char *argv[])
{
  int i;

  liquid_propellant_t p;
  design_param_t      param;
  motor_t             motor_frozen,
                      motor_shifting;
  
  equilibrium_t      *frozen,
                     *shifting;


  load_input("input.lr", &param, &p);
  
  /* Initialisation of cpropep lib */
  errorfile  = stdout;
  outputfile = stdout;
  
  if (load_thermo (THERMO_FILE) < 0)
  {
    printf("Error loading thermodynamic data file.\n");
    return -1;
  }
  
  if (load_propellant (PROPELLANT_FILE) < 0)
  {
    printf("Error loading propellant data file.\n");
    return -1;
  }
  
  frozen   = (equilibrium_t *) malloc (sizeof(equilibrium_t) * 3);
  shifting = (equilibrium_t *) malloc (sizeof(equilibrium_t) * 3);
  
    
  for (i = 0; i < 3; i++)
  {
    initialize_equilibrium(frozen + i);
    initialize_equilibrium(shifting + i);
  }

  add_in_propellant(frozen, p.fuel,    GRAM_TO_MOL(10.0, p.fuel));
  add_in_propellant(frozen, p.oxidant, GRAM_TO_MOL(10.0 * p.o_f_ratio, p.oxidant));

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
  
  return 0;
}
