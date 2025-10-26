
#ifndef __state_h__
#define __state_h__

typedef double real;

typedef real scalar_t;
typedef real vector_t[3];
typedef real matrix_t[3][3];


typedef struct state
{
  scalar_t m;
  scalar_t g;

  scalar_t Ix;
  scalar_t Iy;
  scalar_t Iz;

  /* aerodynamic coefficients */
  scalar_t Clift;
  scalar_t Cbeta;
  scalar_t Cdrag;
  scalar_t Cspin;
  scalar_t Cmoment;
  scalar_t Cdamping;

  scalar_t D; /* Diameter of the rocket */
  scalar_t A; /* reference area (m^2) */
  scalar_t C; /* reference distance */
  
  /* aerodynamic forces */
  vector_t Faero; /* (X, Y, Z) */  
  /* moments */
  vector_t Maero; /* (L, M, N) */

  /* engine forces and moments */
  vector_t Feng;
  vector_t Meng;
  
  /* component of centripetal acceleration in F_B */
  scalar_t Cx;
  scalar_t Cy;
  scalar_t Cz;

  /* earth angular velocity in F_B */
  scalar_t pE;
  scalar_t qE;
  scalar_t rE;

  /* relative angular velocity of F_B to F_V */
  scalar_t P;
  scalar_t Q;
  scalar_t R;

  matrix_t L_BV;
  matrix_t L_VB;
  
} state_t;

#endif /* __state_h__ */






