#include <stdio.h>
#include "librockflight/include/state.h"

void print_summary(rocket_t *rocket, float *init_cond, solution_t *solution)
{
  int i, j, k;
  double propellant_mass = 0.0;
  double rocket_mass = 0.0;
  
  printf("---------------------------------------------\n");
  printf("             SOLUTION SUMMARY\n");
  printf("---------------------------------------------\n");
  printf("Solution duration  : %.2f s\n", (*solution).duration);
  printf("Solution dt        : %g \n", (*solution).dt);
  printf("Solution precision : %.2e \n", (*solution).precision);
  printf("\n");
  printf("Initial conditions.\n");
  printf("-------------------\n");
  printf("  u         = %.2e m/s\n", init_cond[0]);
  printf("  v         = %.2e m/s\n", init_cond[1]);
  printf("  w         = %.2e m/s\n", init_cond[2]);
  printf("  P         = %.2e rad/s\n", init_cond[3]);
  printf("  Q         = %.2e rad/s\n", init_cond[4]);
  printf("  R         = %.2e rad/s\n", init_cond[5]);
  printf("  longitude = %.2e deg\n", init_cond[6]);
  printf("  latitude  = %.2e deg\n", init_cond[7]);
  printf("  altitude  = %.2e m\n", init_cond[8]);
  printf("  phi       = %.2e deg\n", init_cond[9]);
  printf("  theta     = %.2e deg\n", init_cond[10]);
  printf("  psi       = %.2e deg\n", init_cond[11]);
  printf("\n");
  printf("Rocket properties.\n");
  printf("------------------\n");
  printf("  Number of stages : %d\n", (*rocket).n_stage);

  for (i = 0; i < (*rocket).n_stage; i++)
  {
       /* engines contribution */
       for (j = 0; j < (*rocket).stage_properties[i].n_engine; j++)
       {
	    propellant_mass += 
		 (*rocket).stage_properties[i].engines[j].propellant_mass;
	    rocket_mass += 
		 (*rocket).stage_properties[i].engines[j].dry_mass;
       }
       /* stage structure and payload */
       rocket_mass += (*rocket).stage_properties[i].dry_mass;
  }

  printf("  Propellant mass : %.2e kg\n", propellant_mass);
  printf("  Rocket dry mass : %.2e kg\n", rocket_mass);
  printf("  Total mass      : %.2e kg\n", propellant_mass + rocket_mass);
  printf("\n");
  printf("  Propellant mass fraction : %.2f %%\n",
         100*propellant_mass/(propellant_mass + rocket_mass));
  printf("\n");

  printf("Stages properties.\n");
  printf("------------------\n");

  for (i = 0; i < (*rocket).n_stage; i++)
  {
    printf("  Stage %d\n", i+1);
    printf("  |->Diameter               : %.2e m\n",
           (*rocket).stage_properties[i].Diameter);
    printf("  |->Dry mass               : %.2e kg\n",
           (*rocket).stage_properties[i].dry_mass);
    printf("  |->Ix                     : %.2e kg\n",
           (*rocket).stage_properties[i].Ix);
    printf("  |->Iy                     : %.2e kg\n",
           (*rocket).stage_properties[i].Iy);
    printf("  |->Iz                     : %.2e kg\n",
           (*rocket).stage_properties[i].Iz);
    printf("  |->Active time            : %.2e s\n",
           (*rocket).stage_properties[i].active_time);

/*    printf("   Aerodynamic properties.\n"); */
    printf("  |->Drag coefficient       : %.2e\n",
           (*rocket).stage_properties[i].Cdrag);
    printf("  |->Lift coefficient       : %.2e\n",
           (*rocket).stage_properties[i].Clift);
    printf("  |->Beta coefficient       : %.2e\n",
           (*rocket).stage_properties[i].Cbeta);
    printf("  |->Cross spin coefficient : %.2e\n",
           (*rocket).stage_properties[i].Cspin);
    printf("  |->Moment coefficient     : %.2e\n",
           (*rocket).stage_properties[i].Cmoment);
    printf("  |->Damping coefficient    : %.2e\n",
           (*rocket).stage_properties[i].Cdamping);

    printf("  |->Number of engines      : %d\n",
	   (*rocket).stage_properties[i].n_engine);
    for (j = 0; j < (*rocket).stage_properties[i].n_engine; j++)
    {
	 printf("  |->Engine %d\n", j);
	 printf("     |->Propellant mass    : %.2e\n",
		(*rocket).stage_properties[i].engines[j].propellant_mass);
	 printf("     |->Dry mass           : %.2e\n",
		(*rocket).stage_properties[i].engines[j].dry_mass);
	 printf("     |->Start time         : %.2e\n",
		(*rocket).stage_properties[i].engines[j].start_time);
	 printf("     |->Burn time          : %.2e\n",
		(*rocket).stage_properties[i].engines[j].burn_time);
	 printf("     |->Drop time          : %.2e\n",
		(*rocket).stage_properties[i].engines[j].drop_time);
	 printf("     |->Position           : (%.2e, %.2e, %.2e)\n",
		(*rocket).stage_properties[i].engines[j].position[0],
		(*rocket).stage_properties[i].engines[j].position[1],
		(*rocket).stage_properties[i].engines[j].position[2]);
	 printf("     |->Direction          : (%.2e, %.2e, %.2e)\n",
		(*rocket).stage_properties[i].engines[j].direction[0],
		(*rocket).stage_properties[i].engines[j].direction[1],
		(*rocket).stage_properties[i].engines[j].direction[2]);
	 printf("     |->Thrust type        : ");
	 switch((*rocket).stage_properties[i].engines[j].thrust_type)
	 {
	 case _CONSTANT:
	   printf("Constant\n");
	   printf("     |->Thrust             : %.2e\n",
		  *(*rocket).stage_properties[i].engines[j].thrust);
	   break;
	 case _FUNCTION:
	   printf("Function (%d points)\n", 
		  (*rocket).stage_properties[i].engines[j].n_point);
	   for (k = 0; k < (*rocket).stage_properties[i].engines[j].n_point;
		k++)
	   {
	     printf("                             (%.2e, %.2e)\n",
		   *((*rocket).stage_properties[i].engines[j].time+k),
		   *((*rocket).stage_properties[i].engines[j].thrust+k));
	   }
	   break;
	 }
    }

    printf("\n"); 
  }
  printf("---------------------------------------------\n");
  printf("                END SUMMARY\n");
  printf("---------------------------------------------\n");
}


