
#include <stdlib.h>
#include <stdio.h>

typedef enum type
{
  MASS     = 1,
  FORCE    = 2,
  PRESSURE = 3
} type_t;

typedef struct conversion
{
  char *units;
  double factor;
  type_t type;
} conversion_t;

conversion_t conversion_factors[] = {
  {"kg",  1.0,         MASS},
  {"g",   0.001,       MASS},
  {"lb",  0.45359237,  MASS},
  {"oz",  0.028349523, MASS},
  {"N",   1.0,         FORCE},
  {"lbf", 4.4482216,   FORCE},
  {"Pa",  1.0,         PRESSURE},
  {"atm", 101325,      PRESSURE},
  {"bar", 1e5,         PRESSURE},
  {"psi", 6894.7573,   PRESSURE},
  {NULL,  0.0,         0}
};


/* This function will convert the value 'val' represent in units
 * to is SI representation
 */

int convert(double *val, char *from_units, char *to_units, double *newval)
{
  conversion_t *conv = conversion_factors;

  int i = 0;
  double factor = 1;

  short from_ok = 0, to_ok = 0;

  type_t from_type = 0, to_type = 0;
  
  while (conv[i].units != NULL)
  {
    if (strcmp(from_units, conv[i].units) == 0)
    {
      factor *= conv[i].factor;
      from_ok = 1;
      from_type = conv[i].type;
    }
    else if (strcmp(to_units, conv[i].units) == 0)
    {
      factor /= conv[i].factor;
      to_ok = 1;
      to_type = conv[i].type;
    }
    i++;
  }

  if ((from_type != to_type) || from_type == 0 || to_type == 0)
    printf("Incompatible units.\n");

  if (from_ok == 0)
    printf("Unrecognized from units.\n");

  if (to_ok == 0)
    printf("Unrecognized to units.\n");
  
  if (from_ok && to_ok)
    *newval = *val * factor;
  else
    return -1;
  
  return 0;
}


