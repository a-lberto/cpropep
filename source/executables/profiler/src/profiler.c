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
  {"cg",           FLOAT,   "rocket"},

  {"body",         PARENT,  "rocket"},
  {"type",         STRING,  "body"},
  {"length",       FLOAT,   "body"},
  {"diameter",     FLOAT,   "body"},
  {"density",      FLOAT,   "body"},
  {"mass",         FLOAT,   "body"},
  
  {"fin",          PARENT,  "rocket"},
  {"num",          INTEGER, "fin"},
  {"pos",          FLOAT,   "fin"},
  {"root_chord",   FLOAT,   "fin"},
  {"tip_chord",    FLOAT,   "fin"},
  {"root_tip_d",   FLOAT,   "fin"},
  {"semispan",     FLOAT,   "fin"},
  {"body_radius",  FLOAT,   "fin"},
  {"mass",         FLOAT,   "fin"},
  
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

  /* GPCP_PrintData(data); */

  GPCP_FreeData(&data);

  profile_rocket(&rocket);

  free (rocket.fin_set);
  free (rocket.body.body_parts);

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
  int i, n;
  fin_t *f;

  r->m = 0;
  /* reference area of the rocket */
  r->Ar = M_PI * pow(r->Lr, 2)/4;
  
  r->n_mach = 20;
  r->mach = (double *) malloc(sizeof(double) * r->n_mach);

  /* fill the table of the mach number */
  for (i = 0; i < r->n_mach; i++)
  {
    r->mach[i] = (1.0/r->n_mach) * i;
  }

  profile_fins(r);
  profile_body(r);

  /* tables in function of the mach number */
  r->X         = (double *) malloc(sizeof(double) * r->n_mach);
  r->C_N_alpha = (double *) malloc(sizeof(double) * r->n_mach);
  r->C_mq      = (double *) malloc(sizeof(double) * r->n_mach);
  r->C_m_alpha = (double *) malloc(sizeof(double) * r->n_mach);

  for (i = 0; i < r->n_mach; i++)
  {

    r->C_N_alpha[i] = r->body.C_N_alpha_b;

    /* formula 3-106 */
    for (n = 0; n < r->n_fin_set; n++)
    {
      f = r->fin_set + n;

      r->C_N_alpha[i] += f->C_N_alpha_tb_m * f->C_N_alpha_1[i];
      r->C_N_alpha[i] += f->C_N_alpha_bt_m * f->C_N_alpha_1[i];
    }

    /* rocket center of pressure location */
    r->X[i] = r->body.Xb * r->body.C_N_alpha_b;
    /* formula 3-107 */
    for (n = 0; n < r->n_fin_set; n++)
    {
      f = r->fin_set + n;
      r->X[i] += f->Xt  * f->C_N_alpha_tb_m * f->C_N_alpha_1[i];
      r->X[i] += f->Xbt * f->C_N_alpha_bt_m * f->C_N_alpha_1[i];
    }
    r->X[i] = r->X[i] / r->C_N_alpha[i];

    /* formula 3-115 CHECKME, formula was badly print, 
       i redevelop it myself */
    r->C_mq[i] = 0;
    for (n = 0; n < r->n_fin_set; n++)
    {
      f = r->fin_set + n;
      
      r->C_mq[i] += 2 * f->C_N_alpha_tb_m *  f->C_N_alpha_1[i] *
        pow((f->Xt - r->Xcg)/r->Lr, 2);
    }

    r->C_m_alpha[i] = 0;
    for (n = 0; n < r->n_fin_set; n++)
    {
      f = r->fin_set + n;
      r->C_m_alpha[i] += f->C_N_alpha_tb_m *  f->C_N_alpha_1[i] *
        (r->X[i] - r->Xcg) / r->Lr;
    }

    /*
      printf("Aerodynamic properties of the entire rocket.\n"
      "--------------------------------------------\n");
      printf("Reference area (Ar) = % f m2\n", r->Ar);
      
      printf("C_N_alpha    = % f\n", r->C_N_alpha);
      printf("X            = % f\n", r->X);
      printf("Cmq          = % f\n", r->C_mq);
    */
  
  }

  for (i = 0; i < r->n_mach; i++)
  {
    printf("M = %.2f -> C_N_alpha = %.3f, Cmq = %.3f, Cm_alpha = %.3f,"
           " X = %.2f\n",
           r->mach[i], r->C_N_alpha[i], r->C_mq[i], r->C_m_alpha[i], r->X[i]);
  }

  return 0;
}


