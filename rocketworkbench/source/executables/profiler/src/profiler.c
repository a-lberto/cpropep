/*#include <unistd.h>*/
#include <stdlib.h>
#include <string.h>
#include "gpcp.h"
#include "libcompat/include/getopt.h"

#include "profiler.h"

Options rocket_options[] = {
  {"global",       PARENT,  NULL},
  {"length_units", STRING,  "global"},
  {"mass_units",   STRING,  "global"},
  
  {"rocket",       PARENT,  NULL},
  {"name",         STRING,  "rocket"},
  
  {"nose",         PARENT,  "rocket"},
  {"type",         STRING,  "nose"},
  {"length",       FLOAT,   "nose"},
  {"diameter",     FLOAT,   "nose"},
  {"density",      FLOAT,   "nose"},
  
  {"tube",         PARENT,  "rocket"},
  {"length",       FLOAT,   "tube"},
  {"diameter",     FLOAT,   "tube"},
  {"density",      FLOAT,   "tube"},
  
  {"transition",   PARENT,  "rocket"},
  {"length",       FLOAT,   "transition"}, 
  {"density",      FLOAT,   "transition"},
  
  {"fin",          PARENT,  "rocket"},
  {"num",          INTEGER, "fin"},
  {"pos",          FLOAT,   "fin"},
  {"root_chord",   FLOAT,   "fin"},
  {"tip_chord",    FLOAT,   "fin"},
  {"root_tip_d",   FLOAT,   "fin"},
  {"semispan",     FLOAT,   "fin"},
  
  {"motor",        PARENT,  "rocket"},
  {"zpos",         FLOAT,   "motor"},
  {"rpos",         FLOAT,   "motor"},
  {"diameter",     FLOAT,   "motor"},
  {"length",       FLOAT,   "motor"},
  {"empty_mass",   FLOAT,   "motor"},
  {"full_mass",    FLOAT,   "motor"},
  
  {"tank",         PARENT,  "rocket"},
  {"zpos",         FLOAT,   "tank"},
  {"rpos",         FLOAT,   "tank"},
  {"diameter",     FLOAT,   "tank"},
  {"length",       FLOAT,   "tank"},
  {"empty_mass",   FLOAT,   "tank"},
  {"full_mass",    FLOAT,   "tank"},
  
  {"other",        PARENT,  "rocket"},
  {"shape",        STRING,  "other"},
  {"zpos",         FLOAT,   "other"},
  {"rpos",         FLOAT,   "other"},
  {"Ix",           FLOAT,   "other"},
  {"Iy",           FLOAT,   "other"},
  {"mass",         FLOAT,   "other"},
  {NULL,           0,       NULL}
};


int read_value(Data *d);


int main(int argc, char *argv[])
{
  int c;
  Data *data;
  char filename[FILENAME_MAX];
  
  while (1)
  {
    c = getopt(argc, argv, "f:");
    
    if (c == EOF)
      break;
    
    switch (c)
    {
      case 'f':
          if (strlen(optarg) > FILENAME_MAX)
          {
            printf("Filename too long!\n");
            return -1;
            break;
          }
          strncpy (filename, optarg, FILENAME_MAX);
          break;
    } 
  }
  
  GPCP_RegisterOptions(rocket_options);   
  
  if (GPCP_ReadFile(filename, &data))
  {
    printf("Error parsing configuration file.\n");
    return -1;
  }
  
  /*GPCP_PrintData(data);*/

  read_value(data);

  GPCP_FreeData(&data);
  return 0;
};


int read_value(Data *d)
{
  int i, n;

  char *str;
  
  if (GPCP_EnterLevel("global", 0))
  {
    printf("No global value defined.\n");
  }
  else
  {
    if (!GPCP_GetValue("length_units", &str))
      printf("Length units used are: %s\n", str);
    if (!GPCP_GetValue("mass_units", &str))
      printf("Mass units used are  : %s\n", str);
  }

  GPCP_ExitLevel();

  n = GPCP_NumParent("rocket");

  for (i = 0; i < n; i++)
  {
    if (GPCP_EnterLevel("rocket", i))
    {
      printf("Strange, should not be here....\n");
    }
    else
    {
      

      
    }
  }
  return 0;
}




