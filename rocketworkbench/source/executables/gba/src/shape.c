
#include <math.h>

#include "grain.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int build_triangular(point_t *h, int nb_point, double c)
{
  int i;
  point_t *ptr;
  
  ptr = h;
  
  for (i = 0; i < nb_point/3; i++)
  {
    ptr->x = (c/2.0) * (3.0*i/nb_point - 1.0);  
    ptr->y = (SQRT_3*c/2.0) * (3.0*i/nb_point - 1.0/3.0);
    ptr->s = BURNING;
    ptr = ptr->next;
  }

  for (i = 0; i < nb_point/3; i++)
  {
    ptr->x = (c/2.0) * (3.0*i/nb_point);
    ptr->y = (SQRT_3*c/2.0) * (2.0/3.0 - 3.0*i/nb_point);
    ptr->s = BURNING;
    ptr = ptr->next;
  }

  for (i = 0; i < nb_point/3; i++)
  {
    ptr->x = c * (1.0/2.0 - 3.0*i/nb_point);
    ptr->y = (SQRT_3*c/2.0) * (-1.0/3.0);
    ptr->s = BURNING;
    ptr = ptr->next;
  }

  return 0;
}

int build_nuclear(point_t *h, int nb_point, double r1, double r2, int n)
{
  int i, j;

  point_t *ptr;
  
  ptr = h;
  
  for (i = 0; i < 2*n; i++)
  {
    if (i%2 == 0)
    {
      for (j = 0; j < nb_point/(2*n); j++)
      {
        ptr->x = r1*sin((i*M_PI/n)+(j*2*M_PI/nb_point));
        ptr->y = r1*cos((i*M_PI/n)+(j*2*M_PI/nb_point));
        ptr->s = BURNING;
        ptr = ptr->next;
      }
    }
    else
    {
      for (j = 0; j < nb_point/(2*n); j++)
      {
        ptr->x = r2*sin((i*M_PI/n)+(j*2*M_PI/nb_point));
        ptr->y = r2*cos((i*M_PI/n)+(j*2*M_PI/nb_point));
        ptr->s = BURNING;
        ptr = ptr->next;
      } 
    }
  }
  return 0;
}

int build_rectangular(point_t *h, int nb_point, double width, double height)
{
  point_t *ptr;

  ptr = h;

  ptr->x = width/2;
  ptr->y = height/2;
  ptr->s = BURNING;

  ptr = ptr->next;

  ptr->x = width/2;
  ptr->y = -height/2;
  ptr->s = BURNING;

  ptr = ptr->next;

  ptr->x = -width/2;
  ptr->y = -height/2;
  ptr->s = BURNING;

  ptr = ptr->next;

  ptr->x = -width/2;
  ptr->y = height/2;
  ptr->s = BURNING;
  
  return 0;
}


int build_cross(point_t *h, int nb_point, double width, double height)
{
  point_t *ptr;

  ptr = h;

  ptr->x = width/2;
  ptr->y = height/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = width/2;
  ptr->y = width/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = height/2;
  ptr->y = width/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = height/2;
  ptr->y = -width/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = width/2;
  ptr->y = -width/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = width/2;
  ptr->y = -height/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = -width/2;
  ptr->y = -height/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = -width/2;
  ptr->y = -width/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = -height/2;
  ptr->y = -width/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = -height/2;
  ptr->y = width/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = -width/2;
  ptr->y = width/2;
  ptr->s = BURNING;
  ptr = ptr->next;

  ptr->x = -width/2;
  ptr->y = height/2;
  ptr->s = BURNING;

  return 0;
}

int build_round(point_t *h, int nb_point, double r)
{

  int i;
  
  double inc;
  double angle;
  
  point_t *ptr;

  ptr = h;

  inc = 2*M_PI/nb_point;

  angle = 0;
  
  for (i = 0; i < nb_point; i++)
  {
    ptr->x = r*cos(angle);
    ptr->y = r*sin(angle);
    ptr->s = BURNING;

    angle -= inc;

    ptr = ptr->next;

  }

  return 0;
}

