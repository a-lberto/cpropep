
#include <math.h>
#include <stdio.h>

#include "libmcp/include/mcp.h"

#include "libcpropep/include/equilibrium.h"
#include "libcpropep/include/performance.h"

#include "libcpropep/include/conversion.h"
#include "libcpropep/include/const.h"

int modele(int neq, double time, double *y, double *dy, void *data)
{

  data_t *d; /* structure containing the data */
  
  double ld; /* linear density of propellant kg/m */
  double tmdot; /* throat mass flow rate */

  
  double s; /* linear distance already burned (m) */
  double p; /* pressure (atm) */

  double t; /* thrust in Newton */
  
  d = (data_t *) data;

  s = y[0];
  p = y[1];

  
  d->e->properties.P = p;
  
  equilibrium(d->e, HP);
  frozen_performance(d->e, SUPERSONIC_AREA_RATIO, d->ea/d->ta);
    
  ld = (d->Ab)(s) * (d->e->propellant.density * 1000); /* kg/m */
  tmdot = d->ta * ATM_TO_PA / (d->e + 1)->performance.a_dotm; /* kg/s */

  dy[0] = d->a * pow(p, d->n);

  dy[1] = (R*1000*d->e->properties.T/(d->Vc)(s)) *
    (ld*dy[0] - tmdot) / (d->e->properties.M * ATM_TO_PA);

  /*
  printf("%.2e %.2e %.2e %.2e %.2e\n",
         time,
         d->e->properties.T,
         d->e->properties.M,
         (d->e + 2)->performance.Isp,
         tmdot*(d->e + 2)->performance.Isp);
  */
  
  return 0;

}
