
#include <iostream.h>

#include "simulation.h"

extern int model_1(const int& neq, const double& time, 
		   double* z, double* dy, 
		   int& ierr);

extern simulation *simptr;

simulation::simulation(Model_t model) : rk4_solver(model)
{
  simptr = this;
  sim_name = new char[128];
  // model = model_1;
}
     

simulation::~simulation() 
{
  //     delete rocket.prop;
  delete sim_name;
  cout << "Destroying simulation\n";
};
     









