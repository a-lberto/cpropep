/* Liquid rocket design */


#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "libcpropep/include/print.h"
#include "libcpropep/include/type.h"
#include "libthermo/include/thermo.h"
#include "libcpropep/include/conversion.h"

#include "liblrd/include/lrd.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int LRD_compute_caracteristics(equilibrium_t *e, design_param_t *d, motor_t *m,
                               liquid_propellant_t *p)
{
  equilibrium_t *throat, *exit;

  throat = e + 1;
  exit   = e + 2;
  
  m->mass_flow_rate   = d->thrust / exit->performance.Isp;
  m->fu_flow_rate     = m->mass_flow_rate / (p->o_f_ratio + 1);
  m->ox_flow_rate     = m->mass_flow_rate - m->fu_flow_rate;

  m->throat_area      = m->mass_flow_rate * throat->performance.a_dotm
    / ATM_TO_PA; 
  m->throat_diameter  = 2 * sqrt(m->throat_area / M_PI);
  m->throat_radius    = m->throat_diameter/2;
  
  m->exit_area        = m->throat_area * exit->performance.ae_at;
  m->exit_diameter    = 2 * sqrt(m->exit_area / M_PI);
  m->exit_radius      = m->exit_diameter/2;
    
  m->Ac_At            = pow(d->Dc_Dt, 2);
  m->chamber_volume   = d->Lstar * m->throat_area;
  m->chamber_diameter = d->Dc_Dt * m->throat_diameter;
  m->chamber_radius   = m->chamber_diameter/2;
  m->chamber_area     = M_PI * pow(m->chamber_radius, 2);

  m->convergent_length = (m->throat_radius*(sqrt(m->Ac_At) - 1) +
                          d->R_Rt*m->throat_radius*
                          (pow(cos(d->theta*M_PI/180),-1) -1))
    /tan(d->theta*M_PI/180);

/*  m->convergent_volume = (M_PI/3)*m->convergent_length*
      (pow(m->chamber_radius, 2)+pow(m->throat_radius, 2)+
       m->chamber_radius*m->throat_radius); */


  m->convergent_volume = (1/3) * m->throat_area *sqrt(m->throat_area/M_PI) *
    pow(tan(d->theta*M_PI/180), -1)*(pow(m->Ac_At, 1/3) - 1);

  /*
  m->convergent_length = d->R_Rt*m->throat_radius*sin(d->theta*M_PI/180) +
    (m->chamber_radius - d->R_Rt*m->throat_radius*(cos(d->theta*M_PI/180)-1)
     - m->throat_radius)/tan(d->theta*M_PI/180);
  */
    
  m->cylinder_volume = m->chamber_volume - m->convergent_volume;
  
        
  m->cylinder_length   = m->cylinder_volume / m->chamber_area;

  m->chamber_length = m->cylinder_length + m->convergent_length;
 
  m->walls_area       = 2*m->chamber_length*sqrt(M_PI*m->Ac_At*m->throat_area)
    + pow(sin(d->theta), -1)*(m->Ac_At - 1)*m->throat_area;
  
  return 0;
}
    
int LRD_print_propellant_info(liquid_propellant_t *p)
{
  printf("Propellant\n");
  printf("----------\n");
  printf("Fuel      : %s\n", PROPELLANT_NAME(p->fuel));
  printf("Oxidant   : %s\n", PROPELLANT_NAME(p->oxidant));
  printf("O/F ratio : %.2f\n\n", p->o_f_ratio);
  return 0;
}

int LRD_print_design_criteria(design_param_t *c)
{
  printf("Design criteria\n");
  printf("---------------\n");
  printf("Chamber pressure (atm)  (psi)  : % 11.4f % 11.4f\n",
          c->chamber_pressure, c->chamber_pressure * ATM_TO_PSI);
  printf("Thrust           (N)    (lbf)  : % 11.4e % 11.4e\n",
         c->thrust, c->thrust * N_TO_LBF);
  printf("L*               (m)    (in)   : % 11.4f % 11.4f\n",
         c->Lstar, c->Lstar * M_TO_IN);
  printf("Chamber to throat diameter     : % 11.4f\n",
         c->Dc_Dt);
  printf("R/Rt                           : % 11.4f\n",
         c->R_Rt);
  printf("Convergent half angle (deg)    : % 11.4f\n", c->theta);
  printf("\n");
  return 0;
}

int LRD_print_design_information(motor_t *mf, motor_t *ms)
{
  printf("\n");
  printf("                                           "
         "frozen         |        shifting\n");
  printf("Mass flow rate     (kg/s) (lb/s) : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->mass_flow_rate, mf->mass_flow_rate * KG_S_TO_LB_S,
         ms->mass_flow_rate, ms->mass_flow_rate * KG_S_TO_LB_S);
  printf("Fuel flow rate     (kg/s) (lb/s) : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->fu_flow_rate, mf->fu_flow_rate * KG_S_TO_LB_S,
         ms->fu_flow_rate, ms->fu_flow_rate * KG_S_TO_LB_S);
  printf("Oxidant flow rate  (kg/s) (lb/s) : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->ox_flow_rate, mf->ox_flow_rate * KG_S_TO_LB_S,
         ms->ox_flow_rate, ms->ox_flow_rate * KG_S_TO_LB_S);
  printf("Throat area         (m^2) (in^2) : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->throat_area, mf->throat_area * M2_TO_IN2,
         ms->throat_area, ms->throat_area * M2_TO_IN2);
  printf("Throat diameter     (m)   (in)   : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->throat_diameter, mf->throat_diameter * M_TO_IN,
         ms->throat_diameter, ms->throat_diameter * M_TO_IN);
  printf("Exit area           (m^2) (in^2) : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->exit_area, mf->exit_area * M2_TO_IN2,
         ms->exit_area, ms->exit_area * M2_TO_IN2);
  printf("Exit diameter       (m)   (in)   : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->exit_diameter, mf->exit_diameter * M_TO_IN,
         ms->exit_diameter, ms->exit_diameter * M_TO_IN); 
  printf("Chamber volume      (m^3) (in^3) : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->chamber_volume, mf->chamber_volume * M3_TO_IN3,
         ms->chamber_volume, ms->chamber_volume * M3_TO_IN3);
  printf("Chamber diameter    (m)   (in)   : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->chamber_diameter, mf->chamber_diameter * M_TO_IN,
         ms->chamber_diameter, ms->chamber_diameter * M_TO_IN);
  printf("Chamber length      (m)   (in)   : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->chamber_length, mf->chamber_length * M_TO_IN,
         ms->chamber_length, ms->chamber_length * M_TO_IN);
  printf("Convergent length   (m)   (in)   : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->convergent_length, mf->convergent_length * M_TO_IN,
         ms->convergent_length, ms->convergent_length * M_TO_IN);
  printf("Cylinder length     (m)   (in)   : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->cylinder_length, mf->cylinder_length * M_TO_IN,
         ms->cylinder_length, ms->cylinder_length * M_TO_IN);
  printf("Chamber walls area  (m^2) (in^2) : %8.4e %8.4e  | %8.4e %8.4e\n",
         mf->walls_area, mf->walls_area * M2_TO_IN2,
         ms->walls_area, ms->walls_area * M2_TO_IN2);
  
  return 0;
}



