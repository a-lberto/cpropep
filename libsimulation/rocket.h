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

#if !defined (rocket_h)
#define rocket_h 1

#include <iostream>

#include "c++rocket.h"

// enumeration for data

typedef enum
{
  pression_sol,   // ground pressure in Pa
  masse_molaire,  // molar mass of the atmosphere
  temperature,    // temperature of the atmosphere
  ATM_DATA_LAST
} AtmData;

typedef enum
{
  masse,          // mass of the plenet in Kg
  rayon,          // radius of the planet in m
  PLANETE_DATA_LAST
} PlaneteData;

typedef enum
{
  Kdrag,          // drag coefficient
  Kdd,            // Kd variation with delta
  Klift,          // lift coefficient
  Kspin,          // cross-spin coefficient
  Kmoment,        // moment coefficient
  Kdamping,       // damping coefficient
  dmoment,        // restoring moment asymetry
  dlift,          // lift asymetry
  Diameter,       // diameter of the rocket in m
  Mass,           // rocket mass in  kg (without booster)
  rcm_throat,     // distance between mass center and throat throat
  rcm_exit,       // distance between mass center and 'exit area'
  k,              // radius of geryration about a transverse axis in meter
  // this field should become a function of time
  dt,             // angle of the thrust about the rocket axis
  PARA_ROCKET_LAST
} ParaRocket;

// Classes ********************************************

class propulseur {

  char   name[32];
  double diameter;
  double length;     
  
  double masse_poudre;
  double masse_totale;
  double **thrust;    // table of thrust in function of time
  int    check(void); // function to verify if there is data loaded
 public:
  double burntime;    
  int    ndata;      // number of point in **thrust
  int    load_data(char*); // char* is the file name
  int    print_data(void);
  
  //#ifdef WITH_GTK
  //int plot_data(void);  //plot a graph of the thrust as a fonction of time
  //#endif
  
  propulseur(void) { ndata = 0; }
  ~propulseur(void);
  
  double T(double time); // thrust in function of time
  double M(double time); // mass in function of time
  
  // to be verify
  double impulse(void);  // total impulse of the motor
  double favg(void);     // mean thrust of the motor
  
  double vg(void);       // gaz speed at the exit
  double isp(void);      // specific impulse
  
  double propellant_mass_fraction(void);
  double impulse_to_weight(void);
};



class rocket 
{  
  int nprop;      // nomber of stage
     
 public:
  char name[32];  // name of the rocket
  
  double r_data[PARA_ROCKET_LAST];
  class propulseur prop[MAXPROP];   
  
  // set a stage of the rocket (file is the path name of the data file)
  int set_propulseurs(int stage, char file[128]);  
  int n_prop(void) { return nprop; }
  rocket(void);
  rocket(double *);
  ~rocket() { cout << "Destroying rocket\n"; }
  
};



class atmosphere 
{
 public:
  double a_data[ATM_DATA_LAST];
  atmosphere(void);
  atmosphere(double *);
  ~atmosphere() { cout << "Destroying atmosphere\n";}
};


class planete : public atmosphere 
{
 public:  
  double p_data[PLANETE_DATA_LAST];
  
  double densiteatm(double);   // g/cm^3
  
  planete(void);
  planete(double *);
  planete(double *planete_data, double *atm_data);
  
  ~planete() { cout << "Destroying planete\n"; }
};




class flight_program 
{
 public:
  double ta[MAXPROP]; //temps de l'allumage (depuis le largage du précédent)
  double tl[MAXPROP]; //temps du largage (après la fin de la combustion)
  
  int set_prog(int stage, double allumage, double largage);
  
  flight_program(void);
  ~flight_program(void) { }
};


#endif









