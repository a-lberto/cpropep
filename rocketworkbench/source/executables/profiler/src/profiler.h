#ifndef __profiler_h__
#define __profiler_h__

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct _fin_t
{
  /* fin description */
  double s;  /* semispan */
  double cr; /* root chord */
  double ct; /* tip chord */
  double rt; /* rocket radius where the fin is located */
  double lt; /* position of the leading edge from nose tip */
  double xt; /* position of the leading edge at tip chord relative to
		leading edge at root chord, in the x direction */
  int N;     /* number of fins in this set (usually 3 or 4) */

  /* calculation results */
  double Yt; /* Center of pressure Y position (radially) */
  double Xt; /* Center of pressure X position (from the nose tip) */

  double AR; /* aspect ratio */
  double Af; /* aera of one fin */
  double C_N_alpha_1; /* fin normal force coefficient derivative */
  double C_l_delta;   /* fin roll forcing moment coefficient derivative */
  double C_l_p;       /* tail roll damping moment derivative */

  double Ktb_alpha; /* interference factor of tail in presence of the body 
		     for C_N_alpha */
  double Ktb_delta; /* interference factor of tail in presence of the body 
		       for C_l_delta */
  double Kbt;

  double C_N_alpha_tb;
  double C_N_alpha_bt;
  double C_l_delta_tb;

} fin_t;

typedef enum
{
  OGIVE_NOSE,
  ELLIPTIC_NOSE,
  CONIC_NOSE,
  PARABOLIC_NOSE,
  TUBE,
  TRANSITION
} body_enum_t;

typedef struct _body_part_t
{
  /* part description */
  body_enum_t type; /* type of body part */
  double d;         /* diameter */
  double l;         /* length */

  /* calculation results */
  double A; /* area of the part */
  double V; /* volume of the part */

} body_part_t;

typedef struct _body_t
{
  int n_body_parts;
  body_part_t *body_parts;

  float lo; /* total length of the rocket */
  float V;  /* total volume of the body */
  float Ab; /* base area */
  float C_N_alpha_b; /* normal force coefficient for body */
  float Xb;

} body_t;


typedef struct _rocket_t
{
  char *name;
  float Lr; /* maximum diameter of the rocket */
  float Ar; /* reference area */
  
  body_t body;

  int n_fin_set;
  fin_t *fin_set;

  double C_N_alpha; /* total normal force coefficient derivative */
  double X; /* center of pressure location for the whole rocket */

} rocket_t;

#endif  __profiler_h__


