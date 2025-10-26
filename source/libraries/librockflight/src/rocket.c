
#include <stdlib.h>
#include "rocket.h"

int initialize_flight (flight_t *flight, int n_stages)
{
 int i;
 
 flight = (flight_t *) malloc (sizeof(flight_t));
 if (flight == NULL)
   return -1;

 flight->n_stages = n_stages;
 flight->stages = malloc (sizeof(stage_t));

 if (flight->stages == NULL)
   return -1;
 
 for (i = 0; i < n_stages; i++)
 {
   flight->stages[i].is_set = 0;
 }
 return 0;
}

int set_stages (flight_t *flight, int stage, float dry_mass, 
                float start_time, float burn_time, float drop_time,
                float propellant_mass, float thrust, float Isp)
{
  flight->stages[stage].is_set = 1;
  flight->stages[stage].dry_mass = dry_mass;
  flight->stages[stage].start_time = start_time;
  flight->stages[stage].burn_time = burn_time;
  flight->stages[stage].drop_time = drop_time;

  flight->stages[stage].propellant_mass = propellant_mass;
  flight->stages[stage].thrust = thrust;
  flight->stages[stage].Isp = Isp;
  return 0;
}
