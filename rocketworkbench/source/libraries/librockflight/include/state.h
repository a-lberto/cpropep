
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

typedef struct engine
{
  float propellant_mass;
  float dry_mass;
  /*  float thrust; */
  int n_point;
  float *time;
  float *thrust;

  float c;
  float mass_flow;

  float start_time;
  float burn_time;
  float drop_time;

  float position[3];
  float direction[3];

} engine_t;

typedef struct rocket_properties
{
  float dry_mass;
  float Ix;
  float Iy;
  float Iz;
  float Cdrag;
  float Clift;
  float Cbeta;
  float Cspin;
  float Cmoment;
  float Cdamping;
  float Diameter;
  float drop_time;
  int n_engine;
  engine_t *engines;
} rocket_properties_t;

typedef struct rocket
{
  int n_stage;
  rocket_properties_t *stage_properties;
  state_t state;
} rocket_t;

typedef struct solution
{
  float duration;
  float dt;
  float precision;
} solution_t;

#endif /* __state_h__ */






