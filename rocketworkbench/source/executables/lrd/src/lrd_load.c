
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libcpropep/include/conversion.h"

#include "liblrd/include/lrd.h"

int load_input(char *filename, design_param_t *d, liquid_propellant_t *p)
{
  int section = 0;

  FILE *fd;
 
  double sp;
  
  char buffer[128], keyword[64], num[10], qt[10], unit[10];
  
  char *bufptr;

  fd = fopen(filename, "r");

  if (fd == NULL)
    return -1;
  
  while ( fgets(buffer, 128, fd) != NULL )
  {
    switch (section)
    {
      case 0:

          if (buffer[0] == ' ' || buffer[0] == '\n' || buffer[0] == '\0' ||
              buffer[0] == '#')
          {
            section = 0;
            break;
          }
          else if (strncmp(buffer, "Propellant", 10) == 0)
          {
            section = 1;
          }
          else if (strncmp(buffer, "Parameter", 9) == 0)
          {
            section = 2;
          }
          else
          {
            printf("Unknown keyword 1.\n");
          }
          break;
          
      case 1:

          if (buffer[0] == '+')
          {
            sscanf(buffer, "%s %s", keyword, num);
            bufptr = keyword + 1;

            sp = atof(num);
            
            if (strcmp(bufptr, "oxidant") == 0)
            {
              p->oxidant = (int) sp;
            }
            else if (strcmp(bufptr, "fuel") == 0)
            {
              p->fuel = (int) sp;
            }
            else if (strcmp(bufptr, "o/f") == 0)
            {
              p->o_f_ratio = sp;
            }
            else
            {
              printf("Unknown keyword 2.\n");
            }
            break;
          }
          else if (buffer[0] == '#')
          {
            break;
          }
          else if (buffer[0] == ' ' || buffer[0] == '\n' || buffer[0] == '\0')
          {
            section = 0;
          }
          break;

      case 2:

          if (buffer[0] == '+')
          {
            sscanf(buffer, "%s %s %s", keyword, num, unit);
            bufptr = keyword + 1;

            sp = atof(num);
            
            if (strcmp(bufptr, "chamber_pressure") == 0)
            {
              if (strcmp(unit, "atm") == 0)
              {
                d->chamber_pressure = sp;
              }
              else if (strcmp(unit, "psi") == 0)
              {
                d->chamber_pressure = sp * PSI_TO_ATM;
              }
              else
              {
                printf("Unknown unit.\n");
                return -1;
              }

            }
            else if (strcmp(bufptr, "thrust") == 0)
            {
              if (strcmp(unit, "N") == 0)
              {
                d->thrust = sp;
              }
              else if (strcmp(unit, "lbf") == 0)
              {
                d->thrust = sp * LBF_TO_N;
              }
              else
              {
                printf("Unknown unit.\n");
                return -1;
              }
            }
            else if (strcmp(bufptr, "L*") == 0)
            {
              if (strcmp(unit, "m") == 0)
              {
                d->Lstar = sp;
              }
              else if (strcmp(unit, "in") == 0)
              {
                d->Lstar = sp * IN_TO_M;
              }
              else
              {
                printf("Unknown unit.\n");
              }
            }
            else if (strcmp(bufptr, "Dc/Dt") == 0)
            {
              d->Dc_Dt = sp;
            }
            else if (strcmp(bufptr, "theta") == 0)
            {
              d->theta = sp;
            }
            else if (strcmp(bufptr, "r_rt") == 0)
            {
              d->R_Rt = sp;
            }
            else
            {
              printf("Unknown keyword 3.\n");
            }
            break;
          }
          else if (buffer[0] == '#')
          {
            break;
          }
          else if (buffer[0] == ' ' || buffer[0] == '\n' || buffer[0] == '\0')
          {
            section = 0;
          }
          break;

    }
  }

  fclose(fd);
  
  return 0;
}
