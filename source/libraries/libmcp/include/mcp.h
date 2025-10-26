#ifndef mcp_h
#define mcp_h

#include "libcpropep/include/type.h"

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
  //double divergent_angle; /* deg */
	//double grain_surface;
 
} motor_t;

typedef struct _data_
{
  /* fixed parameters */
  double a;  /* burning coefficient */
  double n;  /* burning exponent */
  double ta; /* throat area */
  double ea; /* exit area */

  equilibrium_t *e; /* pointer to the equilibrium structure */
  
  /* changing parameters */

  double (*Ab)(double s); /* Burning area */
  double (*Vc)(double s); /* Chamber volume */
  
} data_t;


#endif
