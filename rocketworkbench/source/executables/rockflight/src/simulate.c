#include <math.h>

#include "libnum/include/num.h"

#include "librockflight/include/state.h"
#include "librockflight/include/rockflight.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int eom(int neq, double time, double *y, double *dy, void *data);

int simulate(rocket_t *r, solution_t *sol, float *ic, double **ans)
{

  int n;
  
  double time;
  float init_cond[12];

  int neq = 12;
      
/*  s.A = M_PI*s.D*s.D/4;*/ /* reference area (m^2) */
/*  s.C = 1;*/    /* reference distance */

  double u      = ic[0];
  double v      = ic[1];
  double w      = ic[2];
  double P      = ic[3];
  double Q      = ic[4];
  double R      = ic[5];
  double lambda = ic[6];
  double mu     = ic[7];
  double Re     = ic[8] + EARTH_RAD; /* convert altitude to distance 
					from eartj center */
  double phi    = ic[9];
  double theta  = ic[10]*(M_PI/180); /* convert deg to rad 
					(use convert instead) */
  double psi    = ic[11];


  /* in kg */
  r->state.m = 16.0; 

  r->state.Ix = 0.0025;
  r->state.Iy = 0.84;
  r->state.Iz = 0.84;
  
  /* aerodynamic coefficients */
  
  r->state.Cdrag    = 0.2;
  r->state.Clift    = 2.3;
  r->state.Cbeta    = 2.3;
  r->state.Cspin    = 20.0;
  r->state.Cmoment  = 6.32;
  r->state.Cdamping = 31.6;
  
  r->state.D = 0.1143;
  
  r->state.Faero[0] = 0;
  r->state.Faero[1] = 0;
  r->state.Faero[2] = 0;
  r->state.Maero[0] = 0;
  r->state.Maero[1] = 0;
  r->state.Maero[2] = 0;
      
  r->state.L_BV[0][0] = cos(theta)*cos(psi);
  r->state.L_BV[0][1] = cos(theta)*sin(psi);
  r->state.L_BV[0][2] =-sin(theta);
  r->state.L_BV[1][0] = sin(phi)*sin(theta)*cos(psi) - cos(phi)*sin(psi);
  r->state.L_BV[1][1] = sin(phi)*sin(theta)*sin(psi) + cos(phi)*cos(psi);
  r->state.L_BV[1][2] = sin(phi)*cos(theta);
  r->state.L_BV[2][0] = cos(phi)*sin(theta)*cos(psi) + sin(phi)*sin(psi);
  r->state.L_BV[2][1] = cos(phi)*sin(theta)*sin(psi) - sin(phi)*cos(psi);
  r->state.L_BV[2][2] = cos(phi)*cos(theta);
  
  init_cond[0]  = u;             /* velocity component in x */
  init_cond[1]  = v;             /* velocity component in y */
  init_cond[2]  = w;             /* velocity component in z */
  init_cond[3]  = P+ (r->state.L_BV[0][0]*WE*cos(lambda) - 
		      r->state.L_BV[0][2]*WE*sin(lambda));
  init_cond[4]  = Q+ (r->state.L_BV[1][0]*WE*cos(lambda) - 
		      r->state.L_BV[1][2]*WE*sin(lambda));
  init_cond[5]  = R+ (r->state.L_BV[2][0]*WE*cos(lambda) - 
		      r->state.L_BV[2][2]*WE*sin(lambda));
  init_cond[6]  = lambda;  /* latitude  */
  init_cond[7]  = mu;      /* longitude */
  init_cond[8]  = Re;      /* radius */
  init_cond[9]  = phi;     /* phi */
  init_cond[10] = theta;   /* theta */
  init_cond[11] = psi;     /* psi */

  time = 0;
  
  n = NUM_rkf (eom, neq, sol->dt, sol->duration, init_cond, ans,
               sol->precision, r);
  return n;
}
