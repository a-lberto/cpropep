/* -*- Mode: C; c-file-style: "k&r"; -*-*/

/* rocket.c
 *    sample program for parsing a rocket propellant simulator config.
 *
 * $Id$
 *
 * Copyright (C) 2000,
 *     Antoine Lefebvre <antoine.lefebvre@polymtl.ca>
 *     Remi Lefebvre <remi@debian.org>
 *
 * gpcp is free software; you can redistribute them and/or modify them
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 */

#include <stdlib.h>
#include <ctype.h>

#include "gpcp.h"

#include "thermo.h"

#define DEFAULT_THERMO_FILE "thermo.dat"
#define DEFAULT_PROP_FILE "propellant.dat"

char *thermo_file;
char *propellant_file;

typedef struct _ing
{
     char *name;      /* name of the propellant */
     char *formula;
     int   n_elem;
     int  *elem;
     int  *coef;
     float heat;      /* heat of formation in Joule/gram */
     float density;   /* density in g/cubic cm */
     float qty;       /* number of mol of this ingrediant */
     
} ing_t;

typedef struct _prop_t
{
     char  *name;
     short  ncomp;
     ing_t *ing;
     float  density;

} prop_t;


int get_value(char *keyword, float *val);
int get_composition(ing_t *ing);
int get_ingrediant(prop_t *p);
int get_qty(char *str, float *qty);
int print_info(prop_t *p, int n);

Options rocket_options[] = {
     {"global",              PARENT,   NULL},
     {"propellant_database", STRING,  "global"},
     {"product_database",    STRING,  "global"},
     {"propellant",          PARENT,   NULL},
     {"name",                STRING,  "propellant"},
     {"ingrediant",          PARENT,  "propellant"},
     {"name",                STRING,  "ingrediant"},
     {"formula",             STRING,  "ingrediant"},
     {"comp",                PARENT,  "ingrediant"},
     {"ing",                 STRING,  "comp"},
     {"enthalpy",            STRING,  "ingrediant"},
     {"qty",                 STRING,  "ingrediant"},
     {"density",             STRING,  "propellant"},
     {"database",            INTEGER, "ingrediant"},
     {"problem",             PARENT,   NULL},
     {"type",                STRING,  "problem"},
     {"propellant",          STRING,  "problem"},
     {"chamber_pressure",    STRING,  "problem"},
     {"exit_pressure",       STRING,  "problem"},
     {NULL,                  0,        NULL}
};

Functions rocket_functions[] = {
     {"compute_density", FLOAT},
     {NULL,              0}
};


int main(int argc, char *argv[])
{
     int i;
     int n;

     int n_prop;
     prop_t *prop_list = NULL;
     
     Data *data;
  
     GPCP_RegisterOptions(rocket_options);   

     if (GPCP_ReadFile("input.pro", &data) != 0)
     {
          printf("Error reading config file.\n");
          return 0;
     }

     //GPCP_PrintData(data);

     if (GPCP_EnterLevel("global", 0) == -1)
     {
          printf("No global definitions, using default values.\n");
          propellant_file = (char *) malloc(strlen(DEFAULT_PROP_FILE) + 1);
          strcpy(propellant_file, DEFAULT_PROP_FILE);

          thermo_file = (char *) malloc(strlen(DEFAULT_THERMO_FILE) + 1);
          strcpy(thermo_file, DEFAULT_THERMO_FILE);
     }
     else
     {
          if (GPCP_GetValue ("propellant_database", &propellant_file) != 0)
          {
               printf("No propellant database file specified,"
                      " using default.\n");
               propellant_file = (char *) malloc(strlen(DEFAULT_PROP_FILE)+1);
               strcpy(propellant_file, DEFAULT_PROP_FILE);
          }
          if (GPCP_GetValue ("product_database", &thermo_file) != 0)
          {
               printf("No product database file specified, using default.\n");
               thermo_file = (char *) malloc(strlen(DEFAULT_THERMO_FILE) + 1);
               strcpy(thermo_file, DEFAULT_THERMO_FILE);
          }
          if (GPCP_ExitLevel() != 0)
          {
               printf("Unable to leave directory.\n");
          }
     }
     
     if ((n = GPCP_NumParent("propellant")) == 0)
     { 
          printf ("No propellant definition available.\n");
     }

     //printf("There is %d propellant.\n", n);
     
     n_prop = 0;
     
     for (i = 0; i < n; i++)
     {
          //printf("Propellant............\n");
          
          if (GPCP_EnterLevel("propellant", i) != 0)
          {
               printf("Unable to get propellant definition for %d.\n", i);
          }
          else
          {
               n_prop++;
               prop_list = (prop_t *) realloc (prop_list,
                                               sizeof(prop_t) * n_prop);
               
               if (GPCP_GetValue ("name", &(prop_list[i].name)) != 0)
                    printf("Name of propellant not specified.\n");
               
               //printf("%s\n", prop_list[i].name);
               
               get_ingrediant(prop_list + i);

               //get_value("density", &(prop_list[i].density));
               
               if (GPCP_ExitLevel() != 0)
                    printf("Unable to leave level.\n");
          }
     }

     print_info(prop_list, n_prop);
      
     GPCP_FreeData(&data);

  
     return 0;
  
}

