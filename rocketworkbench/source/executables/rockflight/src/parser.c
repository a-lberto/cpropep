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
/*     {"nb_stage",            INTEGER, "rocket"},*/
     {"stage",               PARENT,  "rocket"},
     {"propellant_mass",     FLOAT,   "stage"},
     {"thrust",              FLOAT,   "stage"},
     {"Isp",                 FLOAT,   "stage"},  
     {"start_time",          FLOAT,   "stage"},
     {"burn_time",           FLOAT,   "stage"},
     {"drop_time",           FLOAT,   "stage"},
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

     {"solution",            PARENT,  "simulation"},
     {"duration",            FLOAT,   "solution"},
     {"dt",                  FLOAT,   "solution"},
     {"precision",           FLOAT,   "solution"},
     {NULL,                  0,        NULL}
};

int load_data(Data *data, rocket_t *rocket, float *init_cond,
              solution_t *solution)
{
     int i;
     int n;

     float *val;
     
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
                         val = &(*rocket).stage_properties[i].propellant_mass;
                         if (GPCP_GetValue ("propellant_mass", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].thrust;
                         if (GPCP_GetValue ("thrust", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].Isp;
                         if (GPCP_GetValue ("Isp", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].start_time;
                         if (GPCP_GetValue ("start_time", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].burn_time;
                         if (GPCP_GetValue ("burn_time", val) != 0)
                              *val = 0.0;

                         val = &(*rocket).stage_properties[i].drop_time;
                         if (GPCP_GetValue ("drop_time", val) != 0)
                              *val = 0.0;

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

                         if (GPCP_ExitLevel() != 0) 
                         { 
                              printf("Unable to leave 'initial_conditions'"
                                     " level.\n"); 
                         }  
                    }
                    
/*                    if (GPCP_ExitLevel() != 0) 
                    { 
                         printf("Unable to leave 'stage' level.\n"); 
                         } */ 
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

     if (load_data(data, rocket, init_cond, solution) != 0)
     {
          printf("Error: unable to correctly load data.\n");
          return -1;
     }
     
     /* GPCP_PrintData(data); */
           
     GPCP_FreeData(&data);
     return 0;
}

