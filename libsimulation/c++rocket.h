/* c++rocket.h  -  Simulation of rocket flight                         */
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

#if !defined (cxx_rocket_h)
#define cxx_rocket_h 1

class simulation;

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define MAXPROP 3  // Maximum number of stage on the rocket

// pointer to the actual simulation
extern simulation *simptr;

extern const double R; // Perfect gaz constant
extern const double g; // Earth gravitational accélération
extern const double G; // universal gravitational constant


typedef enum
{
  AERO_MODEL,
  SIMPLE_MODEL,
  MODEL_LAST
} Model_t;

#endif






