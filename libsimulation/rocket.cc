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

#include <iostream>
#include <stdio.h>
#include <cmath>
#include "c++rocket.h"
#include "rocket.h"


extern FILE* yyin;  // file pointer for the file parser

// function generate by the flex lexer generator (motor.l)
extern  int motorscan(char *name, double *diameter, 
		      double *length, double *masse_totale, 
		      double *masse_poudre, double **thrust, 
		      int *ndata);


simulation *simptr; // this is the pointer to the active simulation

const double G = 6.67259e-11; // universal gaz constant
const double R = 8.3145;      // perfect gaz constant
const double g = 9.80665;     // Earth gravitational constant




// default value for the initialisation

static double rocket_default[PARA_ROCKET_LAST] = { 0.204,
						   2,
						   2.33,
						   20.4,
						   6.32, 
						   31.6,	
						   0,
						   0,
						   0.1143,
						   16,
						   1,
						   1.1,
						   0.229,
						   0 };


static double planete_default[PLANETE_DATA_LAST] = { 5.98e24,
						     6370000 };


static double atmosphere_default[ATM_DATA_LAST] = { 1.013e5,
						    0.0289644,
						    273.15 };
						    
propulseur::~propulseur()
{
  cout << "Destroying propulseur\n";
  if (!(ndata == 0))
  {
    delete [] thrust[0];
    delete [] thrust[1];
    delete [] thrust;
  }
}

int propulseur::print_data(void) 
{     
  if (!(check()))
    return -1;
  
  cout << "Motor:\t\t\t" << name << endl;
  cout << "Diameter:\t\t" << diameter << " m" << endl;
  cout << "Length:\t\t" << length << " m" << endl;
  cout << "Mass de propergol:\t" << masse_poudre << " Kg" << endl;
  cout << "Total mass:\t\t" << masse_totale << " Kg" << endl;
  //cout << ndata << endl;
  cout << endl;
  cout << "Time (s)\t" << "Thrust (N)" << endl;
  for (int k = 0; k < ndata; k++)
    cout << thrust[0][k] << "\t\t" << thrust[1][k] <<  endl;
  return 0;
}


int propulseur::load_data(char file[32]) 
{
  FILE *file_in = NULL;
  if ((file_in = fopen(file, "r")) == NULL) 
  {
    cout << "No such file or directory " << file << endl;
    return -1;
  }
  yyin = file_in;
  thrust = new double*[2];
  motorscan(name, &diameter, &length, &masse_totale, 
	    &masse_poudre, thrust, &ndata);
   
  fclose(file_in);
  
  burntime = thrust[0][ndata-1];
  return 0;
}


//#ifdef WITH_GTK
//int propulseur::plot_data(void) {
//  cout << "I plot the data...\n";
//  return 0;
//}
//#endif

int propulseur::check(void) 
{
  if (ndata == 0) 
  {
    cout << "No data loaded\n";
    return -1;
  }
  return 1;
}

double propulseur::T(double time) 
{     
  if (!(check()))
    return 0; // no thrust
  
  double pousse;
  
  // there is file with no data at time 0
  if (time < thrust[0][0])
    return (thrust[1][0]/thrust[0][0])*time;
  
  // after the last data, the thrust is 0
  if (time > thrust[0][ndata-1])
    return 0;
  
  // linear interpolation ( should be interesting 
                         // to add quadratic interpolation
  for (int i = 1; i <  ndata; i++)
    if ((thrust[0][i] > time) && (thrust[0][i-1] <= time)) 
    {
      pousse = (thrust[1][i]-thrust[1][i-1])/
	(thrust[0][i]-thrust[0][i-1])*(time-thrust[0][i-1]) + 
	thrust[1][i-1];
    }
  return pousse;
}
		
double propulseur::M(double time) 
{
  if (!(check()))
    return 0;
  
  if (time > burntime)
    return (masse_totale - masse_poudre);
  else 
    return (masse_totale - time*(masse_totale-masse_poudre)/burntime);
  
  return 0;
}
     
double propulseur::impulse(void) 
{  
  if (!(check()))
    return -1;
  
  double I = 0;
  
  for (int i = 0; i < (ndata - 1); i++)
    I += (thrust[1][i]+thrust[1][i+1])*(thrust[0][i+1]-thrust[0][i])/2;
  
  return I;
}

double propulseur::favg(void) 
{
  if (check())
    return (impulse()/burntime);
  return -1;
}

double propulseur::vg(void) 
{
  if (check()) 
    return impulse()/masse_poudre; 
  return -1; 
}

double propulseur::isp(void) 
{
  if (check())
    return vg()/g;
  return -1;
}

double propulseur::propellant_mass_fraction(void) 
{
  if (check())
    return masse_poudre/masse_totale;
  return -1;
}

double propulseur::impulse_to_weight(void) 
{
  if (check())
    return impulse()/(masse_totale*g);
  return -1;
}

rocket::rocket(void) 
{  
  nprop = 0;
  r_data = rocket_default; 
}

rocket::rocket(double *r) 
{
  nprop = 0;
  for (int i = 0; i <  PARA_ROCKET_LAST; i++)
    r_data[i] = r[i];
}

int rocket::set_propulseurs(int stage, char file[128]) 
{  
  if (stage > MAXPROP)
    return -1;
  if (!(stage == 1))
    if (prop[stage-2].ndata == 0) {
      cout << "Stage " << stage-1 << " not loaded.\n";
      return -1;
    }
  
  prop[stage-1].load_data(file);
  nprop = stage;
  
  return 0;
}

planete::planete(void) 
{
  p_data = planete_default;
}

planete::planete(double *p) 
{
  for (int i = 0; i < PLANETE_DATA_LAST; i++)
    p_data[i] = p[i];
}

planete::planete(double *p, double *a) : atmosphere(a) 
{
  for (int i = 0; i < PLANETE_DATA_LAST; i++)
    p_data[i] = p[i];
}


double planete::densiteatm(double altitude) 
{ 
  double d = (((a_data[pression_sol] * a_data[masse_molaire]) / (R*a_data[temperature]))* exp(( (a_data[masse_molaire]*G*p_data[masse])/(R*a_data[temperature]) ) * ( (1/(p_data[rayon] + altitude)) - (1/p_data[rayon]) ) ));
  return d;
}

atmosphere::atmosphere(void) 
{
  a_data = atmosphere_default;
}


atmosphere::atmosphere(double* a) 
{
  for (int i = 0; i < ATM_DATA_LAST; i++)
    a_data[i] = a[i];
}

flight_program::flight_program(void) 
{
  for (int i = 0; i < MAXPROP; i++) 
  {
    ta[i] = 0;
    tl[i] = 0;
  }
}

int flight_program::set_prog(int stage, double allumage, double largage) 
{  
  if ( (stage < 1) || (stage > MAXPROP) )
    return -1;
  
  ta[stage - 1] = allumage;
  tl[stage - 1] = largage;
  
  return 0;
}






