#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "rocflight.h"

#include "num.h"
#include "state.h"

int eom(int neq, double time, double *y, double *dy, void *data);

int main(int argc, char *argv[])
{

  double epsilon = 1e-4;
  
  int i, n;
  int neq = 12;
 
  double time;
  //double res[12];

  double *ans;
  double *init_cond;

  double u      = 100;  /* velocity component in x */
  double v      = 0;  /* velocity component in y */
  double w      = 10;  /* velocity component in z */
  
  double P      = 0;  /* angular velocity */
  double Q      = 0;  /* */
  double R      = 0;  /* */

  double lambda = 0*(M_PI/180);   /* latitude (positive north) */
  double mu     = 0*(M_PI/180);   /* longitude (positive east) */
  double Re     = EARTH_RAD + 5000;      /* distance from earth center */
  double phi    = 0*(M_PI/180);   /*  */
  double theta  = 80*(M_PI/180);   /* positive up */
  double psi    = 0*(M_PI/180); /* positive cw*/


  double dt = 0.01;
  double duration = 5;

  state_t s;
  s.m = 16.0; /* in kg */

  s.Ix = 0.0025;
  s.Iy = 0.84;
  s.Iz = 0.84;

  /* aerodynamic coefficients */
  s.Cdrag    = 0.2;
  s.Clift    = 2.3;
  s.Cbeta    = 2.3;
  s.Cspin    = 20.0;
  s.Cmoment  = 6.32;
  s.Cdamping = 31.6;

  s.D = 0.1143;
//  s.A = M_PI*s.D*s.D/4; /* reference area (m^2) */
//  s.C = 1;    /* reference distance */

  s.Faero[0] = 0;
  s.Faero[1] = 0;
  s.Faero[2] = 0;

  s.Maero[0] = 0;
  s.Maero[1] = 0;
  s.Maero[2] = 0;
    
  init_cond = (double *) malloc(sizeof(double) * neq);
  
  s.L_BV[0][0] = cos(theta)*cos(psi);
  s.L_BV[0][1] = cos(theta)*sin(psi);
  s.L_BV[0][2] =-sin(theta);
  s.L_BV[1][0] = sin(phi)*sin(theta)*cos(psi) - cos(phi)*sin(psi);
  s.L_BV[1][1] = sin(phi)*sin(theta)*sin(psi) + cos(phi)*cos(psi);
  s.L_BV[1][2] = sin(phi)*cos(theta);
  s.L_BV[2][0] = cos(phi)*sin(theta)*cos(psi) + sin(phi)*sin(psi);
  s.L_BV[2][1] = cos(phi)*sin(theta)*sin(psi) - sin(phi)*cos(psi);
  s.L_BV[2][2] = cos(phi)*cos(theta);
  
  init_cond[0]  = u;             /* velocity component in x */
  init_cond[1]  = v;             /* velocity component in y */
  init_cond[2]  = w;             /* velocity component in z */
  init_cond[3]  = P+ (s.L_BV[0][0]*WE*cos(lambda)-s.L_BV[0][2]*WE*sin(lambda));
  init_cond[4]  = Q+ (s.L_BV[1][0]*WE*cos(lambda)-s.L_BV[1][2]*WE*sin(lambda));
  init_cond[5]  = R+ (s.L_BV[2][0]*WE*cos(lambda)-s.L_BV[2][2]*WE*sin(lambda));
  init_cond[6]  = lambda;  /* latitude  */
  init_cond[7]  = mu;      /* longitude */
  init_cond[8]  = Re;      /* radius */
  init_cond[9]  = phi;     /* phi */
  init_cond[10] = theta;   /* theta */
  init_cond[11] = psi;     /* psi */

  time = 0;
  
//  eom(neq, time, init_cond, res, &s);
/*
  fprintf(stderr, "% e (u dot)\n", res[0]);
  fprintf(stderr, "% e (v dot)\n", res[1]);
  fprintf(stderr, "% e (w dot)\n", res[2]);
  fprintf(stderr, "% e (p dot)\n", res[3]);
  fprintf(stderr, "% e (q dot)\n", res[4]);
  fprintf(stderr, "% e (r dot)\n", res[5]);
  fprintf(stderr, "% e (lambda dot)\n", res[6]);
  fprintf(stderr, "% e (mu dot)\n", res[7]);
  fprintf(stderr, "% e (R dot)\n", res[8]);
  fprintf(stderr, "% e (phi dot)\n", res[9]);
  fprintf(stderr, "% e (theta dot)\n", res[10]);
  fprintf(stderr, "% e (psi dot)\n", res[11]);
*/

  //n = NUM_rk4 (eom, neq, dt, duration, init_cond, &ans, &s);

  n = NUM_rkf (eom, neq, dt, duration, init_cond, &ans, epsilon, &s);

  fprintf(stderr, "Memory used: %d kB\n", n*(neq+1)*sizeof(double)/1024);
  
  printf("# Created by rocflight\n");
  printf("# name: M\n");
  printf("# type: matrix\n");
  printf("# rows: %d\n", n);//(int)(duration/dt) + 1);
  printf("# columns: %d\n", neq + 1);

  
  //for( i = 0; i < n; i++)
  //{
  //  printf("% .12e ", ans[0 + neq*i]); /* u */
  //  printf("% .12e ", ans[1 + neq*i]); /* v */
  //  printf("% .12e ", ans[2 + neq*i]); /* w */
  //  printf("% .12e ", ans[3 + neq*i]); /* p */
  //  printf("% .12e ", ans[4 + neq*i]); /* q */
  //  printf("% .12e ", ans[5 + neq*i]); /* r */
  //  printf("% .12e ", ans[6 + neq*i]); /* lambda */
  //  printf("% .12e ", ans[7 + neq*i]); /* mu */
  //  printf("% .12e ", ans[8 + neq*i]); /* R */
  //  printf("% .12e ", ans[9 + neq*i]); /* phi */
  //  printf("% .12e ", ans[10 + neq*i]); /* theta */
  //  printf("% .12e \n", ans[11 + neq*i]); /* psi */
  //}

  
  for( i = 0; i < n; i++)
  {
    printf("% .12e ", ans[12 + (neq+1)*i]); /* t */
    printf("% .12e ", ans[0  + (neq+1)*i]); /* u */
    printf("% .12e ", ans[1  + (neq+1)*i]); /* v */
    printf("% .12e ", ans[2  + (neq+1)*i]); /* w */
    printf("% .12e ", ans[3  + (neq+1)*i]); /* p */
    printf("% .12e ", ans[4  + (neq+1)*i]); /* q */
    printf("% .12e ", ans[5  + (neq+1)*i]); /* r */
    printf("% .12e ", ans[6  + (neq+1)*i]); /* lambda */
    printf("% .12e ", ans[7  + (neq+1)*i]); /* mu */
    printf("% .12e ", ans[8  + (neq+1)*i]); /* R */
    printf("% .12e ", ans[9  + (neq+1)*i]); /* phi */
    printf("% .12e ", ans[10 + (neq+1)*i]); /* theta */
    printf("% .12e ", ans[11 + (neq+1)*i]); /* psi */
    printf("\n");
  }

  
  free(init_cond);
  //free(ans);

  return 0;
}
