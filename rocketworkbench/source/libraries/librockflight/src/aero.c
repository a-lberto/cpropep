

#include <stdio.h>
#include <math.h>

#include "aero.h"

#include "atmos.h"
#include "librockflight/include/state.h"

#define EARTH_RAD    6.3567668e6

/* Compute aerodynamic forces and moments */
int aero(rocket_t *r, double *y, double *t)
{

  state_t *s;

  double q; /* dynamic pressure */

  double u      = y[0];  /* velocity component in x */
  double v      = y[1];  /* velocity component in y */
  double w      = y[2];  /* velocity component in z */ 
/*  double p      = y[3];*/  /* angular velocity */
/*  double q      = y[4];*/  /* */
/*  double r      = y[5];*/  /* */
/*  double lambda = y[6];*/  /* latitude */
/*  double mu     = y[7];*/  /* longitude */
  double Re     = y[8];  /* distance from earth center */
/*  double phi    = y[9];*/  /* euler angles */
/*  double theta  = y[10];*/ /* */
/*  double psi    = y[11];*/ /* */

  double X = 0;
  double Y = 0;
  double Z = 0;
  
  double FD  = 0; /* Drag */
  double FL  = 0; /* Lift */
  double FB  = 0; /* sideslip force */

/*  double SL = 0;*/ /* Cross spin */
/*  double SB = 0;*/ /* Cross spin */

  /* restoring moments */
  double ML = 0;
  double MM = 0;
  double MN = 0;

  /* damping moments */
  double MG = 0;
  double MH = 0;
  double MI = 0;

  AIR air;

  /* double A;  reference aera */
  /* double C;  reference length */

  double V;     /* airspeed */
  double alpha; /* angle of attack */
  double beta;  /* angle of sideslip */

  s = &(r->state);

    /* compute atmospheric properties */
  atmosphere(&air, Re - EARTH_RAD);
  /*printf("T = %f, P = %f, rho = %f, c = %f\n", air.T, air.P, air.rho, air.c);*/
  
  V = sqrt(u*u + v*v + w*w);

  if (V == 0.0)
    return 0;
  
  alpha = atan(w/u);
  beta  = asin(v/V);

 /* A = M_PI * s->D * S->D / 4; */
/*  S = S->D; */

  q = 0.5 * air.rho * V * V;

  FD = - s->Cdrag * q * s->A;
  FL = - s->Clift * q * s->A * alpha;
  FB = - s->Cbeta * q * s->A * beta;

  /*printf("FD = %f, FL = %f\n", FD, FL);*/
  
  ML = 0;
  MG = 0; /*s->C_l_p * q * s->A * pow(s->C, 2) * s->P / (2 * u); */
    
  MM = s->Cmoment  * q * s->A * s->C * alpha;
  MH = s->Cdamping * q * s->A * pow(s->C, 2) * s->Q / (2 * u);

  /*printf("MM = %f, MH = %f\n", MM, MH);*/
  
  MN = s->Cmoment  * q * s->A * s->C * beta;
  MI = s->Cdamping * q * s->A * pow(s->C, 2) * s->R / (2 * u);
  
  /* transform wind axis forces to body axis forces */
  X = FD*cos(alpha)*cos(beta) - FB*cos(alpha)*sin(beta) - FL*sin(alpha);
  Y = FD*sin(beta)            + FB*cos(beta);
  Z = FD*sin(alpha)*cos(beta) - FB*sin(alpha)*sin(beta) + FL*cos(alpha);

  
  s->Faero[0] = X;
  s->Faero[1] = Y;
  s->Faero[2] = Z;

  s->Maero[0] = ML + MG;
  s->Maero[1] = MM + MH;
  s->Maero[2] = MN + MI;
  
  return 0;
}




