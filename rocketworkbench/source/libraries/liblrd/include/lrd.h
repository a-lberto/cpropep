#ifndef lrd_h
#define lrd_h

#include "libcpropep/include/type.h"

typedef struct _design_param_t
{
  /* Design criteria */
  double chamber_pressure; /* Operating pressure of the motor (atm) */
  double thrust;           /* Thrust of the motor (N)               */
  double Lstar;            /* Cracteristic chamber lengtgh          */
  double Dc_Dt;            /* Chamber diameter / Throat diameter    */
  double R_Rt;             /* Ratio of nozzle contour arc radius
                              to throat radius                      */
  double theta;            /* Convergent half angle (deg)           */
  
} design_param_t;

typedef struct _motor_t
{
  /* Unknown that will be compute */
  double mass_flow_rate;    /* Total mass flow (kg/s)     */
  double ox_flow_rate;      /* Oxidant mass flow rate     */
  double fu_flow_rate;      /* Fuel mass flow rate        */

  double throat_area;       /* (m^2)                      */
  double throat_diameter;   /* (m)                        */
  double throat_radius;     /* (m)                        */
  
  double exit_area;         /* (m^2)                      */
  double exit_diameter;     /* (m)                        */
  double exit_radius;       /* (m)                        */
  
  double Ac_At;             /* Chamber area / throat area */

  /* total */
  double chamber_volume;    /* (m^3)                      */
  double chamber_area;      /* (m^2)                      */
  double chamber_diameter;  /* (m)                        */
  double chamber_radius;    /* (m)                        */
  double chamber_length;    /* (m)                        */

  /* conical section */
  double convergent_volume; /* (m^3)                      */
  double convergent_length; /* (m)                        */

  /* cylindrical section */
  double cylinder_volume;   /* (m^3)                      */
  double cylinder_length;   /* (m)                        */
  
  double walls_area;        /* total surface area of the
                               combustion chamber walls
                               excluding injector face    */
 
} motor_t;

typedef struct _liquid_propellant_t
{
  int oxidant;
  int fuel;

  double o_f_ratio;
  
} liquid_propellant_t;

int LRD_compute_caracteristics(equilibrium_t *e, design_param_t *d,
                               motor_t *m, liquid_propellant_t *p);
int LRD_print_propellant_info(liquid_propellant_t *p);
int LRD_print_design_criteria(design_param_t *c);
int LRD_print_design_information(motor_t *mf, motor_t *ms);

#endif
