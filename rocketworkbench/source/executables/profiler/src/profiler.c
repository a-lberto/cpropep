#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gpcp.h"
#include "libcompat/include/getopt.h"

#include "profiler.h"

#define M_PI2 9.869604401

Options rocket_options[] = {
  {"global",       PARENT,  NULL},
  {"length_units", STRING,  "global"},
  {"mass_units",   STRING,  "global"},
  
  {"rocket",       PARENT,  NULL},
  {"name",         STRING,  "rocket"},

  {"body",         PARENT,  "rocket"},
  {"type",         STRING,  "body"},
  {"length",       FLOAT,   "body"},
  {"diameter",     FLOAT,   "body"},
  {"density",      FLOAT,   "body"},
  
  {"fin",          PARENT,  "rocket"},
  {"num",          INTEGER, "fin"},
  {"pos",          FLOAT,   "fin"},
  {"root_chord",   FLOAT,   "fin"},
  {"tip_chord",    FLOAT,   "fin"},
  {"root_tip_d",   FLOAT,   "fin"},
  {"semispan",     FLOAT,   "fin"},
  {"body_radius",  FLOAT,   "fin"},
  
  {"motor",        PARENT,  "rocket"},
  {"zpos",         FLOAT,   "motor"},
  {"rpos",         FLOAT,   "motor"},
  {"diameter",     FLOAT,   "motor"},
  {"length",       FLOAT,   "motor"},
  {"empty_mass",   FLOAT,   "motor"},
  {"full_mass",    FLOAT,   "motor"},
  
  {"tank",         PARENT,  "rocket"},
  {"zpos",         FLOAT,   "tank"},
  {"rpos",         FLOAT,   "tank"},
  {"diameter",     FLOAT,   "tank"},
  {"length",       FLOAT,   "tank"},
  {"empty_mass",   FLOAT,   "tank"},
  {"full_mass",    FLOAT,   "tank"},
  
  {"other",        PARENT,  "rocket"},
  {"shape",        STRING,  "other"},
  {"zpos",         FLOAT,   "other"},
  {"rpos",         FLOAT,   "other"},
  {"Ix",           FLOAT,   "other"},
  {"Iy",           FLOAT,   "other"},
  {"mass",         FLOAT,   "other"},
  {NULL,           0,       NULL}
};

int read_value(Data *d, rocket_t *r);

int profile_rocket(rocket_t *r);
int profile_fins(rocket_t *r);
int profile_body(rocket_t *r);

void usage(void);

int main(int argc, char *argv[])
{
  int c;
  Data *data;
  rocket_t rocket;
  char filename[FILENAME_MAX] = "";

  rocket.body.n_body_parts = 0;
  rocket.body.body_parts = NULL;

  rocket.n_fin_set = 0;
  rocket.fin_set = NULL;

  rocket.Lr = 0;
  rocket.Ar = 0;

  while (1)
  {
    c = getopt(argc, argv, "f:");
    
    if (c == EOF)
      break;
    
    switch (c)
    {
      case 'f':
	if (strlen(optarg) > FILENAME_MAX)
        {
	  printf("Filename too long!\n");
	  return -1;
	  break;
	}
	strncpy (filename, optarg, FILENAME_MAX);
	break;
    
      case 'h':
	usage();
	break;
    } 
  }
  
  if (strcmp(filename, "") == 0)
  {
    printf("A rocket description file must be provide.\n");
    usage();
    return -1;
  }

  GPCP_RegisterOptions(rocket_options);   
  
  if (GPCP_ReadFile(filename, &data))
  {
    printf("Error parsing configuration file.\n");
    return -1;
  }
  
  read_value(data, &rocket);

  GPCP_FreeData(&data);

  profile_rocket(&rocket);

  return 0;
};


void usage(void)
{
  printf("Usage:");
  printf("\n\tprofiler -f infile");

  printf("\n\nArguments:\n");
  printf("-f file \t Perform an analysis of the specified rocket.\n");
  printf("-h      \t Print usage.\n");
}    




int profile_rocket(rocket_t *r)
{
  int n;


  r->Ar = M_PI * pow(r->Lr, 2)/4;

  profile_fins(r);
  profile_body(r);
  
  r->C_N_alpha = r->body.C_N_alpha_b;

  for (n = 0; n < r->n_fin_set; n++)
  {
    r->C_N_alpha += r->fin_set[n].C_N_alpha_tb;
    r->C_N_alpha += r->fin_set[n].C_N_alpha_bt;
  }

  printf("C_N_alpha    = % f\n", r->C_N_alpha);


  return 0;
}


