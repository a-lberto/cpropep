#include <iostream>
#include <fstream.h>

#include "c++rocket.h"
#include "rocket.h"
#include "simulation.h"

//extern simulation *simptr;


int main(int argc, char *argv[]) {
 
     simulation muty(AERO_MODEL);

	simulation simple(SIMPLE_MODEL);
	
     //simptr = &muty;

     muty.set_propulseurs(1,"/home/antoine/debian/rocketsim-0.3/thrust/AEJ125.edx");
     muty.set_propulseurs(2,"/home/antoine/debian/rocketsim-0.3/thrust/AEJ180T.eng");
//     muty.set_propulseurs(3,"/home/antoine/debian/rocketsim-0.3/thrust/AEK250W.eng");

     
     muty.set_prog(0,0,0);

     //cout << muty.tl[2] << endl;

     //simptr = &muty;

     double state[7] = {0, 0, 6370000, PI/3, PI/3, 100, 0};

	double state2[5] = {0, 0, 6370000, PI/3, 1000};
     //lsode ode;
     //int flag = ode.solve(state, 10, 0.1);



     muty.solve(state, 500, 0.001);

	simple.solve(state2, 10, 0.01);
	
//     cout << simptr->prop[0].impulse() << endl;
//     cout << simptr->prop[0].favg() << endl;
//     cout << simptr->prop[0].vg() << endl;
//     cout << simptr->prop[0].propellant_mass_fraction() << endl;
//     cout << simptr->prop[0].impulse_to_weight() << endl;
//     cout << simptr->prop[0].isp() << endl;



     //simptr->prop[0].plot_data();

     //for (int i = 0; i<MAXPROP; i++)
     //	  simptr->prop[i].print_data();


     //for (int i = 0; i < 40; i++)
//	  cout << mass((double)i/2) << endl;

     
	//muty.print();
	simple.print();

     //ode.print();
     //simptr->print();


     //cout << flag << endl;


     return 0;
}





