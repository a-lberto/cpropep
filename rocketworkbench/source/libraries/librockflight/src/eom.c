
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "aero.h"

#include "librockflight/include/rockflight.h"
#include "librockflight/include/state.h"
#include "atmos.h"
#include "engine.h"
#include "mass.h"
#include "stage.h"

#define SEC(THETA) (1./cos(THETA))
#define EARTH_RAD    6.3567668e6

int set_state(rocket_t *rocket, double *y, double *time);
int evaluate_function(function_t *f, state_t *s, double time, double *ans);

int eom(int neq, double time, double *y, double *dy, void *data)
{

  state_t *s;
  rocket_t *rocket = (rocket_t *) data;
  
  double u      = y[0];  /* velocity component in x */
  double v      = y[1];  /* velocity component in y */
  double w      = y[2];  /* velocity component in z */ 
  double p      = y[3];  /* angular velocity */
  double q      = y[4];  /* */
  double r      = y[5];  /* */
  double lambda = y[6];  /* latitude */
/*double mu     = y[7];*//* longitude */
  double Re     = y[8];  /* distance from earth center */
  double phi    = y[9];  /* euler angles */
  double theta  = y[10]; /* */
/*double psi    = y[11];*//* */
  
 
  /* aerodynamic forces */
  double X = 0.0;
  double Y = 0.0;
  double Z = 0.0;

  /* moments */
  double L = 0.0;
  double M = 0.0;
  double N = 0.0;

  /* set the pointer to the rocket state structure */
  s = &(rocket->state);

  set_state(rocket, y, &time);

      
  X += s->Faero[0]; /*printf("X = %f\n", s->Faero[0]);*/
  Y += s->Faero[1]; /*printf("Y = %f\n", s->Faero[1]);*/
  Z += s->Faero[2]; /*printf("Z = %f\n", s->Faero[2]);*/

  L += s->Maero[0]; /*printf("L = %f\n", s->Maero[0]);*/
  M += s->Maero[1]; /*printf("M = %f\n", s->Maero[1]);*/
  N += s->Maero[2]; /*printf("N = %f\n", s->Maero[2]);*/
  
  /* add the thrust of the engine */
  X += s->Feng[0];
  Y += s->Feng[1];
  Z += s->Feng[2];

  L += s->Meng[0];
  M += s->Meng[1];
  N += s->Meng[2];
  
  /* acceleration of the rocket in the body axis */
  dy[0] = X/s->m - s->g*sin(theta)          - s->Cx - (s->qE + q)*w +
    (s->rE + r)*v;
  dy[1] = Y/s->m + s->g*cos(theta)*sin(phi) - s->Cy - (s->rE + r)*u +
    (s->pE + p)*w;
  dy[2] = Z/s->m + s->g*cos(theta)*cos(phi) - s->Cz - (s->pE + p)*v +
    (s->qE + q)*u;

  /* angular acceleration equation */
  dy[3] = (L + (s->Iy - s->Iz)*q*r)/s->Ix;
  dy[4] = (M + (s->Iz - s->Ix)*r*p)/s->Iy;
  dy[5] = (N + (s->Ix - s->Iy)*p*q)/s->Iz;

  /* position relative to earth */
  dy[6] = (s->L_VB[0][0]*u + s->L_VB[0][1]*v+s->L_VB[0][2]*w)/Re;
  dy[7] = (s->L_VB[1][0]*u + s->L_VB[1][1]*v+s->L_VB[1][2]*w)/(Re*cos(lambda));
  dy[8] =-(s->L_VB[2][0]*u + s->L_VB[2][1]*v+s->L_VB[2][2]*w);

  /* relative angular velocity of vehicle relative to atmosphere */
  s->P = p - (s->L_BV[0][0]*(WE + dy[7])*cos(lambda) -
           s->L_BV[0][1]*dy[6] - s->L_BV[0][2]*(WE + dy[7])*sin(lambda));
  s->Q = q - (s->L_BV[1][0]*(WE + dy[7])*cos(lambda) -
           s->L_BV[1][1]*dy[6] - s->L_BV[1][2]*(WE + dy[7])*sin(lambda));
  s->R = r - (s->L_BV[2][0]*(WE + dy[7])*cos(lambda) -
           s->L_BV[2][1]*dy[6] - s->L_BV[2][2]*(WE + dy[7])*sin(lambda));

 
  /* euler angle equation */
  /* we have to check for division by zero */
  if (abs(cos(theta)) > 0.001)
  {
    dy[9]  = s->P + sin(phi)*tan(theta)*s->Q + cos(phi)*tan(theta)*s->R;
    dy[10] =                 cos(phi)  *s->Q -          sin(phi)  *s->R;
    dy[11] =        sin(phi)*SEC(theta)*s->Q + cos(phi)*SEC(theta)*s->R;
  }
  else
  {
    dy[9]  = s->P;
    dy[10] = cos(phi)*s->Q - sin(phi)*s->R;
    dy[11] = 0;
  }

  
  return 0;
}


