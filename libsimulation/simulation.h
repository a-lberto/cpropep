#if !defined (simulation_h)
#define simulation_h 1

#include <iostream>
#include "c++rocket.h"
#include "rocket.h"

#include "lsode.h"



extern int model_1(const int& neq, const double& time, 
		   double* z, double* dy, 
		   int& ierr);





class simulation : public flight_program, 
		   public rocket, 
		   public planete,
		   public lsode 
{

 public:
     
     char *sim_name;

     simulation(Model_t model);

     simulation(double* r, double* p, Model_t model ) : 
	  rocket(r), planete(p), lsode(model) { 
	  cout << "Constructing simulation\n"; 
     }

     simulation(double* r, double* p, double* a, Model_t model) :
	  rocket(r), planete(p, a), lsode(model) { 
	  cout << "Constructing simulation\n"; 
     }

     ~simulation(void);


     //int model_1(const int& neq, const double& time, 
//		 double* z, double* dy, 
//		 int& ierr);
     


};

#endif






