
#ifndef __state_h__
#define __state_h__

typedef double real;

typedef real scalar_t;
typedef real vector_t[3];
typedef real matrix_t[3][3];

typedef enum
{
  _CONSTANT,
  _FUNCTION,
  _TABLE
} val_t;

typedef enum
{
  ALT,
  MACH,
  AOA,
  TIME
} parameter_t;

typedef struct _function
{
  char *name;
  val_t type;

  /* if type is _CONSTANT */
  float constant_value;

  /* if type is _TABLE */
  int n_point;
  parameter_t independant_var;
  float *x, *y;
  
  /* if type is _FUNCTION */
  char *function;
  
} function_t;

typedef struct state
{
  int s; /* active stage */
  
  scalar_t m; /* mass of the rocket */
  scalar_t g; /* gravitational acceleration */

  scalar_t Ix;
  scalar_t Iy;
  scalar_t Iz;

  /* */
  scalar_t alt;
  scalar_t aoa;
  scalar_t mach;
  
  /* aerodynamic coefficients */
  scalar_t Cd;
  scalar_t CL;
  scalar_t CB;
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

  function_t thrust;
  
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

  function_t Ix;
  function_t Iy;
  function_t Iz;
  
  function_t Cd;
  function_t CL;
  function_t CB;

  float Cspin;
  float Cmoment;
  float Cdamping;
  
  float Diameter;
  
  float active_time;
  
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