int set_state(rocket_t *rocket, double *y, double *time)
{
  state_t *s;
  function_t *f;
  
  /*double u      = y[0];*/  /* velocity component in x */
  /*double v      = y[1];*/  /* velocity component in y */
  /*double w      = y[2];*/  /* velocity component in z */ 
  /*double p      = y[3];*/  /* angular velocity */
  /*double q      = y[4];*/  /* */
  /*double r      = y[5];*/  /* */
  double lambda = y[6];  /* latitude */
  /* double mu     = y[7];*/  /* longitude */
  double Re     = y[8];  /* distance from earth center */
  double phi    = y[9];  /* euler angles */
  double theta  = y[10]; /* */
  double psi    = y[11]; /* */
  
  s = &(rocket->state);

  /* transformation matrix (vector in vehicle carried reference to
     body reference */
  s->L_BV[0][0] =  cos(theta)*cos(psi);
  s->L_BV[0][1] =  cos(theta)*sin(psi);
  s->L_BV[0][2] = -sin(theta);
  s->L_BV[1][0] =  sin(phi)*sin(theta)*cos(psi) - cos(phi)*sin(psi);
  s->L_BV[1][1] =  sin(phi)*sin(theta)*sin(psi) + cos(phi)*cos(psi);
  s->L_BV[1][2] =  sin(phi)*cos(theta);
  s->L_BV[2][0] =  cos(phi)*sin(theta)*cos(psi) + sin(phi)*sin(psi);
  s->L_BV[2][1] =  cos(phi)*sin(theta)*sin(psi) - sin(phi)*cos(psi);
  s->L_BV[2][2] =  cos(phi)*cos(theta);

  /* Inverse of the previous matrix */
  s->L_VB[0][0] = s->L_BV[0][0];
  s->L_VB[0][1] = s->L_BV[1][0];
  s->L_VB[0][2] = s->L_BV[2][0];
  s->L_VB[1][0] = s->L_BV[0][1];
  s->L_VB[1][1] = s->L_BV[1][1];
  s->L_VB[1][2] = s->L_BV[2][1];
  s->L_VB[2][0] = s->L_BV[0][2];
  s->L_VB[2][1] = s->L_BV[1][2];
  s->L_VB[2][2] = s->L_BV[2][2];

  /* Earth angular velocity vector expressed in body reference */
  s->pE = (s->L_BV[0][0]*cos(lambda) - s->L_BV[0][2]*sin(lambda)) * WE;
  s->qE = (s->L_BV[1][0]*cos(lambda) - s->L_BV[1][2]*sin(lambda)) * WE;
  s->rE = (s->L_BV[2][0]*cos(lambda) - s->L_BV[2][2]*sin(lambda)) * WE;

  /* Centripetal acceleration */
  s->Cx = (s->L_BV[0][0]*Re*cos(lambda)*sin(lambda) +
           s->L_BV[0][2]*Re*cos(lambda)*cos(lambda)) * WE2;
  s->Cy = (s->L_BV[1][0]*Re*cos(lambda)*sin(lambda) +
           s->L_BV[1][2]*Re*cos(lambda)*cos(lambda)) * WE2;
  s->Cz = (s->L_BV[2][0]*Re*cos(lambda)*sin(lambda) +
           s->L_BV[2][2]*Re*cos(lambda)*cos(lambda)) * WE2;

  s->alt = Re - EARTH_RAD; 
  
  /* gravitational model */
  s->g = UGC*ME/(Re*Re);

  /* find the actual active stage */
  if (stage(rocket, time))
    return -1;

  /* set the inertia of the rocket */
  f = &(rocket->stage_properties[s->s].Ix);
  evaluate_function(f, s, *time, &(s->Ix));
  
  f = &(rocket->stage_properties[s->s].Iy);
  evaluate_function(f, s, *time, &(s->Iy));

  f = &(rocket->stage_properties[s->s].Iz);
  evaluate_function(f, s, *time, &(s->Iz));
  
  f = &(rocket->stage_properties[s->s].Cd);
  evaluate_function(f, s, *time, &(s->Cd));

  f = &(rocket->stage_properties[s->s].CL);
  evaluate_function(f, s, *time, &(s->CL));

  f = &(rocket->stage_properties[s->s].CB);
  evaluate_function(f, s, *time, &(s->CB));
  
  /* compute rocket mass */
  if (mass(rocket, time))
    return -1;

  /* compute aerodynamic forces and moments */
  if (aero(rocket, y, time))
    return -1;

  /* compute enginethrust */
  if (engine(rocket, time))
    return -1;

  return 0;
}












