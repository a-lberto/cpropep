
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include "libthermo/include/load.h"
#include "libcpropep/include/print.h"
#include "libcpropep/include/type.h"
#include "libthermo/include/thermo.h"
#include "libcpropep/include/conversion.h"

#define THERMO_FILE     "../cpropep/data/thermo.dat"
#define PROPELLANT_FILE "../cpropep/data/propellant.dat"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* We assume a cylindrical motor */
typedef struct _motor_t
{
  /* propellant burning information */
  /* coefficient for burning rate equation
     r = aP^n (r is in m/s and P in atm */
  double a;
  double n;
  
  /* nozzle */
  double throat_diameter;
  double exit_diameter;  
  double divergent_angle; /* deg */
	double grain_surface;
 
} motor_t;


int load_input(FILE *fd, motor_t *motor, equilibrium_t *e)
{
	double m;
  
  int sp;
  int section = 0;
  int n_case = 0;
  
  char buffer[128], num[10], qt[10], unit[10];
  char variable[64];
  
  char *bufptr;

  while ( fgets(buffer, 128, fd) != NULL )
  {
    switch (section)
    {
      case 0:
          
          if (buffer[0] == ' ' || buffer[0] == '\n' || buffer[0] == '\0' ||
              buffer[0] == '#')
          {
            section = 0;
            break;
          }
          else if (strncmp(buffer, "Propellant", 10) == 0)
          {
            section = 1;
          }
					else if (strncmp(buffer, "Motor", 5) == 0)
					{
						section = 2;
					}          
					else
          {
						printf ("Unknown option.\n");
					}
          break;
          
      case 1:   /* propellant section */
          if (buffer[0] == '+')
          {
            sscanf(buffer, "%s %s %s", num, qt, unit);
            bufptr = num + 1;
            sp = atoi(bufptr);
            m = atof(qt);
            
						if (sp != 0)
						{
							if (strcmp(unit, "g") == 0)
							{
								add_in_propellant(e, sp, GRAM_TO_MOL(m, sp) );
							}
							else if (strcmp(unit, "m") == 0)
							{
								add_in_propellant(e, sp, m);
							}
							else
							{
								printf("Unit must be g (gram) or m (mol)\n");
								break;
							}
						}
						else
						{
							if (strcmp(bufptr, "a") == 0)
							{
								motor->a = m;
							}
							else if (strcmp(bufptr, "n") == 0)
							{
								motor->n = m;
							}
							else
							{
								printf("Unknown option.\n");
							}
						}
          }
          else if (buffer[0] == '#')
          {
            break;
          }
          else if (buffer[0] == ' ' || buffer[0] == '\n' || buffer[0] == '\0')
          {
            section = 0;
          }
          break;

      case 2:
          if (buffer[0] == '+')
          {
            sscanf(buffer, "%s %s %s", variable, qt, unit);
            bufptr = variable + 1;

            if (strcmp(bufptr, "throat_diameter") == 0)
            {
              m = atof(qt);
              motor->throat_diameter = m;
            }
            else if (strcmp(bufptr, "exit_diameter") == 0)
            {
              m = atof(qt);
							motor->exit_diameter = m;
            }
            else if (strcmp(bufptr, "propellant_surface") == 0)
            {
              m = atof(qt);
							motor->grain_surface = m;
            }
          }
          else if (buffer[0] == '#')
          {
            break;
          }
          else if (buffer[0] == ' ' || buffer[0] == '\n' || buffer[0] == '\0')
          {
            section = 0;
            n_case++;
          }
          break;
          
      default:
          section = 0;
          break;
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
  int i;

  FILE *fd;

  double chamber_pressure;
  
  motor_t motor;
  
  equilibrium_t *frozen;

  double throat_mass_flow_rate;
  double burning_mass_flow_rate;

  double throat_area;
  double exit_area;

  double Cd;
  
  double burning_area;
  
  errorfile = stderr;
  outputfile = stdout;

  
  if (load_thermo (THERMO_FILE) < 0 )
	{
		printf("Problem loading thermo file.\n");
		return -1;
	}
  if (load_propellant (PROPELLANT_FILE) < 0)
	{
		printf("Problem loading propellant file.\n");
		return -1;
	}
  
  frozen   = (equilibrium_t *) malloc (sizeof(equilibrium_t)*3);
  for (i = 0; i < 3; i++)
  {
    initialize_equilibrium(frozen + i);
  }

  fd = fopen("input.mot", "r");
	load_input(fd, &motor, frozen);	
	fclose(fd);

  compute_density(&(frozen->propellant));

  if (frozen->propellant.density == 0.0)
	{
		printf("Density is 0.0, Aborted.\n");
		return 0;
	}

  
  throat_area  = M_PI * pow(motor.throat_diameter/2, 2);
  exit_area    = M_PI * pow(motor.exit_diameter/2, 2);
  burning_area = motor.grain_surface;
  
  /* initial estimate of the chamber pressure */
  chamber_pressure = 60;

  i = 0;
  do
  {
		if (chamber_pressure > 500)
		{
			printf("Pressure increase over 500 atm. Aborted.\n");
			return -1;
		}
    frozen->properties.P = chamber_pressure;
    
    equilibrium(frozen, HP);
    
    frozen_performance(frozen, SUPERSONIC_AREA_RATIO,
                       exit_area / throat_area);
    
    throat_mass_flow_rate = throat_area * ATM_TO_PA /
      (frozen + 1)->performance.a_dotm;

    burning_mass_flow_rate = burning_area * (frozen->propellant.density * 1000)
      * motor.a * pow(chamber_pressure, motor.n);
        
    /* Evaluate Cd */
    Cd = ATM_TO_PA / ((frozen + 1)->performance.a_dotm * chamber_pressure);
    
    chamber_pressure = pow(
        (throat_area * Cd /(burning_area * motor.a
                            * (frozen + 1)->propellant.density * 1000)),
        1/(motor.n - 1));
    
    i++;
  } while (fabs(burning_mass_flow_rate - throat_mass_flow_rate) > 0.0004);


	printf("Pressure evaluation assuming frozen equilibrium.\n\n");
	print_propellant_composition(frozen);
	printf("Properties of the propellant.\n");
	printf("b = aP^n\n\n");
	printf("a = %.4f\n", motor.a);
	printf("n = %.4f\n\n", motor.n);

	print_product_properties(frozen, 3);
	print_product_composition(frozen, 3);
  print_performance_information(frozen, 3);

  printf("\n");
  printf("Chamber pressure converge in %d iterations.\n", i);
  printf("Pressure (atm)      : % .4f\n", chamber_pressure);
  printf("Throat dot m (kg/s) : % .4f\n", throat_mass_flow_rate);
  printf("Thrust (N)          : % .4f\n", throat_mass_flow_rate *
         (frozen + 2)->performance.Isp);

  printf("\nKn     : % .4f\n", burning_area/throat_area);

  return 0;
}

