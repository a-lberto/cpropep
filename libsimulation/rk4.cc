/* rk4.cc  -  Simulation of rocket flight                              */
/* Copyright (C) 2000                                                  */
/* Antoine Lefebvre <antoine.lefebvre@polymtl.ca>                      */

/* This program is free software; you can redistribute it and/or modify*/
/* it under the terms of the GNU General Public License as published by*/
/* the Free Software Foundation; either version 2 of the License, or   */
/* (at your option) any later version.                                 */

/* This program is distributed in the hope that it will be useful,     */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of      */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       */ 
/* GNU General Public License for more details.                        */

/* You should have received a copy of the GNU General Public License   */
/* along with this program; if not, write to the Free Software         */
/* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.           */

#include <stdlib.h>
#include <stdio.h>

#include <cmath>
#include <iostream>
#include <fstream.h>

#include "rk4.h"
#include "c++rocket.h"


static int ModelNeq[MODEL_LAST] = { 7, 5 };


rk4_solver::rk4_solver(Model_t model) 
{
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
 

rk4_solver::~rk4_solver() 
{  
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

  // fill the ans array
  rk4 ( (ModelFunc_t)md, neq, step, duration, st, ans); 
  return length;
}

// print the data to screen
void rk4_solver::print() 
{
  for (int i = 0; i < length; i++) 
  {
    for (int j = 0; j < neq; j++) 
    {
      cout << ans[i][j] << "\t";
    }
    cout << "\n";
  }
}

// export the data in filename with the format need by octave
// for the function load. It will maybe work with matlab to
// but i'm not sure
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


