#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "thermo.h"
#include "load.h"


int global_verbose = 0;

#define MAX_MOL 100

typedef enum _unit_
{
  DIMENSIONLESS,
  SPECIFIC,
  MOLAR
} units_t;


typedef struct _prop_
{
  char   symb[3];
  char   unit_1[12];
  char   unit_2[12];
  char   title[24];
  double (*f)(int n, double T);
} prop_t;

typedef enum _value_
{
  CP = 0,
  H = 1,
  U = 2,
  S = 3
} value_t;

double SpecificHeat(int n, double T);
double Enthalpy(int n, double T);
double InternalEnergy(int n, double T);
double Entropy(int n, double T);

prop_t properties[] = {
  {"cp", "Cp/R", "kJ/%s-K", "Specific Heat (Cp)", SpecificHeat},
  {"h", "H/RT", "kJ/%s", "Enthalpy", Enthalpy},
  {"u", "U/RT", "kJ/%s", "Internal Energy", InternalEnergy},
  {"s", "S/R", "kJ/%s-K", "Entropy", Entropy},
  {"\0", "\0", "\0", "\0", NULL}
};

double SpecificHeat(int n, double T)
{
  return specific_heat_0(n, T);
}

double Enthalpy(int n, double T)
{
  return enthalpy_0(n, T) -
    ((thermo_list + n)->heat - (thermo_list + n)->dho)/(R*T);
}
double InternalEnergy(int n, double T)
{
  return  (enthalpy_0(n, T) - ((thermo_list + n)->heat -
                               (thermo_list + n)->dho)/(R*T) - 1);
}
double Entropy(int n, double T)
{
  return entropy_0(n, T);
}

