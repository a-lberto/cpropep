#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/*#ifdef GCC
# include <unistd.h>
#else
*/
#include "libcompat/include/getopt.h"

/*#endif*/


#include "librockflight/include/state.h"
#include "librockflight/include/rockflight.h"
#include "libnum/include/num.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void usage(void);
void print_summary(rocket_t *rocket, float *init_cond, solution_t *solution);
void print_octave_results(double *ans, int neq, int n);

int eom(int neq, double time, double *y, double *dy, void *data);
int read_file(char *config_file, rocket_t *rocket, float *init_cond,
              solution_t *solution);
int simulate(rocket_t *r, solution_t *s, float *ic, double **ans);

int main(int argc, char *argv[])
{

  int n, c;
  int neq = 12;
  
  /* hold the rocket properties */
  rocket_t rocket;
  solution_t solution;
  double *ans;
  float *init_cond;


  int debug = 0;
  int Lfileread = 0;
  char config_file[FILENAME_MAX];

  /* allocate memory to hold initial conditions */
  init_cond = (float *) malloc(sizeof(float) * neq);
  
  /* Read command line options */
  while (1)
  {
    c = getopt(argc, argv, "dhf:");

    if (c == EOF)
      break;
    
    switch (c)
    {
      /* the input file */
      case 'f':
          if (strlen(optarg) > FILENAME_MAX)
          {
            printf("Filename too long!\n");
            break;
          }
          strncpy (config_file, optarg, FILENAME_MAX - 1);
          Lfileread = 1;
          break;

      case 'd':
          debug = 1;
          break;
          
      case 'h':
          usage();
          return 0;
          break;
          
      default:
          usage();
          break;
    }
  }

  if (! (Lfileread))
  {
    printf("Error: A config file must be specified.\n");
    printf("       try rockflight -h for options.\n");
    abort();
  }

  /* parse the configuration file */
  if ((read_file (config_file, &rocket, init_cond, &solution)) != 0)
    abort();

  print_summary(&rocket, init_cond, &solution);
  
  /* return 0; */
  
  /* solve the model */
  n = simulate(&rocket, &solution, init_cond, &ans);
    
  fprintf(stderr, "Memory used: %d kB\n", n*(neq+1)*sizeof(double)/1024);
  print_octave_results(ans, neq, n);
  
  free(init_cond);
  free(ans);

  return 0;
}


