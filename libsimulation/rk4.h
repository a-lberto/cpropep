/* rk4.h   -  Simulation of rocket flight                              */
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
  void *md;   // pointer to the model function
  int  memory;

  double time;

  int    neq;
  int    length;
  double **ans;

 public:
  rk4_solver(Model_t model);
  ~rk4_solver();

  void print();
  void export_octave(char *filename);

  int solve(double *st, double duration, double step);
};


#endif