int profile_fins(rocket_t *r)
{
  int n;  
  float cr, ct, rt, lt, xt, s;

  float beta;
  float lambda;
  float sac;  /* midchord sweep angle cosine */
  float tau;  /* (s + rt)/rt */
  float tau2; /* tau^2 */

  fin_t *f;

  /* fin treatment */
  for (n = 0; n < r->n_fin_set; n++)
  {
    f = r->fin_set + n;

    cr = f->cr;
    ct = f->ct;
    rt = f->rt;
    lt = f->lt;
    xt = f->xt;
    s  = f->s;

    f->Yt = rt + (s/3)*(cr + 2*ct)/(cr + ct);
    f->Xt = lt + (xt/3)*(cr + 2*ct)/(cr + ct) + 
      (cr + ct - cr*ct/(cr+ct))/6;

    f->Af = cr*s - xt*s/2 + (xt + ct - cr)*s/2;
    f->AR = pow(s,2)/f->Af;

    /* M is the mach number */
    beta = sqrt(1 - 0.5*0.5);
    
    /* cosine of the midchord sweep angle */
    sac = s/sqrt(pow(xt + ct - cr/2 - ct/2, 2) + pow(s, 2));

    /* formula 3-6 */
    f->C_N_alpha_1 = 2 * M_PI * f->AR * 
      (f->Af/r->Ar) / (2 + sqrt(4 + pow(beta*f->AR/sac, 2) ));

    /* formula 3-35 */
    f->C_l_delta = f->N * f->C_N_alpha_1 * f->Yt / r->Lr;
    
    lambda = ct / cr;
    
    /* formula 3-51 */
    f->C_l_p = -(f->N * cr * s / (6*pow(r->Lr, 2)))*
      ((1+3*lambda)*pow(s, 2) + 4*(1+2*lambda)*s*rt + 6*(1+lambda)*pow(rt, 2))*
      f->C_N_alpha_1;

    tau = (s + rt)/rt;
    tau2 = pow(tau, 2);
    
    /* formula 3-96 */
    f->Ktb_alpha = 2*((1 + pow(tau, -4))*(0.5*atan(0.5*(tau-1/tau))) - 
		      pow(tau, -2)*((tau-1/tau) + 2*atan(1/tau)))/
      (M_PI * pow(1-1/tau, 2));
    
    /*formula 3-98 */
    f->Kbt = pow((1 - 1/tau2)/(1 - 1/tau), 2) - f->Ktb_alpha;

    /* formula 3-105 */
    f->Ktb_delta =
      (M_PI2 * pow(1+1/tau, 2)/4 + 
       M_PI * pow( (tau2 + 1)/(tau2-tau), 2)*asin((tau2-1)/(tau2+1)) -
       2*M_PI*(tau+1)/(tau2 - tau) + 
       pow((tau2+1)/(tau2-tau), 2)*pow(asin((tau2-1)/(tau2+1)), 2) - 
       4*(tau+1)*asin((tau2-1)/(tau2+1))/(tau2-tau) + 
       8*log((tau2+1)/(2*tau))/pow(tau-1,2))/M_PI2;


    f->C_N_alpha_tb = f->N * f->C_N_alpha_1 * f->Ktb_alpha / 2;
    f->C_N_alpha_bt = f->N * f->C_N_alpha_1 * f->Kbt / 2;
    f->C_l_delta_tb = f->C_l_delta * f->Ktb_delta;

    printf("Ar = %f\n", r->Ar);
    printf("Yt = %f\n", f->Yt);
    printf("Xt = %f\n", f->Xt);
    printf("Af = %f\n", f->Af);
    printf("AR = %f\n", f->AR);
    printf("sac = %f\n", sac);
    printf("C_N_alpha_1  = % f\n", f->C_N_alpha_1);
    printf("C_l_delta    = % f\n", f->C_l_delta);
    printf("C_l_p        = % f\n", f->C_l_p);
    printf("Ktb_alpha    = % f\n", f->Ktb_alpha);
    printf("Kbt          = % f\n", f->Kbt);
    printf("Ktb_delta    = % f\n", f->Ktb_delta);
    printf("C_N_alpha_tb = % f\n", f->C_N_alpha_tb);
    printf("C_N_alpha_bt = % f\n", f->C_N_alpha_bt);
    printf("C_N_delta_tb = % f\n", f->C_l_delta_tb);

  }

  return 0;
}

int profile_body(rocket_t *r)
{
  int i, n;

  float fn;
  float dp; /* diameter of the previous part */

  body_part_t *part = NULL;

  /* body parts treatment */
  for (n = 0; n < r->body.n_body_parts; n++)
  {
    part = r->body.body_parts + n;

    switch (part->type)
    {

    case OGIVE_NOSE:
      fn = part->l / part->d;
      
      break;

    case ELLIPTIC_NOSE:

      break;

    case CONIC_NOSE:
      part->A = M_PI * part->d * sqrt(pow(part->d/2, 2) + pow(part->l, 2)) / 2;
      part->V = M_PI * pow(part->d/2, 2) * part->l / 3;
      break;

    case PARABOLIC_NOSE:

      break;

    case TUBE:
      part->A = M_PI * part->d * part->l;
      part->V = M_PI * pow(part->d, 2) * part->l / 4;
      break;

    case TRANSITION:
      dp = r->body.body_parts[n - 1].d;
      
      part->A = M_PI*(part->d*sqrt(pow(part->d/2, 2) + 
				   pow(part->l/(1 - part->d/dp), 2))/2 - 
		      dp*sqrt(pow(dp/2, 2) + 
			      pow(part->l*part->d/(dp*(1-part->d/dp)), 2))/2);

      part->V = M_PI * part->l * (pow(part->d/2, 2) - pow(dp/2, 2)*dp/part->d)/
	(3*(1 - dp/part->d));
      
      break;
    
    }
  }

  r->body.V  = 0;
  r->body.lo = 0;
  r->body.Ab = 0;
  for (i = 0; i < r->body.n_body_parts; i++)
  {
    part = r->body.body_parts + i; 
    r->body.V += part->V;
    r->body.lo += part->l;

    /* compute it only for the last part */
    r->body.Ab = M_PI * pow(part->d, 2) / 4;
  }

  /* formula 3-66 */
  r->body.C_N_alpha_b = 2 * r->body.Ab / r->Ar;
  r->body.Xb = r->body.lo - r->body.V/r->body.Ab;

  printf("Ab           = % f\n", r->body.Ab);
  printf("lo           = % f\n", r->body.lo);
  printf("C_N_alpha_b  = % f\n", r->body.C_N_alpha_b);
  printf("Xb           = % f\n", r->body.Xb);
  
  return 0;
}
