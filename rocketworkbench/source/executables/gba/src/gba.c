
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "shape.h"
#include "output.h"
#include "grain.h"

#define STRING_LEN 64

#define STRIDE 0.3

#define NBURN 12

typedef struct _grain_info
{
  double perimeter; /* perimeter of the grain shape */
  double surface;   /* inside surface */
  double depth;     /* thickness of propellant burned */ 
} grain_info_t;

int output_info(grain_info_t *info, int n, char *filename);

int main(int argc, char *argv[])
{
  FILE *octscript;
  
  int i;
  int nb_point;
  
  point_t *beginning; /* pointer to the first point in the structure */

  char string[STRING_LEN];
  char filename[STRING_LEN];
  
  char c[2];
  int d;
  
  grain_info_t info[NBURN+1];

  /* for nuclear */
  /*int n = 8;  
    nb_point = 200 * n;*/

  /* for rectangular */
  /*nb_point = 4;*/

  /* for cross */
  nb_point = 12;

  /* for round */
  /*nb_point = 20;*/
  
  /* generate a random file name */
  snprintf(string, STRING_LEN, "%s", "patate");

  d = 0;
  
  snprintf(filename, STRING_LEN, "%s%s%s", "Z", string, ".m");
  octscript = fopen(filename, "w");
  perror(filename);
            
  create_list(&beginning, nb_point);

  /* fill the shape with a triangular form */
  /*build_triangular(beginning, nb_point, 4);*/
  /*build_nuclear(beginning, nb_point, 0.5, 1, n);*/
  /*build_rectangular(beginning, nb_point, 0.5, 8.0);*/
  build_cross(beginning, nb_point, 1.0, 7.0);
  /* build_round(beginning, nb_point, 1.0);*/

  
  properties(beginning, &(info[0].perimeter), &(info[0].surface));
  info[0].depth = 0.0;
 
 
  snprintf(filename, STRING_LEN, "%s%d%s", string, d, ".m");
          
  output_matlab(beginning, filename, d);

  snprintf(filename, STRING_LEN, "%s%d", string, d);

  fprintf(octscript, "gset nokey\n");
  fprintf(octscript, "%s;\n", filename);
  fprintf(octscript, "plot(S%d(:,1),S%d(:,2))\n", d, d);
  fprintf(octscript, "hold on\n");

  /*c[0]++;*/
  d++;
  
  for (i = 0; i < NBURN; i++)
  {
    burn_surface(&beginning, STRIDE, 5);

    snprintf(filename, STRING_LEN, "%s%d%s", string, d, ".m");
        
    output_matlab(beginning, filename, d);

    snprintf(filename, STRING_LEN, "%s%d", string, d);
    fprintf(octscript, "%s;\n", filename);
    fprintf(octscript, "plot(S%d(:,1),S%d(:,2))\n", d, d);
    
    properties(beginning, &(info[i+1].perimeter), &(info[i+1].surface));

    info[i+1].depth = info[i].depth + STRIDE;
    
    /*c[0]++;*/
    d++;
    
  }  

  output_info(info, NBURN+1, "info.m");
  
  fclose (octscript);

  dealloc_list(&beginning);
  
  return 0;

}


int output_info(grain_info_t *info, int n, char *filename)
{

  FILE *fd;

  int i;
  
  fd = fopen(filename, "w");

  fprintf(fd, "I = [\n");

  for (i = 0; i < n; i++)
  {
    fprintf(fd, "%f %f %f\n", info[i].depth, info[i].perimeter,
            info[i].surface);
  }

  fprintf(fd, "];\n");
  fclose(fd);

  return 0;
}





