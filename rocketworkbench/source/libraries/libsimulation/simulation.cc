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

#include <iostream.h>
#include "simulation.h"

//extern int model_1(const int& neq, const double& time, 
//		   double* z, double* dy, 
//		   int& ierr);

extern simulation *simptr;

simulation::simulation(Model_t model) : rk4_solver(model)
{
  simptr = this;
  sim_name = new char[128];
  //model = model_1;
}
     

simulation::~simulation() 
{
  //delete rocket.prop;
  delete sim_name;
  cout << "Destroying simulation\n";
};
     









