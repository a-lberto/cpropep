#include <stdlib.h>
#include <stdio.h>

#include <cmath>
#include <iostream>
#include <fstream.h>
#include "rk4.h"

#include "c++rocket.h"


static int ModelNeq[MODEL_LAST] = { 7, 5 };


rk4_solver::rk4_solver(Model_t model) {

  neq = ModelNeq[model];
  time = 0.0;
  memory = 0;

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
 

rk4_solver::~rk4_solver() {
  
  if (memory) {
    for (int j = 0; j < length; j++)
      delete [] ans[j];
    delete ans;
  }
  cout << "Freeing memory...\n" ;
}

int rk4_solver::solve(double *st, double duration, double step) 
{
  int i;

  length = (int)rint(duration/step) + 1;
  
  // memory allocation
  ans = new double*[length];
  for (int j = 0; j < length; j++)
    ans[j] = new double[neq];
 
  memory = 1; 

  rk4 ( (ModelFunc_t)md, neq, step, duration, st, ans); 
  return length;
}


void rk4_solver::print() {
  for (int i = 0; i < length; i++) 
  {
    for (int j = 0; j < neq; j++) 
    {
      cout << ans[i][j] << "\t";
    }
    cout << "\n";
  }
}

void rk4_solver::export_octave(char *filename)
{
  
  ofstream out(filename);

  out << "# Create by c++rocket" << endl;
  out << "# name: M" << endl;
  out << "# type: matrix" << endl;
  out << "# rows: " << length << endl;
  out << "# columns: " << neq << endl;

  for (int i = 0; i < length; i++) 
  {
    for (int j = 0; j < neq; j++) 
    {
      out << ans[i][j] << " ";
    }
    out << endl;
  }
}
