#include <cmath>
#include <iostream>
#include "lsode.h"
//#include "simulation.h"
#include "c++rocket.h"

//typedef int (* ModelFunc)(const int &, const double &, double *, 
//			  double *, int &);

char *error_msg(State s);

static int ModelNeq[MODEL_LAST] = { 7, 5 };

lsode::lsode(Model_t model) {
  
  memory = 0;

  neq = ModelNeq[model];
  //dy = new double*[7];
  time = 0.0;
  
  itol = 1;
  rtol = 1.49012e-08;
  atol = 1.49012e-08;
  itask = 1;
  istate = 1;
  iopt = 0;
  
  rwork = new double[20+16*neq];
  lrw = 20+16*neq;
  iwork = new int[20];
  liw = 20;
  mf = 10;
  state = new double[neq];
  
  if (!rwork) {
    cout << "Allocation error\n";
    exit(-1);
  }
  
  switch (model)
  {
  case AERO_MODEL:
    md = model_1;
    break;
  case SIMPLE_MODEL:
    md = model_2;
    break;
  default:
    exit(-1);
  }
}

lsode::~lsode() {
  
  if (memory) {
    for (int j = 0; j < neq; j++)
      delete [] dy[j];
    delete dy;
  }
  
  delete [] state;
  delete [] iwork;
  delete [] rwork;
  cout << "Freeing memory...\n" ;
}

int lsode::solve(double *st, double duration, double step) 
{
  reset();
  
  if (memory) {
    for (int j = 0; j < neq; j++)
      delete [] dy[j];
    delete dy;
    
    memory = 0;
  }
  
  length = (int)rint(duration/step);
  
  // on alloue la memoire 
  dy = new double*[neq];
  for (int j = 0; j < neq; j++)
    dy[j] = new double[length];
  
  memory = 1;  // pour indiquer que l'on a alloue la memoire
  
     // on simule et on remplie l'array
  for (int i = 0; i < length; i++) {
    tout = i*step;
    
    
    lsode_((ModelFunc_t)md, neq, st, time, tout, itol, rtol, atol, 
	   itask, istate, iopt, rwork, lrw, iwork, liw, NULL, mf);
    
    for (int j = 0; j < neq ; j++)
      dy[j][i] = st[j];
    
    if (istate < 0) // if any error, stop the simulation 
    {
      cout << "ERROR: " << error_msg ((State)istate) << endl;
      cout << i << " points out of " << length << " were found." 
	   << endl;
      return i;
    }
    
  }
  
  cout << "Simulation completed successfully." << endl;
  cout << length << " " << "points computed" << endl;
  
  return length;
}

void lsode::reset()
{
  tout = 0.0; // restart the simulation at 0
  time = 0.0; // restart the time at 0
  istate = 1; // should reset the istate
}

void lsode::print() {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < neq; j++) {
      cout << dy[j][i] << "\t";
    }
    cout << "\n";
  }
}


void lsode::get_data(double** a)
{
  for (int i = 0; i < length; i++)
    for (int j = 0; j < neq; j++)
      a[j][i] = dy[j][i];
}




char *error_msg(State s)
{
  switch (s)
  {
  case TOO_MUCH_WORK:
    return "Excess work done on this call (perhaps wrong mf).";
    break;
    
  case TOO_MUCH_ACC:
    return "Excess Accuracy Requested (Tolerances too small).";
    break;
    
  case ILLEGAL_INPUT:
    return "Illegal input detected.";
    break;
    
  case ERR_FAILURE:
    return "Repeated error test failures.";
    break;
    
  case CONV_FAILURE:
    return "Repeated convergence failures.";
    break;
    
  case ERROR_WEIGHT:
    return "Error weight became zero during problem.";
    break;
    
  case EXIT_IN_FUNCTION:
    return "Exit requested in ode function.";
    break;
    
  default:
    return "Unrecognize error state.";
    
  }
  
}













