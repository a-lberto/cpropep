#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "libnum/include/num.h"
#include "libcpropep/include/equilibrium.h"
#include "libthermo/include/thermo.h"

#include "libmcp/include/mcp.h"
#include "libthermo/include/load.h"
#include "libcpropep/include/print.h"

#define NEQ 2

#define NPOINT 300
#define STEP 0.002

double Ab(double s);
double Vc(double s);

extern int modele(int neq, double time, double *y, double *dy, void *data);

int main(int argc, char *argv[])
{
  int i;
  
  data_t data;

  double temp;
  double tmp2[2];
  
  double **ans;
  double *ic; /* initial conditions */

  equilibrium_t *e;

  char thermo_file[FILENAME_MAX] = "../cpropep/data/thermo.dat";
  char propellant_file[FILENAME_MAX] = "../cpropep/data/propellant.dat";

//  global_verbose = 1;
  errorfile = stderr;
  outputfile = stdout;
  
  if (load_thermo (thermo_file) < 0)
  {
    printf("Unable to open %s\n", thermo_file);
    return -1;
  }
  if (load_propellant (propellant_file) < 0)
  {
    printf("Unable to open: %s\n", propellant_file);
    return -1;
  }
  
  e = (equilibrium_t *) malloc (sizeof (equilibrium_t) * 3);
  for (i = 0; i < 3; i++)
  {
    initialize_equilibrium(e + i);
  }
  add_in_propellant(e, 108, GRAM_TO_MOL(78, 766)); /* NH4ClO4 */
  add_in_propellant(e, 788, GRAM_TO_MOL(11, 788)); /* HTPB */
  add_in_propellant(e, 34,  GRAM_TO_MOL(7, 34));  /* ALU */

  compute_density(&(e->propellant));

  
  ic = (double *) malloc (sizeof(double) * NEQ);

  if ( (ans = (double **) malloc (sizeof(double *) * (NPOINT+1))) == NULL)
  {
    printf("Problem allocating memory\n");
    return -1;
  }
  for (i = 0; i < (NPOINT+1); i++)
  {
    ans[i] = (double *) malloc (sizeof(double) * NEQ);
  }

  ic[0] = 0;
  ic[1] = 5; /* atm */

//  data.a = 0.001804;
  data.a = 0.0016917;
  data.n = 0.35;
  data.ta = M_PI * pow(0.00595/2, 2);
  data.ea = M_PI * pow(0.00795/2, 2);
  data.Ab = Ab;
  data.Vc = Vc;
  data.e = e;
  
//  modele(2, 0, ic, tmp2, (void*)&data);
  
//  printf("%f %f\n", tmp2[0], tmp2[1]);
  
  /* find the solution */
  printf("%d\n", Round(1.0/0.001));

  
  NUM_rk4 (modele, NEQ, STEP, NPOINT*STEP, ic, ans, (void*)&data);

                       
  for (i = 0; i < NPOINT; i++)
  {
    printf("%.2e %.2e %.2e\n", i*STEP, ans[i][0], ans[i][1]);
  }
  printf("\n");
  
  
  free(ic);
  for (i = 0; i < (NPOINT+1); i++)
    free(ans[i]);
  free(ans);

  return 0;
}

double Ab(double s)
{
  double A;

  if ((s >= 0) && (s < 0.01265))
    A = 2 * M_PI * 0.09525 * (s + 0.00635);
  else
    A = 0;
  
  return A;
}

double Vc(double s)
{
  double V;
  
  if (s < 0)
    s = 0;
  else if (s > 0.01265)
    s = 0.01265;
  
  V = M_PI * 0.09525 * pow(s + 0.00635, 2); 

  return V;
}