int profile_fins(rocket_t *r)
{
  int i, n;  
  float cr, ct, rt, lt, xt, s;

  float beta;
  float lambda;
  float sac;  /* midchord sweep angle cosine */
  float tau;  /* (s + rt)/rt */
  float tau2; /* tau^2 */
  float tau3;

  fin_t *f;

  /* fin treatment */
  for (n = 0; n < r->n_fin_set; n++)
  {
    f = r->fin_set + n;

    f->C_N_alpha_1 = (double *) malloc(sizeof(double) * r->n_mach);

    cr = f->cr;
    ct = f->ct;
    rt = f->rt;
    lt = f->lt;
    xt = f->xt;
    s  = f->s;

    /* add fin mass to the rocket mass */
    r->m += f->N * f->m;

    /* inertial properties */
    f->Xcg = 0;
    f->Ixx = 0;
    f->Iyy = 0;
    
    /* center of pressure of the fins */
    /* formula 3-27 */
    f->Yt = rt + (s/3)*(cr + 2*ct)/(cr + ct);
    /* formula 3-30 */
    f->Xt = lt + (xt/3)*(cr + 2*ct)/(cr + ct) + 
      (cr + ct - cr*ct/(cr+ct))/6;

    /* formula 3-101 */
    f->Xbt = lt + cr/4 + 
      ((sqrt(s*s - rt*rt)*acosh(s/rt) - s + M_PI*rt/2)/
       (rt*acosh(s/rt)/sqrt(s*s - rt*rt) + s/rt - M_PI/2) - rt) * 
      (xt + 0.25*(ct-cr))/s; /* check this part (tan(sweepback)) */
    
    /* fin area */
    f->Af = cr*s - xt*s/2 + (xt + ct - cr)*s/2;
    
    /* fin aspect ratio */
    f->AR = pow(s,2)/f->Af;

    /* cosine of the midchord sweep angle */
    sac = s/sqrt(pow(xt + ct - cr/2 - ct/2, 2) + pow(s, 2));

    for (i = 0; i < r->n_mach; i++)
    {
      /* M is the mach number */
      beta = sqrt(1 - pow(r->mach[i], 2));

      /* formula 3-6 */
      f->C_N_alpha_1[i] = 2 * M_PI * f->AR * 
        (f->Af/r->Ar) / (2 + sqrt(4 + pow(beta*f->AR/sac, 2) ));
    }

    /* formula 3-35 */
    f->C_l_delta_m = f->N * f->Yt / r->Lr; /* *f->C_N_alpha_1 */
    
    lambda = ct / cr;
    
    /* formula 3-51 */
    f->C_l_p_m = -(f->N * cr * s / (6*pow(r->Lr, 2)))*
      ((1+3*lambda)*pow(s, 2) + 4*(1+2*lambda)*s*rt + 6*(1+lambda)*pow(rt, 2));
    /* * f->C_N_alpha_1;*/

    tau = (s + rt)/rt;
    tau2 = pow(tau, 2);
    tau3 = pow(tau, 3);

    /* formula 3-96 */
    f->Ktb_alpha = 2*((1 + pow(tau, -4))*(0.5*atan(0.5*(tau-1/tau))) - 
                      pow(tau, -2)*((tau-1/tau) + 2*atan(1/tau)))/
      (M_PI * pow(1-1/tau, 2));
    
    /*formula 3-98 */
    f->Kbt = pow((1 - 1/tau2)/(1 - 1/tau), 2) - f->Ktb_alpha;

    /* formula 3-105 */
    /* CHECK THIS :  when optimize with -O2 or -O3 in gcc, the value is nan
       but is good with -O ???? */
    f->Ktb_delta =
      (M_PI2 * pow(1+1/tau, 2)/4 + 
       M_PI * pow( (tau2 + 1)/(tau2-tau), 2)*asin((tau2-1)/(tau2+1)) -
       2*M_PI*(tau+1)/(tau2 - tau) + 
       pow((tau2+1)/(tau2-tau), 2)*pow(asin((tau2-1)/(tau2+1)), 2) - 
       4*(tau+1)*asin((tau2-1)/(tau2+1))/(tau2-tau) + 
       8*log((tau2+1)/(2*tau))/pow(tau-1,2))/M_PI2;

    /* formula 3-122 */
    f->Krb = 1 + ((tau-lambda)/tau - (1-lambda)*log(tau)/(tau-1))/
      ((tau+1)*(tau-lambda)/2 - (1-lambda)*(tau3-1)/(3*(tau-1)));
      

    f->C_N_alpha_tb_m = f->N * f->Ktb_alpha / 2; /* * f->C_N_alpha_1 */
    f->C_N_alpha_bt_m = f->N * f->Kbt / 2;       /* * f->C_N_alpha_1 */
    f->C_l_delta_tb_m = f->C_l_delta_m * f->Ktb_delta;
    f->C_l_p_tb_m     = f->C_l_p_m * f->Krb;

    printf("Aerodynamic properties of the fin set number %d\n"
           "-----------------------------------------------\n", n+1);
    printf("Fin area (Af)      = % f m2\n", f->Af);
    printf("Aspect ratio (AR)  = % f\n", f->AR);
    printf("\n");

    printf("Interference factors.\n");
    printf("Ktb_alpha    = % f\n", f->Ktb_alpha);
    printf("Kbt          = % f\n", f->Kbt);
    printf("Ktb_delta    = % f\n", f->Ktb_delta);
    printf("Krb          = % f\n", f->Krb);
    printf("\n");

    printf("Fin center of pressure location.\n");
    printf("Yt  = % .3f m\n", f->Yt);
    printf("Xt  = % .3f m\n", f->Xt);
    printf("Xbt = % .3f m\n",  f->Xbt);
    printf("\n");
    
    printf("Fin stability coefficients.\n");
    for (i = 0; i < r->n_mach; i++)
    {
      printf("M = % .2f -> C_N_alpha_1 = % .4f\n", 
             r->mach[i], f->C_N_alpha_1[i]);
    }

    printf("C_l_delta_m    = % f\n", f->C_l_delta_m);
    printf("C_l_p_m        = % f\n", f->C_l_p_m);
    printf("\n");

    printf("Fin stability coefficients with interference factors.\n");
    printf("C_N_alpha_tb_m = % f\n", f->C_N_alpha_tb_m);
    printf("C_N_alpha_bt_m = % f\n", f->C_N_alpha_bt_m);
    printf("C_N_delta_tb_m = % f\n", f->C_l_delta_tb_m);
    printf("C_l_p_tb_m     = % f\n", f->C_l_p_tb_m);
  }

  return 0;
}

