#if !defined (rk4_h)
#define rk4_h 1

#include "c++rocket.h"

extern int model_1(int neq, double time, 
		   double* z, double* dy, 
		   int ierr);

extern int model_2(int neq, double time, 
		   double* z, double* dy, 
		   int ierr);

typedef int (*ModelFunc_t) (int, double,
			    double*, double*, int);

extern "C" int rk4( int (*f)(int neq, double time, 
			     double *y, double *dy, 
			     int ierr), 
		    int neq, double step, double duration, 
		    double *ic, double **y );

class rk4_solver
{

 private:
  void *md;   // model

  int memory;

  double time;

  int neq;
  int length;
  double **ans;

 public:
  rk4_solver(Model_t model);
  ~rk4_solver();

  void print();
  void export_octave(char *filename);

  int solve(double *st, double duration, double step);
};


#endif
