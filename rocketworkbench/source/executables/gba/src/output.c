
#include <stdio.h>
#include "grain.h"

int output_matlab(point_t *h, char *filename, int id)
{
  FILE *fd;
  char header[10];
  point_t *ptr;
  
  ptr = h;
  
  sprintf(header, "S%d = [\n", id);
  fd = fopen(filename, "w");
  fprintf(fd, header);

  do
  {
    fprintf(fd, "% .15f, % .15f\n", ptr->x, ptr->y);
    ptr = ptr->next;
  } while (ptr != h);
  
  fprintf(fd, "% .15f, % .15f\n", ptr->x, ptr->y);
  
  fprintf(fd, "];\n");

  fclose(fd);
  return 0;
}
