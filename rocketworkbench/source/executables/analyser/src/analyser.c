#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

/* Numerical method library */
#include "num.h"
/* Config file parser library */
#include "gpcp.h"
/* Conversion library */
#include "convert.h"

#include "analyser.h"

#define MAX_LINE 100

Options analyser_options[] = {
  {"datafile",        STRING,  NULL},
  {"format",          PARENT,  NULL},
  {"column",          INTEGER, "format"},
  {"time",            PARENT,  "format"},
  {"col",             INTEGER, "time"},
  {"units",           STRING,  "time"},
  {"thrust",          PARENT,  "format"},
  {"col",             INTEGER, "thrust"},
  {"units",           STRING,  "thrust"},
  {"comments",        STRING,  "format"},
  {"motor",           PARENT,  NULL},
  {"propellant_mass", PARENT,  "motor"},
  {"val",             FLOAT,   "propellant_mass"},
  {"units",           STRING,  "propellant_mass"},
  {"total_mass",      PARENT,  "motor"},
  {"val",             FLOAT,   "total_mass"},
  {"units",           STRING,  "total_mass"},
  {NULL,              0,       NULL}
};
        
int main(int argc, char *argv[])
{
  int n, c;
  
  char *datafile;      /* Name of the datafile */
  double *data = NULL; /* Array containing the data read in the data file */

  format_t format;
  motor_t   motor;
  results_t results;
  
  /* Name of the config file */
  char filename[FILENAME_MAX] = "\0";

  GPCP_RegisterOptions(analyser_options);
  
  /* Read the command line arguments */
  while (1)
  {
    c = getopt(argc, argv, "f:");
    if (c == EOF)
      break;
    switch (c)
    {
      case 'f':
          strncpy(filename, optarg, FILENAME_MAX);
          break;
    }
  }
  
  if (filename[0] =='\0')
  {
    printf("A config file must be specified.\n");
    return -1;
  }
  
  if (parse_config(filename, &datafile, &format, &motor))
    return 1;
  
  if (parse_datafile(datafile, &data, &n, &format))
    return 1;
  
  analyse_data(data, n, &results, &format, &motor);
  print_results(&results);  
  return 0;
}

void print_results(results_t *res)
{
  printf("Results of thrust data analysis.\n");
  printf("--------------------------------\n");
  printf("Maximum thrust           : %.2f N\n", res->max_thrust);
  printf("Total impulse            : %.2f Ns\n", res->impulse);
  printf("Average thrust           : %.2f N\n", res->avg_thrust);
  printf("Burn time                : %.2f s\n", res->burn_time);
  printf("Exhaust velocity         : %.2f m/s\n", res->vg);
  printf("Specific impulse         : %.2f s\n", res->isp);
  printf("Propellant mass fraction : %.2f \n", res->prop_mass_fraction);
  printf("Impulse to weight ratio  : %.2f \n", res->impulse_to_weight);
  // I will have to invert thrust and impulse in the definition
  printf("Motor classification     : %c-%d\n",
         'A' + (char) (log(res->max_thrust/2.5)/log(2)) + 1,
         (int) res->impulse);
}
    
int analyse_data(double *data, int n, results_t *res, format_t *f, motor_t *m)
{
  int i;
  double max;
  double integral;
  
  /* Calculation based on those data */
  //double max_thrust;         /* Maximum thrust */
  //double avg_thrust;         /* Mean thrust */
  //double burn_time;          /* Burning time */
  //double impulse;            /* Total impulse */
  //double vg;                 /* Exhaust velocity */
  //double isp;                /* Specific impulse */
  //double prop_mass_fraction; /* Propellant mass fraction */
  //double impulse_to_weight;  /* Impulse to weight ratio */

  memset(res, 0x00, sizeof(results_t));
         
  max = 0;
  for (i = 0; i < n; i++)
  {
    max = ((data[1 + i*f->col] > max) ? data[1 + i*f->col] : max);
  }
  
  res->max_thrust = max;
  res->burn_time = data[0 + (n - 1)*f->col];
  
  simpson(data, n, 2, 1, &integral);
  
  res->impulse = integral;
  res->avg_thrust = res->impulse / res->burn_time;

  if (m->isprop)
  {
    res->vg = res->impulse / m->prop_mass;
    res->isp = res->vg / 9.80665;
  }
  if (m->isprop && m->istot)
  {
    res->prop_mass_fraction = m->prop_mass / m->total_mass;
    res->impulse_to_weight = res->impulse / (m->total_mass * 9.80665);
  }
  
  return 0;
}

