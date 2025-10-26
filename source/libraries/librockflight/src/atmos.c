/************************************************************************
File:         atmos.c
Author:       David E. Hall
Date:         24 April 1996
Description:  A quick and dirty atmosphere calculator based upon the
  module found in the ExtREME (Extendible Round Earth Missile Emulation)
  simulation (authored by your's truly as well).
*************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "atmos.h"

/*--- physical constants */
#define EARTH_RAD    6.3567668e6         /* earth radius (m)           */
#define GRAVITY      -9.806655352        /* gravitation accel (m/s2)    */
#define R            287.05376255        /* gas constant for air       */

int atmosphere (AIR *a, double altitude)
{

/*
  This function determines the atmospheric conditions at any given
  altitude.  Note: the routine is a translation of an old FORTRAN
  routine of unknown origin and thus I have no idea as to what is
  being referenced in the original notes listed below....
  This atmosphere model is after the ISO Standard atmosphere as
  documented in ref. no. ISO 2533-1975(E) which has a first edition
  date of 1975-05-15 and corrected and reprinted date of 1978-12-15.
  This atmosphere is identical with the ICAO and WMO Standard
  atmospheres from -2000 to 32000 meters.
*/
  
  static double Abreak[9] = {-2.0,     11.0,    20.0,    32.0,    47.0,
                             52.0,     61.0,    79.0,    88.74          };
  static double Tnot[9]   = {301.15,   216.65,  216.65,  228.65,  270.65,
                             270.65,   252.65,  180.65,  180.65         };
  static double Tslope[9] = {-6.5,     0.0,     1.0,     2.8,     0.0,
                             -2.0,     -4.0,    0.0,     0.0            };
  static double Pnot[9]   = {127774.0, 22632.0, 5475.87, 868.014, 
                             110.906,  59.002,  18.208,  1.03757, 0.00  };
  int zone = -1;
  double alt;
  double delta_alt;
  double x, y;

  /*fprintf(stderr, "%f\n", altitude);*/
  /* Convert to geopotential altitude */
  alt = EARTH_RAD*altitude/(EARTH_RAD+altitude)/1000.0;
  
  while ((zone < 8) && (alt >= Abreak[zone + 1]))
    zone++;
  
  if (zone < 0)
  {      /* Below atmosphere data! */
    /* fprintf(stderr, "*** ERROR: Below atmospheric model (%f)\n", alt); */
    return -1;
  }
  
  /* The meat of the function */
  delta_alt = alt - Abreak[zone];
  a->T = Tnot[zone] + Tslope[zone] * delta_alt;
  
  switch (zone)
  {
    case 1:
    case 4:
    case 7:
    case 8:
        a->P = Pnot[zone] * exp(GRAVITY*delta_alt*1000.0/R
                                /a->T);
        break;
        
    default:
        x = 1.0 + Tslope[zone] * delta_alt / Tnot[zone];
        y = GRAVITY * 1000.0 / Tslope[zone] / R;
        a->P = Pnot[zone] * pow(x,y);
        break;
  }
  
  a->rho = a->P / R / a->T;
  a->c = 20.046796 * sqrt(a->T);  /* c = sqrt(kRT) k=1.4 */
  
  return 0;
}

/*
int main(int argc, char *argw[])
{
  AIR air;
  double alt;
  
  for (alt = 0; alt < 100000; alt += 1000)
  {
    atmosphere(&air, alt);
    printf("%f %f %f %f\n", air.temperature, air.pressure,
           air.density, air.mach1);
  }  
  return 0;
}
*/
