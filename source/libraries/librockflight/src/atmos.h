#ifndef __atmos_h__
#define __atmos_h__

typedef struct
{
  double T;
  double P;
  double rho;
  double c;
} AIR;

int atmosphere (AIR *a, double altitude);

#endif __atmos_h__
