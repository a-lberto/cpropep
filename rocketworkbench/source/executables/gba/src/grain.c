
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "shape.h"
#include "grain.h"

#include "output.h"

int burn_surface(point_t **h, double qt, double radius)
{
  int i;
  double x1_0, y1_0, x1_1, y1_1;
  double x1, y1, x2, y2, x3, y3;

  double A, B, C, det, n1, n2, n;

  
  double c_angle; /* cosinus of the angle */
  /*double s_angle;*/ /* sinus of the angle   */
  
  double distance;
  double next_distance;
  
  int itn;
  double nqt;
  
  point_t *ptr1;
  point_t *ptr2;

  /* do itn iteration to have small enough step */
  itn = (int) ceil(3 * qt / MIN_DIST);

  nqt = qt/itn;

  printf("Number of iteration need: %d\n", itn);
  
  for (i = 0; i < itn; i++)
  {

    /* first fill the space */
    adjust_space(h);

    
    ptr1 = *h;
    ptr2 = ptr1->next;
    
    x1_0 = ptr1->x;
    y1_0 = ptr1->y;
    x1_1 = ptr2->x;
    y1_1 = ptr2->y;
    
    do
    {
      
      x1 = ptr1->x;
      y1 = ptr1->y;
      
      x2 = ptr2->x;
      y2 = ptr2->y;
      
      x3 = ptr2->next->x;
      y3 = ptr2->next->y;

      distance      = sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) );
      next_distance = sqrt( pow(x3-x2, 2) + pow(y3-y2, 2) );
      
      /* compute the cosinus of the angle between the two next line segment
         using the scalar product */
      c_angle = ((x2-x1)*(x3-x2) + (y2-y1)*(y3-y2))/(distance * next_distance);
      /* compute the sinus of the angle between the two next line segment
         using the vectorial product */
      /*s_angle = (x1*y2-x2*y1) / (distance * next_distance);*/

      if ((c_angle < 0))/* && (s_angle > 0))*/
      {        
        delete_next_point(ptr1);
        
        if (ptr2 == *h)
          *h = ptr1->next;
        
        ptr2 = ptr1->next;
        x2 = ptr2->x;
        y2 = ptr2->y;
        x3 = ptr2->next->x;
        y3 = ptr2->next->y;
        distance = sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) );
        next_distance = sqrt( pow(x3-x2, 2) + pow(y3-y2, 2) );

        if (distance < MIN_DIST/2.0)
          merge_point(ptr1);

        ptr2 = ptr1->next;
        x1 = ptr1->x;
        y1 = ptr1->y;
        x2 = ptr2->x;
        y2 = ptr2->y;
        x3 = ptr2->next->x;
        y3 = ptr2->next->y;
        distance = sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) );
        next_distance = sqrt( pow(x3-x2, 2) + pow(y3-y2, 2) );
        
      }

      
      /* new point position */
      ptr1->x = (x1+x2)/2.0 + nqt * (y1-y2)/distance;
      ptr1->y = (y1+y2)/2.0 + nqt * (x2-x1)/distance;
      
      /* check if the point has attain the rocket wall */
      if ((pow(ptr1->x, 2) + pow(ptr1->y, 2)) >= pow(radius, 2))
      {
        /* fit the point on the wall */
        
        A = pow(y1-y2, 2) + pow(x2-x1, 2);
        B = (x1+x2)*(y1-y2) + (y1+y2)*(x2-x1);
        C = pow((x1+x2)/2.0, 2) + pow((y1+y2)/2.0, 2) - pow(radius, 2);
        
        det = sqrt(pow(B, 2) - 4*A*C);
        n1 = (-B + det)/(2*A);
        n2 = (-B - det)/(2*A);
 
        n = ((n1 > n2) ? n1 : n2);
        if (isnan(n))
        {
          printf("Error, isnan...\n");
          exit(-1);
        }
        /* new point position */
        ptr1->x = (x1+x2)/2.0 + n * (y1-y2);
        ptr1->y = (y1+y2)/2.0 + n * (x2-x1);
              
        /* the point has finish burning */
        ptr1->s = BURNED;
      }
      
      ptr1 = ptr2;
      ptr2 = ptr2->next;
      
    } while (ptr2 != *h);

    /* the last point is connect with the first one */

    if (ptr1->s == BURNING)
    {
      x1 = ptr1->x;
      y1 = ptr1->y;
      x2 = x1_0;
      y2 = y1_0;
      x3 = x1_1;
      y3 = y1_1;
      
      distance = sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) );
      next_distance = sqrt( pow(x3-x2, 2) + pow(y3-y2, 2) );
      
      /* compute the angle between the two next line segment
         using the scalar product */
      
      c_angle = ((x2-x1)*(x3-x2) + (y2-y1)*(y3-y2))/(distance * next_distance);
      /*s_angle = (x1*y2-x2*y1) / (distance * next_distance); */
      
      if ((c_angle < 0))/* && (s_angle > 0))*/
      {
        delete_next_point(ptr1->prev);
      }
      else
      {
        ptr1->x = (x1+x2)/2 + nqt * (y1-y2)/distance;
        ptr1->y = (y1+y2)/2 + nqt * (x2-x1)/distance;


        if ((pow(ptr1->x, 2) + pow(ptr1->y, 2)) >= pow(radius, 2))
        {
          /* fit the point on the wall */
          A = pow(y1-y2, 2) + pow(x2-x1, 2);
          B = (x1+x2)*(y1-y2) + (y1+y2)*(x2-x1);
          C = pow((x1+x2)/2.0, 2) + pow((y1+y2)/2.0, 2) - pow(radius, 2);

          det = sqrt(pow(B, 2) - 4*A*C);
          n1 = (-B + det)/(2*A);
          n2 = (-B - det)/(2*A);

          n = ((n1 > n2) ? n1 : n2);
          if (isnan(n))
          {
            printf("Error, isnan...\n");
            exit(-1);
          }
          /* new point position */
          ptr1->x = (x1+x2)/2.0 + n * (y1-y2);
          ptr1->y = (y1+y2)/2.0 + n * (x2-x1);

          /* the point has finish burning */
          ptr1->s = BURNED;
        }

        
      }  
    }

    /* verify if there is line crossing for the new surface */
    
  }
  return 0;
}

                      
int create_list(point_t **h, int nb)
{
  int i;
  point_t *ptr;
  
  *h = (point_t *) malloc(sizeof(point_t));
  ptr = *h;

  for (i = 0; i < nb-1; i++)
  {
    ptr->next = (point_t *) malloc(sizeof(point_t));
    ptr->next->prev = ptr;
    ptr = ptr->next;
  }

  ptr->next  = *h; /* the las element point to the first */

  (*h)->prev = ptr;

  return 0;
}

