#ifndef grain_h
#define grain_h


#define MIN_DIST 0.05

#define SQRT_3 1.732050807568877

typedef enum 
{
  BURNING,
  BURNED
} point_state_t;

typedef struct _point
{
  double x;
  double y;
  point_state_t s;
  struct _point *prev;
  struct _point *next;
} point_t;

int adjust_space      (point_t **h);
int fill_space        (point_t *h);
int insert_point      (point_t *p);
int merge_point       (point_t *p);
int delete_next_point (point_t *p);

int create_list(point_t **h, int nb);
int burn_surface(point_t **h, double qt, double radius);
int properties(point_t *h, double *perimeter, double *surface);


#endif