int simulate(rocket_t *r, solution_t *sol, float *ic, double **ans)
{

  int n;
  
  double time;
  float init_cond[12];

  int neq = 12;
    
  
/*  s.A = M_PI*s.D*s.D/4;*/ /* reference area (m^2) */
/*  s.C = 1;*/    /* reference distance */

  double u      = ic[0];
  double v      = ic[1];
  double w      = ic[2];
  double P      = ic[3];
  double Q      = ic[4];
  double R      = ic[5];
  double lambda = ic[6];
  double mu     = ic[7];
  double Re     = ic[8] + EARTH_RAD; /* convert altitude to distance 
					from eartj center */
  double phi    = ic[9];
  double theta  = ic[10]*(M_PI/180); /* convert deg to rad 
					(use convert instead) */
  double psi    = ic[11];


  /* in kg */
  r->state.m = 16.0; 

  r->state.Ix = 0.0025;
  r->state.Iy = 0.84;
  r->state.Iz = 0.84;
  
  /* aerodynamic coefficients */
  
  r->state.Cdrag    = 0.2;
  r->state.Clift    = 2.3;
  r->state.Cbeta    = 2.3;
  r->state.Cspin    = 20.0;
  r->state.Cmoment  = 6.32;
  r->state.Cdamping = 31.6;
  
  r->state.D = 0.1143;
  
  r->state.Faero[0] = 0;
  r->state.Faero[1] = 0;
  r->state.Faero[2] = 0;
  r->state.Maero[0] = 0;
  r->state.Maero[1] = 0;
  r->state.Maero[2] = 0;
      
  r->state.L_BV[0][0] = cos(theta)*cos(psi);
  r->state.L_BV[0][1] = cos(theta)*sin(psi);
  r->state.L_BV[0][2] =-sin(theta);
  r->state.L_BV[1][0] = sin(phi)*sin(theta)*cos(psi) - cos(phi)*sin(psi);
  r->state.L_BV[1][1] = sin(phi)*sin(theta)*sin(psi) + cos(phi)*cos(psi);
  r->state.L_BV[1][2] = sin(phi)*cos(theta);
  r->state.L_BV[2][0] = cos(phi)*sin(theta)*cos(psi) + sin(phi)*sin(psi);
  r->state.L_BV[2][1] = cos(phi)*sin(theta)*sin(psi) - sin(phi)*cos(psi);
  r->state.L_BV[2][2] = cos(phi)*cos(theta);
  
  init_cond[0]  = u;             /* velocity component in x */
  init_cond[1]  = v;             /* velocity component in y */
  init_cond[2]  = w;             /* velocity component in z */
  init_cond[3]  = P+ (r->state.L_BV[0][0]*WE*cos(lambda) - 
		      r->state.L_BV[0][2]*WE*sin(lambda));
  init_cond[4]  = Q+ (r->state.L_BV[1][0]*WE*cos(lambda) - 
		      r->state.L_BV[1][2]*WE*sin(lambda));
  init_cond[5]  = R+ (r->state.L_BV[2][0]*WE*cos(lambda) - 
		      r->state.L_BV[2][2]*WE*sin(lambda));
  init_cond[6]  = lambda;  /* latitude  */
  init_cond[7]  = mu;      /* longitude */
  init_cond[8]  = Re;      /* radius */
  init_cond[9]  = phi;     /* phi */
  init_cond[10] = theta;   /* theta */
  init_cond[11] = psi;     /* psi */

  time = 0;
  
  n = NUM_rkf (eom, neq, sol->dt, sol->duration, init_cond, ans,
               sol->precision, r);
  return n;
}

void print_octave_results(double *ans, const int neq, const int n)
{
  int i;
  
  printf("# Created by rocflight\n");
  printf("# name: M\n");
  printf("# type: matrix\n");
  printf("# rows: %d\n", n);/*(int)(duration/dt) + 1);*/
  printf("# columns: %d\n", neq + 1);

  
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
  
}

void usage(void)
{
  printf("Usage:");
  printf("\n\trockflight -f infile [-o outfile] [-d]");
  printf("\n\trockflight -h");

  printf("\n\nArguments:\n");
  printf("-f file \t Perform a simulation using file.\n");
  printf("-o file \t Results file, stdout if omitted\n");
  printf("-d      \t Debug mode\n");
  printf("-h      \t Print help\n\n");
}