int dealloc_list(point_t **h)
{
  point_t *ptr;
  point_t *next;
  
  ptr = *h;
  next = ptr->next;

  do
  {
    free(ptr);
    ptr = next;
    next = ptr->next;
  } while (ptr != *h);

  return 0;
}
  
int properties(point_t *h, double *perimeter, double *surface)
{
  point_t *ptr1;
  point_t *ptr2;

  double x1, y1, x2, y2;
  
  *perimeter = 0.0;
  *surface   = 0.0;

  ptr1 = h;
  ptr2 = ptr1->next;
    
  do
  {
    x1 = ptr1->x;
    y1 = ptr1->y;
    
    x2 = ptr2->x;
    y2 = ptr2->y;

    /* We can compute the inside surface by the green-riemann theorem
         A = integral (x dy - y dx)/2 on the closed curve */
    *surface   += fabs((x1*(y2-y1) - y1*(x2-x1)))/2;

    if (ptr1->s == BURNING)
      *perimeter += sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) );

    ptr1 = ptr2;
    ptr2 = ptr2->next;
      
  } while (ptr1 != h);

  return 0;
}

    
int adjust_space (point_t **h)
{
  point_t *ptr1, *ptr2;
  double x1, x2, x3, y1, y2, y3;
  double distance;

  double next_distance;

  /* when the first distance is too litle, we go out of the
     while statement because ptr1 point gain to the header,
     so we have to check if the first point was done */
  short first_done = 0;
 
  ptr1 = *h;
  
  ptr2 = ptr1->next;
  do
  {
    x1 = ptr1->x;
    y1 = ptr1->y;
  
    x2 = ptr2->x;
    y2 = ptr2->y;

    x3 = ptr2->next->x;
    y3 = ptr2->next->y;
    
    distance   = sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) );
    
    /* insert new points if the distance is too large */
    if (distance > (3.0*MIN_DIST))
    {
      do
      {
        insert_point(ptr1);
        ptr2 = ptr1->next;
        x2 = ptr2->x;
        y2 = ptr2->y;
        distance = sqrt( pow(x2-x1, 2) + pow(y2-y1, 2) );
      } while (distance > (3.0*MIN_DIST));

      ptr1 = ptr2;
      ptr2 = ptr2->next;
      
    }
    else if (distance < MIN_DIST/2.0)
    {
      next_distance = sqrt( pow(x3-x2, 2) + pow(y3-y2, 2) );

      if (next_distance < MIN_DIST/2.0)
      {
        delete_next_point(ptr1);

        if (ptr2 == *h) /* do not delete the beginning point !! */
        {
          *h = ptr1->next;
        }
        
        ptr2 = ptr1->next;
      }
      else
      {
        ptr1 = ptr2;
        ptr2 = ptr2->next;
      }
    }
    else
    {
      ptr1 = ptr2;
      ptr2 = ptr2->next;
    }

    if (!(first_done) && (ptr1 != *h))
        first_done = 1;
    
  } while ((ptr1 != *h) || !(first_done));

  return 0;
}

int insert_point(point_t *p)
{
  point_t *ptr;
  
  ptr = (point_t *) malloc(sizeof(point_t));

  /* insert the point in the list */
  ptr->next = p->next;
  ptr->next->prev = ptr;
  
  p->next = ptr;
  ptr->prev = p;
  
  /* set the point value in the middle of the other */
  ptr->x = (p->x + ptr->next->x)/2;
  ptr->y = (p->y + ptr->next->y)/2;

  ptr->s = p->s;
  
  return 0;
}

int merge_point(point_t *p)
{

  point_t *ptr;

  ptr = p->next;
  /* The point will be move in the middle of the two points */
  p->x = (p->x + ptr->x)/2;
  p->y = (p->y + ptr->y)/2;

  p->next = ptr->next;
  ptr->next->prev = p;
  
  free(ptr);
  return 0;
}

int delete_next_point(point_t *p)
{
  point_t *ptr;
  ptr = p->next;
  p->next = ptr->next;
  ptr->next->prev = p;
  free(ptr);
  return 0;
}






