/* -*- Mode: C; c-file-style: "k&r"; -*-*/

/* rocket.c
 *    sample program for parsing a rocket propellant simulator config.
 *
 * $Id$
 *
 * Copyright (C) 2000,
 *     Antoine Lefebvre <antoine.lefebvre@polymtl.ca>
 *
 */

#include <stdlib.h>
#include <ctype.h>

#include "gpcp.h"
#include "librockflight/include/state.h"

int get_point_value(char *name, float *point);
int load_data(char *file, float **x, float **y, int *n);

Options rockflight_options[] = {
     {"simulation",          PARENT,   NULL},

     {"initial_conditions",  PARENT,  "simulation"},
     {"u",                   FLOAT,   "initial_conditions"},
     {"v",                   FLOAT,   "initial_conditions"},
     {"w",                   FLOAT,   "initial_conditions"},
     {"P",                   FLOAT,   "initial_conditions"},
     {"Q",                   FLOAT,   "initial_conditions"},
     {"R",                   FLOAT,   "initial_conditions"},
     {"latitude",            FLOAT,   "initial_conditions"},
     {"longitude",           FLOAT,   "initial_conditions"},
     {"altitude",            FLOAT,   "initial_conditions"},
     {"phi",                 FLOAT,   "initial_conditions"},
     {"theta",               FLOAT,   "initial_conditions"},
     {"psi",                 FLOAT,   "initial_conditions"},

     {"rocket",              PARENT,  "simulation"},
     {"stage",               PARENT,  "rocket"},

     {"dry_mass",            FLOAT,   "stage"}, 
     {"Ix",                  FLOAT,   "stage"},
     {"Iy",                  FLOAT,   "stage"},      
     {"Iz",                  FLOAT,   "stage"},       
     {"Cdrag",               FLOAT,   "stage"},  
     {"Clift",               FLOAT,   "stage"},   
     {"Cbeta",               FLOAT,   "stage"},   
     {"Cspin",               FLOAT,   "stage"},
     {"Cmoment",             FLOAT,   "stage"},
     {"Cdamping",            FLOAT,   "stage"},
     {"Diameter",            FLOAT,   "stage"},
     {"active_time",         FLOAT,   "stage"},

     {"engine",              PARENT,  "stage"},
     {"type",                STRING,  "engine"},
     {"propellant_mass",     FLOAT,   "engine"},
     {"dry_mass",            FLOAT,   "engine"},
     {"thrust",              FLOAT,   "engine"},
     {"thrust_data_file",    STRING,  "engine"},
     {"mass_flow",           FLOAT,   "engine"},
     {"c",                   FLOAT,   "engine"},
     {"start_time",          FLOAT,   "engine"},
     {"burn_time",           FLOAT,   "engine"},
     {"drop_time",           FLOAT,   "engine"},
     {"position",            PARENT,  "engine"},
     {"x",                   FLOAT,   "position"},
     {"y",                   FLOAT,   "position"},
     {"z",                   FLOAT,   "position"},
     {"direction",           PARENT,  "engine"},
     {"x",                   FLOAT,   "direction"},
     {"y",                   FLOAT,   "direction"},
     {"z",                   FLOAT,   "direction"},
			       
     {"solution",            PARENT,  "simulation"},
     {"duration",            FLOAT,   "solution"},
     {"dt",                  FLOAT,   "solution"},
     {"precision",           FLOAT,   "solution"},
     {NULL,                  0,        NULL}
};

