#ifndef __profiler_h__
#define __profiler_h__

typedef struct _fin_t
{
  /* fin description */
  double s; /* semispan */
  double cr; /* root chord */
  double ct; /* tip chord */
  double rt; /* rocket radius where the fin is located */
  double lt; /* position of the leading edge from nose tip */
  int N;     /* number of fins in this set (usually 3 or 4) */

  /* calculation results */
  double Yt; /* Center of pressure Y position (radially) */
  double Xt; /* Center of pressure X position (from the nose tip) */

  double AR; /* aspect ratio */
  double Af; /* aera of one fin */
  double C_N_alpha_1; /* fin normal force coefficient derivative */
  double C_l_delta;   /* fin roll forcing moment coefficient derivative */
  double C_l_p;       /* tail roll damping moment derivative */

} fin_t;

typedef struct _body_t
{

} body_t;

typedef struct _rocket_t
{
  int n_body_parts;
  body_t *body_parts;
  
  int n_fin_set;
  fin_t *fin_set;

} rocket_t;

#endif  __profiler_h__


