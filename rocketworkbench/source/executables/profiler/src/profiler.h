#ifndef __profiler_h__
#define __profiler_h__

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct _fin_t
{
  int N;     /* number of fins in this set (usually 3 or 4) */

  /* fin description */
  double s;  /* semispan */
  double cr; /* root chord */
  double ct; /* tip chord */
  double rt; /* rocket radius where the fin is located */
  double lt; /* position of the leading edge from nose tip */
  double xt; /* position of the leading edge at tip chord relative to
		leading edge at root chord, in the x direction */
  
  /* calculation results */
  double Yt; /* Center of pressure Y position (radially) */
  double Xt; /* Center of pressure X position (from the nose tip) */
  double Xbt; /* Center of pressure X position with body interferance */

  double AR; /* aspect ratio */
  double Af; /* aera of one fin */

  /* the C_N_alpha_1 value is a function of the mach number.
     the values will evaluate for some discrete value of the mach
     number and stored in an array for table lookup.
  */
  
  double *C_N_alpha_1; /* fin normal force coefficient derivative for each
			mach number */


  /* these value should be multiply by C_N_alpha_1 to get the coefficient */
  double C_l_delta_m;  /* fin roll forcing moment coefficient derivative 
			  multiplier*/
  double C_l_p_m;      /* tail roll damping moment derivative multiplier */

  double Ktb_alpha; /* interference factor of tail in presence of the body 
		     for C_N_alpha */
  double Ktb_delta; /* interference factor of tail in presence of the body 
		       for C_l_delta */
  double Kbt;
  double Krb;       /* roll damping interference factor */

  /* these should also be multiply by C_N_alpha_1 to get the coefficients */
  double C_N_alpha_tb_m;
  double C_N_alpha_bt_m;
  double C_l_delta_tb_m;
  double C_l_p_tb_m;

  /* mass and inertial properties */
  double m;
  double Xcg;
  double Ixx;
  double Iyy;
  
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
  double m;  /* mass of the part */

  /* calculation results */
  double A;  /* area of the part */
  double V;  /* volume of the part */

  double Xof; /* distance of the beginning of the part from nose tip */
  double Xcg;
  /* moments of inertia about axis passing trough the cg of the part */
  double Ix;
  double Iy; /* Iy = Iz for body parts (symmetry of rotation) */ 


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

  int n_mach; /* number of points in the values table */
  double *mach;

  float Lr;  /* maximum diameter of the rocket */
  float Ar;  /* reference area */
  float m;   /* total mass of the rocket */
  float Xcg; /* position of the center of gravity */

  body_t body;
  int n_fin_set;
  fin_t *fin_set;


  /* calculated values (table as a function of the mach  number) */

  double *X;         /* center of pressure location for the whole rocket */
  double *C_N_alpha; /* total normal force coefficient derivative */
  double *C_mq;      /* pitch damping moment coefficient derivative */
  double *C_m_alpha; /* pitch forcing moment coefficient derivative */

  /* ???? should they be here */
  double C_l_p;
  double C_l_delta;

} rocket_t;

#endif  __profiler_h__