int parse_datafile(char *datafile, double **data, int *ndata, format_t *f)
{
  FILE *inputfile;
  
  int i, n;
  
  char *token;
  char line[MAX_LINE];
  
  double *tmp;
  
  if ((inputfile = fopen(datafile, "r")) == NULL)
  {
    printf("Unable to open datafile.\n");
    return -1;
  }
  
  tmp = (double *) malloc (sizeof(double) * f->col);
  
  /* Reading file */
  n = 0;
  while (fgets(line, MAX_LINE, inputfile) != NULL)
  {
    if (line[0] != f->comments[0]) /* comments */
    {
      token = strtok(line, " \t");

      if (!isdigit(token[0])) /* no number on this line, skip... */
        continue;

      tmp[0] = atof(token);

      for (i = 1; i < f->col; i++)
      {
        token = strtok(NULL, " \t");
        if (!isdigit(token[0]))  /* error, there is missing number
                                    on this line */
          return -1;

        tmp[i] = atof(token);
      }

      /* reallocating the memory */
      *data = (double *) realloc(*data, sizeof(double) * 2 * (n + 1));
      
      (*data)[0 + n*f->col] = tmp[f->time - 1] * f->sctime;
      (*data)[1 + n*f->col] = tmp[f->thrust - 1] * f->scthrust;
      n++;
    }
  }
  *ndata = n;

  fclose(inputfile);
  
  return 0;
}

int parse_config(char *configfile, char **datafile, format_t *f, motor_t *m)
{
  char *tmp;
  float number;
  
  Data *config;
  
  /* read the file */
  if (GPCP_ReadFile(configfile, &config) != 0)
  {
    printf("Error reading config file.\n");
    return 1;
  }
  
  /* Use config data (we must provide string length*/
  if (GPCP_GetValue ("datafile", datafile) != 0)
  {
    printf("A 'datafile' must be specified.\n");
    return 1;
  }
  if (GPCP_EnterLevel("format", 0) != 0)
  {
    printf("The file format must be specified.\n");
    return 1;
  }
  else
  {
    if (GPCP_GetValue ("column", &f->col) != 0)
    {
      printf("Unable to get number of column.\n");
      return 1;
    }
    if (GPCP_GetValue ("comments", &f->comments) != 0)
    {
      printf("No 'comments' sign specified: assuming '#'.\n");
      strcpy(f->comments, "#");
    }
    if (GPCP_EnterLevel("time", 0) == -1)
    {
      printf("Unable to get time information.\n");
      return 1;
    }
    else
    {
      if (GPCP_GetValue ("col", &f->time) != 0)
      {
        printf("Unable to get time column index.\n");
        return 1;
      }
      if (GPCP_GetValue ("units", &tmp) != 0)
      {
        printf("Unable to get units.\n");
        return 1;
      }
      f->sctime = 1;
      free(tmp);
      GPCP_ExitLevel();
    }
    if (GPCP_EnterLevel("thrust", 0) == -1)
    {
      printf("Unable to get thrust information.\n");
      return 1;
    }
    else
    {
      if (GPCP_GetValue ("col", &f->thrust) != 0)
      {
        printf("Unable to get time column index.\n");
        return 1;
      }
      if (GPCP_GetValue ("units", &tmp) != 0)
      {
        printf("Unable to get units.\n");
        return 1;
      }
      
      f->scthrust = 1.0;
      convert("force", &(f->scthrust), tmp, "N");      
      free(tmp);
      
      GPCP_ExitLevel();
    }
    GPCP_ExitLevel();
  }
  
  m->isprop = 0;
  m->istot  = 0;
  
  if (GPCP_EnterLevel("motor", 0) != 0)
  {
    printf("No motor information specified.\n");
  }
  else
  {
    if (GPCP_EnterLevel("propellant_mass", 0) == -1)
    {
      printf("No propellant mass specified.\n");
    }
    else
    {
      if (GPCP_GetValue ("val", &number) != 0)
      {
        printf("Unable propellant mass.\n");
        return 1;
      }
      if (GPCP_GetValue ("units", &tmp) != 0)
      {
        printf("Unable to get units.\n");
        return 1;
      }

      m->prop_mass = (double) number;
      if (convert("mass", &(m->prop_mass), tmp, "kg"))
      {
        printf("Incorrect units: %s\n", tmp);
        return -1;
      }
      free(tmp);

      m->isprop = 1;
      
      GPCP_ExitLevel();
    }
    if (GPCP_EnterLevel("total_mass", 0) == -1)
    {
      printf("No total mass specified.\n");
    }
    else
    {
      if (GPCP_GetValue ("val", &number) != 0)
      {
        printf("Unable total mass.\n");
        return 1;
      }
      if (GPCP_GetValue ("units", &tmp) != 0)
      {
        printf("Unable to get units.\n");
        return 1;
      }
      
      m->total_mass = (double) number;
      if (convert("mass", &(m->total_mass), tmp, "kg"))
      {
        printf("Incorrect units: %s\n", tmp);
        return -1;
      }
      free(tmp);
      m->istot = 1;
      
      GPCP_ExitLevel();
    }
    GPCP_ExitLevel();
  }
      
  GPCP_FreeData(&config);
  return 0;
}