int load_config(Data *data, rocket_t *rocket, float *init_cond,
              solution_t *solution)
{
     int i, j;
     int n, m;

     int status;

     char *type;
     char *filename;
     float *val;
     
     engine_t *engine;

     if (GPCP_EnterLevel("simulation", 0) == -1)
     {
          printf("Error: no simulation to execute in config file.\n");
          return -1;
     }
     else
     {
          /***************** initial conditions ********************/
          n = GPCP_NumParent("initial_conditions");
          if (n > 1)
          {
               printf("There should be only one \"initial_conditions\""
                      " keyword in \"simulation\".\n"
                      "The first one will be used.\n");
          }
          else if (n == 0)
          {
               printf("No \"initial_conditions\" in config file.\n");
               return -1;
          }
          
          if (GPCP_EnterLevel("initial_conditions", 0) == -1)
          {

          }
          else
          {
               
               if (GPCP_GetValue ("u", init_cond) != 0)
                    *(init_cond+0) = 0.0;
               
               if (GPCP_GetValue ("v", init_cond+1) != 0)
                    *(init_cond+1) = 0.0;

               if (GPCP_GetValue ("w", init_cond+2) != 0)
                    *(init_cond+2) = 0.0;

               if (GPCP_GetValue ("P", init_cond+3) != 0)
                    *(init_cond+3) = 0.0;

               if (GPCP_GetValue ("Q", init_cond+4) != 0)
                    *(init_cond+4) = 0.0;

               if (GPCP_GetValue ("R", init_cond+5) != 0)
                    *(init_cond+5) = 0.0;

               if (GPCP_GetValue ("latitude", init_cond+6) != 0)
                    *(init_cond+6) = 0.0;

               if (GPCP_GetValue ("longitude", init_cond+7) != 0)
                    *(init_cond+7) = 0.0;

               if (GPCP_GetValue ("altitude", init_cond+8) != 0)
                    *(init_cond+8) = 0.0;

               if (GPCP_GetValue ("phi", init_cond+9) != 0)
                    *(init_cond+9) = 0.0;

               if (GPCP_GetValue ("theta", init_cond+10) != 0)
                    *(init_cond+10) = 0.0;

               if (GPCP_GetValue ("psi", init_cond+11) != 0)
                    *(init_cond+11) = 0.0;
               
               
               if (GPCP_ExitLevel() != 0) 
               { 
                    printf("Unable to leave 'initial_conditions' level.\n"); 
               }  
          }
          /********************* end initial_conditions *****************/
          
          /********************* rocket *********************************/
          n = GPCP_NumParent("rocket");
          if (n > 1)
          {
               printf("There should be only one \"rocket\""
                      " keyword in \"simulation\".\n"
                      "The first one will be used.\n");
          }
          else if (n == 0)
          {
               printf("No \"rocket\" in config file.\n");
               return -1;
          }

          if (GPCP_EnterLevel("rocket", 0) == -1)
          {

          }
          else
          {
               n = GPCP_NumParent("stage");
               if (n == 0)
               {
                    printf("No \"stage\" in config file.\n");
                    return -1;
               }

               /* allocate memory for stage properties */
               (*rocket).n_stage = n;
               (*rocket).stage_properties = (rocket_properties_t *)
                    malloc (sizeof(rocket_properties_t) * n);
               
               /* for each stage */
               for (i = 0; i < n; i++)
               {
                    if (GPCP_EnterLevel("stage", i) == -1)
                    {
                         
                    }
                    else
                    {

                         val = &(*rocket).stage_properties[i].dry_mass;
                         if (GPCP_GetValue ("dry_mass", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Ix;
                         if (GPCP_GetValue ("Ix", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Iy;
                         if (GPCP_GetValue ("Iy", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Iz;
                         if (GPCP_GetValue ("Iz", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Cdrag;
                         if (GPCP_GetValue ("Cdrag", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Clift;
                         if (GPCP_GetValue ("Clift", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Cbeta;
                         if (GPCP_GetValue ("Cbeta", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Cspin;
                         if (GPCP_GetValue ("Cspin", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Cmoment;
                         if (GPCP_GetValue ("Cmoment", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Cdamping;
                         if (GPCP_GetValue ("Cdamping", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Diameter;
                         if (GPCP_GetValue ("Diameter", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].active_time;
                         if (GPCP_GetValue ("active_time", val) != 0)
                              *val = 0.0;

			 m = GPCP_NumParent("engine");
			 (*rocket).stage_properties[i].n_engine = m;
			 if (m < 1)
			 {
			      printf("There is no engine for stage %d\n", i);
			      (*rocket).stage_properties[i].engines = NULL;
			 }
			 else
			 {
			      /* allocate memory for engines */
			      (*rocket).stage_properties[i].engines = 
				   (engine_t *) malloc (sizeof(engine_t)*m);
			 }
			 
			 for (j = 0; j < m; j++)
			 {         
			      if (GPCP_EnterLevel("engine", j) == -1)
			      {
				   
			      }
			      else
			      {
		       	       /* set a temporary pointer */
			       engine = 
				   &((*rocket).stage_properties[i].engines[j]);
			       
			       if (GPCP_GetValue("type", &type) != 0)
			       {
				 printf("Type of thrust must be specified.\n");
				 return -1;
			       }
			       else
			       {
				    if (strcmp("constant_thrust", type) == 0)
				    {
					 engine->thrust_type = _CONSTANT;
				    }
				    else if (strcmp("thrust_vs_time", type) == 0)
				    {
					 engine->thrust_type = _FUNCTION;
				    }
				    else
				    {
					 printf("Undefined type of thrust.\n");
					 return -1;
				    }
				    free(type);
			       }

			       switch (engine->thrust_type)
			       {
				    
			       case _CONSTANT:
				    engine->time = NULL;
				    engine->thrust = 
					 (float *) malloc(sizeof(float));
				    
				    val = engine->thrust;
				    if (GPCP_GetValue("thrust", val) != 0)
					 *val = 0.0;
				    break;
				    
			       case _FUNCTION:

				    if (GPCP_GetValue("thrust_data_file",
						      &filename) 
					== 0)
				    {
					 status = load_data(filename, 
							    &(engine->time), 
							    &(engine->thrust), 
							    &(engine->n_point)
					      );

					 if (status != 0)
					 {
					  printf("Error reading data file.\n");
					  return -1;
					 }
					 free(filename);
				    }
				    break;
			       }


			       val = &(engine->propellant_mass);
			       if (GPCP_GetValue("propellant_mass", val) != 0)
				    *val = 0.0;

			       val = &(engine->dry_mass);
			       if (GPCP_GetValue("dry_mass", val) != 0)
				    *val = 0.0;

			       val = &(engine->mass_flow);
			       if (GPCP_GetValue("mass_flow", val) != 0)
				    *val = 0.0;

			       val = &(engine->c);
			       if (GPCP_GetValue("c", val) != 0)
				    *val = 0.0;

			       val = &(engine->start_time);
			       if (GPCP_GetValue("start_time", val) != 0)
				    *val = 0.0;

			       val = &(engine->burn_time);
			       if (GPCP_GetValue("burn_time", val) != 0)
				    *val = 0.0;

			       val = &(engine->drop_time);
			       if (GPCP_GetValue("drop_time", val) != 0)
				    *val = 0.0;

			       val = engine->position;
			       if (get_point_value("position", val) != 0)
			       {
				    printf("Unable to get position.\n");
			       }

			       val = engine->direction;
			       if (get_point_value("direction", val) != 0)
			       {
				    printf("Unble to get direction.\n");
			       }
			       
			      }
			      if (GPCP_ExitLevel() != 0)
			      {
				   printf("Unable to leave engine level.\n");
			      }
			 }
			 
			 
                         if (GPCP_ExitLevel() != 0) 
                         { 
                              printf("Unable to leave 'initial_conditions'"
                                     " level.\n"); 
                         }  
                    }
                    
               }
	       
               if (GPCP_ExitLevel() != 0) 
               { 
                    printf("Unable to leave 'rocket' level.\n"); 
               }  
          }
          /********************* end rocket *****************************/
          

          /********************* solution *******************************/
          n = GPCP_NumParent("solution");
          if (n > 1)
          {
               printf("There should be only one \"solution\""
                      " keyword in \"simulation\".\n"
                      "The first one will be used.\n");
          }
          else if (n == 0)
          {
               printf("No \"solution\" in config file.\n");
               return -1;
          }

          if (GPCP_EnterLevel("solution", 0) == -1)
          {

          }
          else
          {
               if (GPCP_GetValue ("duration", &((*solution).duration)) != 0)
               {
                    printf("A duration mus be specified.\n");
               }
               if (GPCP_GetValue ("dt", &((*solution).dt)) != 0)
               {
                    (*solution).dt = 0.1;
               }
               if (GPCP_GetValue ("precision", &((*solution).precision)) != 0)
               {
                    (*solution).precision = 0.01;   
               }
               
               if (GPCP_ExitLevel() != 0) 
               { 
                    printf("Unable to leave 'solution' level.\n"); 
               }   
          }
          /***************** end solution *****************************/

          if (GPCP_ExitLevel() != 0) 
          { 
               printf("Unable to leave 'simulation' level.\n"); 
          }           
     }

     return 0;
}


int get_point_value(char *name, float *point)
{
     if (GPCP_EnterLevel(name, 0) != 0)
     {
	  return -1;
     }
     else
     {
	  if (GPCP_GetValue("x", point+0) != 0)
	       *(point+0) = 0.0;
	  if (GPCP_GetValue("y", point+1) != 0)
	       *(point+1) = 0.0;
	  if (GPCP_GetValue("z", point+2) != 0)
	       *(point+2) = 0.0;
     }

     if (GPCP_ExitLevel() != 0)
     {
	  printf("Unable to leave '%s' level.\n", name);
	  return -1;
     }
     return 0;
}

int read_file(char *config_file, rocket_t *rocket, float *init_cond,
              solution_t *solution)
{
     Data *data;
     GPCP_RegisterOptions(rockflight_options);

     if (GPCP_ReadFile(config_file, &data) != 0)
     {
          printf("Error reading config file.\n");
          return -1;
     }

     if (load_config(data, rocket, init_cond, solution) != 0)
     {
          printf("Error: unable to correctly load data.\n");
          return -1;
     }
     
     /* GPCP_PrintData(data); */
           
     GPCP_FreeData(&data);
     return 0;
}

int load_data(char *file, float **x, float **y, int *n)
{
     int i;
     FILE *fd;
     char line[128];
     char *retval;
     int scan_status;

     fd = fopen(file, "r");
     
     if (fd == NULL) 
     {
	  printf("Unable to open file: %s.\n", file);
	  return -1;
     }
     printf("Reading data file...\n");

     retval = fgets(line, 128, fd);
     *x = NULL;
     *y = NULL;
     i = 0;
     while (retval != NULL)
     {
	  if (line[0] != '#')
	  {
	       i++;
	       
	       *x = (float *) realloc(*x, sizeof(float)*i);
	       if (*x == NULL)
	       {
		    printf("memory error.\n");
		    return -1;
	       }
	       *y = (float *) realloc(*y, sizeof(float)*i);
	       if (*y == NULL)
	       {
		    printf("memory error.\n");
		    return -1;
	       }
	       
	       scan_status = sscanf(line, "%f %f", *x+i-1, *y+i-1);

	       if (scan_status != 2)
	       {
		    printf("Error reading file.\n");
	       }
	  }
	  retval = fgets(line, 128, fd);	  
     }

     *n = i;

     printf("... %d points read.\n", i);

     fclose(fd);
     return 0;
}