int get_ingrediant(prop_t *p)
{
     int i, n;

     char *string;
     
     if ((n = GPCP_NumParent("ingrediant")) == 0)
     {
          printf("No ingrediant available.\n");
     }
     
     p->ncomp = n;
     p->ing = NULL;
     
     for (i = 0; i < n; i++)
     {
          if (GPCP_EnterLevel("ingrediant", i) != 0)
               printf("Unable to get ingrediant number %d.\n", i);
          else
          {
               p->ing = (ing_t *) realloc(p->ing,
                                          sizeof(ing_t)*(i+1));
               
               if (GPCP_GetValue("name", &(p->ing[i].name)) != 0)
               {
                    printf("Name of ingrediant not specified.\n");
                    p->ing[i].name = (char *) malloc (sizeof(char)
                                                      *(strlen("NONE")+1));
                    strcpy(p->ing[i].name, "NONE");
               }
               if (GPCP_GetValue("formula", &(p->ing[i].formula)) != 0)
               {
                    printf("Formula of ingrediant not specified.\n");
                    p->ing[i].formula = (char *) malloc (sizeof(char)
                                                      *(strlen("NONE")+1));
                    strcpy(p->ing[i].formula, "NONE");
               }
               
               if (GPCP_GetValue("enthalpy", &string) != 0)
                    printf("Enthalpy not specified.\n");
               else
               {
                    get_qty(string, &(p->ing[i].heat));
                    free(string);
               }
               
               if (GPCP_GetValue("qty", &string) != 0)
                    printf("No quantity specified.\n");
               else
               {
                    get_qty(string, &(p->ing[i].qty));
                    free(string);
               }
               
               get_composition(p->ing + i);
               
               if (GPCP_ExitLevel() != 0)
                    printf("Unable to leave level.\n");
          }
     }

     return 0;
}

int get_qty(char *str, float *qty)
{
     char *token;

     *qty = atof(str);
     return 0;
}

int get_composition(ing_t *ing)
{
     int i, n;

     float tmp;

     char *tmpstr;
     
     char *string;
     
     char elem[3];

     if (GPCP_EnterLevel("comp", 0) != 0)
     {
          printf("Unable to get composition of ingredient.\n");
          return -1;
     }
     
     if ((n = GPCP_NumEntry("ing")) == 0)
     {
         printf("No ing field found\n");
     }
     
     ing->n_elem = 0;
     ing->elem = NULL;
     ing->coef = NULL;
     
     for (i = 0; i < n; i++)
     {
          if (GPCP_GetValue_n("ing", &string, i) != 0)
               printf("Can't read ingrediant.\n");
          else
          {
               ing->n_elem++;
               ing->elem = (int *) realloc(ing->elem,
                                           sizeof(int) * ing->n_elem);
               ing->coef = (int *) realloc(ing->coef,
                                           sizeof(int) * ing->n_elem);

               ing->coef[i] = atoi(string);

               tmpstr = string;
               while (*tmpstr != ' ')
                    tmpstr++;

               elem[0] = *(tmpstr+1);
               elem[1] = *(tmpstr+2);
               elem[2] = '\0';
               if (!(isalpha(elem[1])))
                    elem[1] = ' ';

               //printf("%s, %d, %s...\n", string, ing->coef[i], elem);
               
               ing->elem[i] = atomic_number(elem);
               
               
               free(string);
          }
     }

     if (GPCP_ExitLevel() != 0)
          printf("Unable to leave level.\n");
     
     return 0;
}

int get_value(char *keyword, float *val)
{
     float v = 0;
     char *units;
     
     if (GPCP_EnterLevel(keyword, 0) != 0)
     {
          printf("Unable to enter level.\n");
          return -1;
     }
     else
     {
          if (GPCP_GetValue("val", &v) != 0)
               printf("Unable to get value.\n");
          
          if (GPCP_GetValue("units", &units) != 0)
               printf("Unable to get value.\n");
          
          *val = v;
          //convert_to_si(*val, v, units);
          
          if (GPCP_ExitLevel() != 0)
               printf("Unable to leave level.\n");
     }
     
     return 0;
}

int print_info(prop_t *p, int n)
{

     int i, j, k;
     
     for (i = 0; i < n; i++)
     {
          printf("Propellant Definition.\n");
          printf("Name    : %s\n", p[i].name);
          printf("Density : %f\n", p[i].density);
          printf("Composition\n");

          for (j = 0; j < p[i].ncomp; j++)
          {
               printf("%f mol of %s (%s)\n", p[i].ing[j].qty,
                      p[i].ing[j].formula, p[i].ing[j].name);

               printf("heat of formation: %f\n", p[i].ing[j].heat);
               
               for (k = 0; k < p[i].ing[j].n_elem; k++)
               {
                    printf("  %d %s\n", p[i].ing[j].coef[k],
                           symb[p[i].ing[j].elem[k]]);
                    
               }
          }
     }
     return 0;
}





