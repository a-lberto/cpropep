#if !defined (lsode_h)
#define lsode_h 1

#include "c++rocket.h"

extern int model_1(const int& neq, const double& time, 
		   double* z, double* dy, 
		   int& ierr);

extern int model_2(const int& neq, const double& time, 
		   double* z, double* dy, 
		   int& ierr);

extern "C" int lsode_ (int (*)(const int&, const double&,
			       double*, double*, int&),
		       int&, double*, double&, double&, int&,
		       double&, double&, int&, int&, int&,
		       double*, int&, int*, int&,
		       int (*)(const int&, const double&,
			       double*, const int&, const int&,
			       double*, const int&),
		       int&);

typedef int (*ModelFunc_t) (const int&, const double&,
			    double*, double*, int&);
     

class lsode
{
     int memory; // est vrai si la memoire a ete alloue
  // ce sont des variables utilises par lsode_
     int neq;
     int length;

     double **dy;

     double time;
     double tout;
     int itol;
     double rtol;
     double atol;
     int itask;
     int istate;
     int iopt;
     double *rwork;
     int lrw;
     int *iwork;
     int liw;
     int mf;
     double *state;

     void *md;

     //int (*func)(const int&, const double&, double*, double*, int&)

     void reset();

 public:

     lsode(Model_t model);
     ~lsode();
     void print();
     void set_atol(double tol) { atol = tol; }
     void set_rtol(double tol) { rtol = tol; }

     int solve(double *st, double duration, double step);

     int get_length(void) { return length; };
     void get_data(double**);

     int get_neq(void) { return neq; };
};


typedef enum 
{ 
     SUCCES = 2,
     TOO_MUCH_WORK = -1, 
     TOO_MUCH_ACC = -2, 
     ILLEGAL_INPUT = -3, 
     ERR_FAILURE = -4, 
     CONV_FAILURE = -5, 
     ERROR_WEIGHT = -6, 
     EXIT_IN_FUNCTION = -13 
}State;
 

#endif


