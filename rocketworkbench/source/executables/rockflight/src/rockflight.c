#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "libcompat/include/getopt.h"

#include "librockflight/include/state.h"
#include "librockflight/include/rockflight.h"
#include "libnum/include/num.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void usage(void);
extern void print_summary(rocket_t *rocket, float *init_cond, 
			  solution_t *solution);
extern void print_octave_results(double *ans, int neq, int n);

int eom(int neq, double time, double *y, double *dy, void *data);
int read_file(char *config_file, rocket_t *rocket, float *init_cond,
              solution_t *solution);
extern int simulate(rocket_t *r, solution_t *s, float *ic, double **ans);

int main(int argc, char *argv[])
{

  int n = 0, c;
  int neq = 12;
  
  /* hold the rocket properties */
  rocket_t rocket;
  solution_t solution;
  double *ans;
  float *init_cond;


  int debug = 0;
  int check_only = 0;
  
  int Lfileread = 0;
  char config_file[FILENAME_MAX];

  /* allocate memory to hold initial conditions */
  init_cond = (float *) malloc(sizeof(float) * neq);
  
  /* Read command line options */
  while (1)
  {
    c = getopt(argc, argv, "cdhf:");

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

      case 'c':
          check_only = 1;
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
  
  if (check_only)
    check(&rocket, 300.0, 1.0);
  else
    /* solve the model */
    n = simulate(&rocket, &solution, init_cond, &ans);
    
  fprintf(stderr, "Memory used: %d kB\n", n*(neq+1)*sizeof(double)/1024);

  print_octave_results(ans, neq, n);
  
  free(init_cond);
  free(ans);

  return 0;
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