void print_summary(rocket_t *rocket, float *init_cond, solution_t *solution)
{
  int i, j;
  double propellant_mass = 0.0;
  double rocket_mass = 0.0;
  
  printf("---------------------------------------------\n");
  printf("             SOLUTION SUMMARY\n");
  printf("---------------------------------------------\n");
  printf("Solution duration  : %.2f s\n", (*solution).duration);
  printf("Solution dt        : %g \n", (*solution).dt);
  printf("Solution precision : %.2e \n", (*solution).precision);
  printf("\n");
  printf("Initial conditions.\n");
  printf("-------------------\n");
  printf("  u         = %.2e m/s\n", init_cond[0]);
  printf("  v         = %.2e m/s\n", init_cond[1]);
  printf("  w         = %.2e m/s\n", init_cond[2]);
  printf("  P         = %.2e rad/s\n", init_cond[3]);
  printf("  Q         = %.2e rad/s\n", init_cond[4]);
  printf("  R         = %.2e rad/s\n", init_cond[5]);
  printf("  longitude = %.2e deg\n", init_cond[6]);
  printf("  latitude  = %.2e deg\n", init_cond[7]);
  printf("  altitude  = %.2e m\n", init_cond[8]);
  printf("  phi       = %.2e deg\n", init_cond[9]);
  printf("  theta     = %.2e deg\n", init_cond[10]);
  printf("  psi       = %.2e deg\n", init_cond[11]);
  printf("\n");
  printf("Rocket properties.\n");
  printf("------------------\n");
  printf("  Number of stages : %d\n", (*rocket).n_stage);

  for (i = 0; i < (*rocket).n_stage; i++)
  {
       /* engines contribution */
       for (j = 0; j < (*rocket).stage_properties[i].n_engine; j++)
       {
	    propellant_mass += 
		 (*rocket).stage_properties[i].engines[j].propellant_mass;
	    rocket_mass += 
		 (*rocket).stage_properties[i].engines[j].dry_mass;
       }
       /* stage structure and payload */
       rocket_mass += (*rocket).stage_properties[i].dry_mass;
  }

  printf("  Propellant mass : %.2e kg\n", propellant_mass);
  printf("  Rocket dry mass : %.2e kg\n", rocket_mass);
  printf("  Total mass      : %.2e kg\n", propellant_mass + rocket_mass);
  printf("\n");
  printf("  Propellant mass fraction : %.2f %%\n",
         100*propellant_mass/(propellant_mass + rocket_mass));
  printf("\n");

  printf("Stages properties.\n");
  printf("------------------\n");

  for (i = 0; i < (*rocket).n_stage; i++)
  {
    printf("  Stage %d\n", i+1);
    printf("  |->Diameter               : %.2e m\n",
           (*rocket).stage_properties[i].Diameter);
    printf("  |->Dry mass               : %.2e kg\n",
           (*rocket).stage_properties[i].dry_mass);
    printf("  |->Ix                     : %.2e kg\n",
           (*rocket).stage_properties[i].Ix);
    printf("  |->Iy                     : %.2e kg\n",
           (*rocket).stage_properties[i].Iy);
    printf("  |->Iz                     : %.2e kg\n",
           (*rocket).stage_properties[i].Iz);
    printf("  |->Drop time              : %.2e s\n",
           (*rocket).stage_properties[i].drop_time);

/*    printf("   Aerodynamic properties.\n"); */
    printf("  |->Drag coefficient       : %.2e\n",
           (*rocket).stage_properties[i].Cdrag);
    printf("  |->Lift coefficient       : %.2e\n",
           (*rocket).stage_properties[i].Clift);
    printf("  |->Beta coefficient       : %.2e\n",
           (*rocket).stage_properties[i].Cbeta);
    printf("  |->Cross spin coefficient : %.2e\n",
           (*rocket).stage_properties[i].Cspin);
    printf("  |->Moment coefficient     : %.2e\n",
           (*rocket).stage_properties[i].Cmoment);
    printf("  |->Damping coefficient    : %.2e\n",
           (*rocket).stage_properties[i].Cdamping);

    printf("  |->Number of engines      : %d\n",
	   (*rocket).stage_properties[i].n_engine);
    for (j = 0; j < (*rocket).stage_properties[i].n_engine; j++)
    {
	 printf("  |->Engine %d\n", j);
	 printf("     |->Propellant mass    : %.2e\n",
		(*rocket).stage_properties[i].engines[j].propellant_mass);
	 printf("     |->Dry mass           : %.2e\n",
		(*rocket).stage_properties[i].engines[j].dry_mass);
	 printf("     |->Start time         : %.2e\n",
		(*rocket).stage_properties[i].engines[j].start_time);
	 printf("     |->Burn time          : %.2e\n",
		(*rocket).stage_properties[i].engines[j].burn_time);
	 printf("     |->Drop time          : %.2e\n",
		(*rocket).stage_properties[i].engines[j].drop_time);
	 printf("     |->Position           : (%.2e, %.2e, %.2e)\n",
		(*rocket).stage_properties[i].engines[i].position[0],
		(*rocket).stage_properties[i].engines[i].position[1],
		(*rocket).stage_properties[i].engines[i].position[2]);
	 printf("     |->Direction          : (%.2e, %.2e, %.2e)\n",
		(*rocket).stage_properties[i].engines[i].direction[0],
		(*rocket).stage_properties[i].engines[i].direction[1],
		(*rocket).stage_properties[i].engines[i].direction[2]);
    }

    printf("\n"); 
  }
  printf("---------------------------------------------\n");
  printf("                END SUMMARY\n");
  printf("---------------------------------------------\n");
}