int profile_body(rocket_t *r)
{
  int i, n;

  float fn;
  float dp; /* diameter of the previous part */

  body_part_t *previous_part = NULL;
  body_part_t *part = NULL;

  /* body parts treatment */
  for (n = 0; n < r->body.n_body_parts; n++)
  {
    part = r->body.body_parts + n;
    if (n != 0)
      previous_part = r->body.body_parts + n - 1;

    r->m += part->m;

    switch (part->type)
    {

      case OGIVE_NOSE:
          fn = part->l / part->d;

          part->A = 0;
          part->V = 0;
          part->Xof = 0;
          part->Xcg = 0.466 * part->l;
          part->Ix = 0;
          part->Iy = 0;
      
          break;

      case ELLIPTIC_NOSE:

          
          break;

      case CONIC_NOSE:
          part->A = M_PI * part->d *
            sqrt(pow(part->d/2, 2) + pow(part->l, 2)) / 2;

          part->V = M_PI * pow(part->d/2, 2) * part->l / 3;

          part->Xof = 0;
          part->Xcg = 2 * part->l / 3;

          part->Ix = part->m * pow(part->d, 2) / 8;
          /* moment of inertia about the axis passing at the tip of the cone */
          part->Iy = part->m * (pow(part->d, 2)/16 + pow(part->l, 2)/2);
          break;

      case PARABOLIC_NOSE:

          break;

      case TUBE:
          part->A = M_PI * part->d * part->l;
          part->V = M_PI * pow(part->d, 2) * part->l / 4;

          if (previous_part == NULL)
            return -1;
          part->Xof = previous_part->Xof + previous_part->l;
          part->Xcg = part->Xof + part->l / 2;

          part->Ix = part->m * pow(part->d, 2) / 4;
          part->Iy = part->m * (pow(part->d, 2)/8 + pow(part->l, 2)/12);
          break;

      case TRANSITION:
          dp = r->body.body_parts[n - 1].d;
      
          part->A = M_PI*(part->d*sqrt(pow(part->d/2, 2) + 
                                       pow(part->l/(1 - part->d/dp), 2))/2 - 
                          dp*sqrt(pow(dp/2, 2) + 
                                  pow(part->l*part->d/(dp*(1-part->d/dp)), 2))/2);

          part->V = M_PI * part->l *
            (pow(part->d/2, 2) - pow(dp/2, 2)*dp/part->d)/(3*(1 - dp/part->d));

          /*part->Ix = ;*/

          break;
      default:
          printf("Unknown type : %d\n", part->type);
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

    printf("Part geometric characteristics.\n");
    printf("Volume = %e m3\n", part->V);
    printf("Area   = %e m2\n", part->A);
    printf("Mass   = %e kg\n", part->m);
    printf("Ixx    = %e kg-m2\n", part->Ix);
    printf("Iyy    = %e kg-m2\n", part->Iy);
    printf("Xof    = %e m\n", part->Xof);
    printf("Xcg    = %e m\n", part->Xcg);
  }

  /* formula 3-66 */
  r->body.C_N_alpha_b = 2 * r->body.Ab / r->Ar;
  r->body.Xb = r->body.lo - r->body.V/r->body.Ab;

  printf("\n");
  printf("Aerodynamic properties of the body.\n"
         "-----------------------------------\n");
  printf("Ab           = % f\n", r->body.Ab);
  printf("lo           = % f\n", r->body.lo);
  printf("C_N_alpha_b  = % f\n", r->body.C_N_alpha_b);
  printf("Xb           = % f\n", r->body.Xb);
  
  return 0;
}






