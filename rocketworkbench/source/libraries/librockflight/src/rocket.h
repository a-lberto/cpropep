#ifndef __stage_h__
#define __stage_h__

typedef struct stage
{
   int is_set;

   /* engine properties */
   float propellant_mass;
   float thrust;
   float Isp;

   /* flight profile */
   float start_time;
   float burn_time;
   float drop_time;

   /* inertial properties */
   float dry_mass;
   float Ix;
   float Iy;
   float Iz;
   
   /* aerodynamic properties */
   float Cdrag;
   float Clift;
   float Cbeta;
   float Cspin;
   float Cmoment;
   float Cdamping;

   float Diameter;
} stage_t;

typedef struct flight
{
  int n_stages;
  stage_t *stages;
  
} flight_t;

#endif __stage_h__