void print_octave_results(double *ans, const int neq, const int n)
{
  int i;
  
  printf("# Created by rocflight\n");
  printf("# name: M\n");
  printf("# type: matrix\n");
  printf("# rows: %d\n", n);/*(int)(duration/dt) + 1);*/
  printf("# columns: %d\n", neq + 1);

  
  for( i = 0; i < n; i++)
  {
    printf("% .12e ", ans[12 + (neq+1)*i]); /* t */
    printf("% .12e ", ans[0  + (neq+1)*i]); /* u */
    printf("% .12e ", ans[1  + (neq+1)*i]); /* v */
    printf("% .12e ", ans[2  + (neq+1)*i]); /* w */
    printf("% .12e ", ans[3  + (neq+1)*i]); /* p */
    printf("% .12e ", ans[4  + (neq+1)*i]); /* q */
    printf("% .12e ", ans[5  + (neq+1)*i]); /* r */
    printf("% .12e ", ans[6  + (neq+1)*i]); /* lambda */
    printf("% .12e ", ans[7  + (neq+1)*i]); /* mu */
    printf("% .12e ", ans[8  + (neq+1)*i]); /* R */
    printf("% .12e ", ans[9  + (neq+1)*i]); /* phi */
    printf("% .12e ", ans[10 + (neq+1)*i]); /* theta */
    printf("% .12e ", ans[11 + (neq+1)*i]); /* psi */
    printf("\n");
  }
  
}
