/* cpropep.c  -  Calculation of Complex Chemical Equilibrium           */
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

#if !defined (simulation_h)
#define simulation_h 1

#include <iostream>
#include "c++rocket.h"
#include "rocket.h"

#include "rk4.h"


//extern int model_1(const int& neq, const double& time, 
//		   double* z, double* dy, 
//		   int& ierr);


class simulation : public flight_program, 
		   public rocket, 
		   public planete,
		   public rk4_solver
		   //public lsode 
{

 public:
     
  char *sim_name;
  
  simulation(Model_t model);
  
  simulation(double* r, double* p, Model_t model ) : 
    rocket(r), planete(p), rk4_solver(model) { 
    cout << "Constructing simulation\n"; 
  }
  
  simulation(double* r, double* p, double* a, Model_t model) :
    rocket(r), planete(p, a), rk4_solver(model) { 
    cout << "Constructing simulation\n"; 
  }
  
  ~simulation(void);
  
  
  //int model_1(const int& neq, const double& time, 
  //		 double* z, double* dy, 
  //		 int& ierr);
  


};

#endif