int main(int argc, char *argv[])
{

  FILE *fdres;
  FILE *fdplot;
  
  double T;
  int i, c, n;

  int num_mol;
  int mol[MAX_MOL];
  
  char thermo_file[FILENAME_MAX] = "../data/thermo.dat";
  char propellant_file[FILENAME_MAX] = "../data/propellant.dat";

  value_t val;
  units_t units;

  double Ti, Tf, step;

  char temp[128];
  
  char *ptr;
  char *token;
  char *running;

  bool _molec = false;
  bool _value = false;
  bool _units = false;
  bool _range = false;
    
  
  if (load_thermo (thermo_file) < 0)
  {
    printf("Error loading thermo data file: %s\n", thermo_file);
    return -1;
  }
  if (load_propellant (propellant_file) < 0)
  {
    printf("Error loading propellant file: %s\n", propellant_file);
    return -1;
  }
  if ((fdres = fopen("result.m", "w")) == NULL)
  {
    printf("Error opening file.\n");
    return -1;
  }
  if ((fdplot = fopen("plotres.m", "w")) == NULL)
  {
    printf("Error opening file.\n");
    return -1;
  }
  
  num_mol = 0;
  
  while (1)
  {
    c = getopt(argc, argv, "m:p:u:T:");

    if (c == EOF)
      break;

    switch (c)
    {
      /* the output file */
      case 'm':
          /* duplicate the string */
        if ((running = strdup(optarg)) == NULL)
        {
          printf("Not enough memory.\n");
          return -1;
        }
        ptr = running;
        token = strsep(&running, "-");

        /* get each molecule */
        while (token != NULL)
        {  
          mol[num_mol] = atoi(token);
          num_mol++;
          token = strsep(&running, "-");
        }
        free(ptr);

        _molec = true;
        
        break;
          
      case 'p':
          if (strcmp(optarg, "cp") == 0)
            val = CP;
          else if (strcmp(optarg, "h") == 0)
            val = H;
          else if (strcmp(optarg, "u") == 0)
            val = U;
          else if (strcmp(optarg, "s") == 0)
            val = S;
          else
          {
            printf("Unknown properties: %s\n", optarg);
            break;
          }

          _value = true;
          break;
          
      case 'u':
          if (strcmp(optarg, "dimensionless") == 0)
            units = DIMENSIONLESS;
          else if (strcmp(optarg, "specific") == 0)
            units = SPECIFIC;
          else if (strcmp(optarg, "molar") == 0)
            units = MOLAR;
          else
          {
            printf("Unknown units: %s\n", optarg);
            break;
          }
          _units = true;
          break;
          
      case 'T':
             /* duplicate the string */
        if ((running = strdup(optarg)) == NULL)
        {
          printf("Not enough memory.\n");
          return -1;
        }
        ptr = running;
        if ((token = strsep(&running, ":")) == NULL)
        {
          printf("Not enought arguments to temperature.\n");
          break;
        }
        Ti = atof(token);
        if ((token = strsep(&running, ":")) == NULL)
        {
          printf("Not enought arguments to temperature.\n");
          break;
        }  
        step = atof(token);
        if ((token = strsep(&running, ":")) == NULL)
        {
          printf("Not enought arguments to temperature.\n");
          break;
        }
        Tf = atof(token);
        
        free(ptr);

        _range = true;
        break;
          
    }
  }

  /*
  for (i = 0; i < num_mol; i++)
  {
    printf("%d\n", mol[i]);
  }
  printf("%d\n", val);
  printf("%d\n", units);
  printf("%f - %f - %f\n", Ti, step, Tf);
  return 0;
  */

  if (!(_molec == _value == _units == _range == true))
  {
    printf("Missing arguments. Aborted.\n");
    return -1;
  }

  fprintf(fdplot, "result;\n");
  fprintf(fdplot, "hold on;\n");
  fprintf(fdplot, "gset grid\n");
  fprintf(fdplot, "gset title \"%s as a function of Temperature in the standard state\"\n",
          properties[val].title);
  fprintf(fdplot, "gset xlabel \"Temperature (K)\"\n");

  if (units == DIMENSIONLESS)
  {
    fprintf(fdplot, "gset ylabel \"%s\"\n", properties[val].unit_1);
  }
  else
  {
    sprintf(temp, "gset ylabel \"%s\"\n", properties[val].unit_2);
    if (units == SPECIFIC)
    {
      fprintf(fdplot, temp, "kg");
    }
    else
    {
      fprintf(fdplot, temp, "mol");
    }
  }


  for (i = 0; i < num_mol; i++)
  {
    n = mol[i];
 
    fprintf(fdres, "M%d = [\n", n);

    fprintf(fdplot, "plot(M%d(:,1),M%d(:,2), ';%s;')\n", n, n,
            (thermo_list + n)->name);
    
    switch (units)
    {
      case DIMENSIONLESS:
          for (T = Ti; T < Tf; T += step)
          {
            fprintf(fdres, "%.1f, %.3f,\n", T, properties[val].f(n, T));
          }
          break;
          
      case SPECIFIC:
          if ((val == CP) || (val == S))
          {
            for (T = Ti; T < Tf; T += step)
            {
              fprintf(fdres, "%.1f, %.3f,\n", T,
                      properties[val].f(n, T)*R/(thermo_list + n)->weight);
            }
          }
          else
          {
            for (T = Ti; T < Tf; T += step)
            {
              fprintf(fdres, "%.1f, %.3f,\n", T,
                      properties[val].f(n, T)*R*T/(thermo_list + n)->weight);
            }
          }
          break;
          
      case MOLAR:
          if ((val == CP) || (val == S))
          {
            for (T = Ti; T < Tf; T += step)
            {
              fprintf(fdres, "%.1f, %.3f,\n", T, properties[val].f(n, T)*R);
            }
          }
          else
          {
            for (T = Ti; T < Tf; T += step)
            {
              fprintf(fdres, "%.1f, %.3f,\n", T, properties[val].f(n, T)*R*T);
            }
          }
          break;
    }
    fprintf(fdres, "];\n");
  }
  

  fclose(fdres);
  fclose(fdplot);
  
  return 0;
  
}
