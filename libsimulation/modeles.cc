#include <cmath>
#include "simulation.h"

#include "c++rocket.h"

double Atan(double x, double y);
double coef(double v);
double T(double time);
double mass(double time);




int model_1(const int& neq, const double& time, 
	    double* z, double* dy, int& ierr);

int model_2(const int& neq, const double& time, 
	    double* z, double* dy, int& ierr);


extern simulation *simptr;

/**********************************************************************
 * model_1                                                            *
 * modele de simulation de la fus�e en tenant compte des param�tres   *
 * a�rodynamiques                                                     *
 *********************************************************************/


int model_1(const int& neq, const double& time, 
	    double* z, double* dy, int& ierr) 
{
//  double s = z[0];

     double x = z[1];
     double y = z[2];
     double theta = z[3];
     double phi = z[4];
     double v = z[5];
     double dphi = z[6];
     double h = sqrt(pow(x,2) + pow(y,2));
     double g = G*simptr->p_data[masse]/(pow(h,2));
     double delta = phi - theta;
     double rho = simptr->densiteatm(h - simptr->p_data[rayon]);
     double drag_correction = coef(v)/0.15;
     double Drag = -simptr->r_data[Kdrag]*(1+simptr->r_data[Kdd]*pow(delta,2))\
	  *rho*pow(simptr->r_data[Diameter],2)*pow(v,2)*drag_correction;     
     double Lift = simptr->r_data[Klift]*rho*pow(simptr->r_data[Diameter],2) * pow(v,2)*sin(delta - simptr->r_data[dlift]);
     double Spin = simptr->r_data[Kspin]*rho*pow(simptr->r_data[Diameter],3)*v*dphi;
     double Moment = -simptr->r_data[Kmoment]*rho*pow(simptr->r_data[Diameter],3)*pow(v,2)*sin(delta-simptr->r_data[dmoment]);
     double Damping = -simptr->r_data[Kdamping]*rho*pow(simptr->r_data[Diameter],4)*v*dphi;
     
     //il faut changer cette fonction pour tenir compte du plan de vol
     //et pour v�rifier si le propulseur existe (shit)
     double thrust = T(time); //la pousse
     double dmasse = 0; //le debit massique
     
     double angle = Atan(x, y);
    
     dy[0] = v;
     dy[1] = v*cos(theta);
     dy[2] = v*sin(theta);
     dy[3] = (thrust* sin( delta - simptr->r_data[dt] ) - mass(time)*g*cos(theta + PI/2 - angle) + Lift + Spin*cos(delta))/(mass(time)*v); //dtheta
     dy[4] = dphi;
     dy[5] = (thrust*cos(delta-simptr->r_data[dt])-mass(time)*g*sin(theta+PI/2-angle) + Drag + Spin*sin(delta))/mass(time);
     dy[6] = (thrust*simptr->r_data[rcm_throat]*sin(simptr->r_data[dt]) + Moment + Damping - dmasse*dphi*(simptr->r_data[rcm_throat]*simptr->r_data[rcm_exit]-pow(simptr->r_data[k],2)))/(mass(time)*pow(simptr->r_data[k],2));

     return 0;
}




double Atan(double x, double y) {
     double angle;
     if ((x == 0)&&(y > 0))
	  angle = PI/2;
     else if ((x == 0)&&(y < 0))
	  angle = -PI/2;
     else if ((x > 0))
	  angle = atan(y/x);
     else if ((x < 0))
	  angle = PI + atan(y/x);
     return angle;
}


// table de variation des du coefficient de frottement en 
// fonction de la vitesse
double coef(double v) {
     static const int V[] = {0, 50, 100, 150, 200, 250, 300, 350, 400, 450, 
			    500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 
			    1000, 1050, 1100, 1150, 1200};

     static const double C[] = {0.15, 0.13, 0.125, 0.12, 0.135, 0.15, 0.2, 
			       0.3, 0.37, 0.39, 0.38, 0.37, 0.36, 0.35, 0.34, 
			       0.33, 0.32, 0.315, 0.31,0.305, 0.302, 0.3, 
			       0.299, 0.298, 0.297};
     
     if (v > 1200)
	  return 0.295;
     else {
	  int b = (int)floor(v/50)+1;
	  return  ( (C[b+1]-C[b]) / (V[b+1]-V[b]) )*(v-V[b]) + C[b];
     }
}


double mass(double time) {

     double trel = 0;
     double M = simptr->r_data[Mass];

     if (simptr->n_prop() == 0)
	  return M;


     for (int i = 0; i < simptr->n_prop(); i++)
	  M += simptr->prop[i].M(0);


     if (time < simptr->ta[0])
	  return M;

     M -= simptr->prop[0].M(0);
     for (int i = 0; i < simptr->n_prop(); i++)
	  if (time < (simptr->tl[i]+simptr->ta[i]+simptr->prop[i].burntime+trel) )
	       return (M + simptr->prop[i].M(time - trel - simptr->ta[i]));
	  else {
	       trel += simptr->ta[i] + simptr->tl[i] + simptr->prop[i].burntime;
	       M -= simptr->prop[i].M(0);
	  }     
     return simptr->r_data[Mass];
}


//retourne la pouss�e de la fus�e en fonction du temps, des param�tres de vol
//et des moteurs charg�es
double T(double time) {
     double trel = 0;
     if (simptr->n_prop() == 0)
	  return 0.0;
     if (time < simptr->ta[0])
	  return 0.0;
     for (int i = 0; i < simptr->n_prop(); i++) {
	  if (time < (simptr->tl[i]+simptr->ta[i]+simptr->prop[i].burntime+trel) )
	       return simptr->prop[i].T(time - trel - simptr->ta[i]);
	  else
	       trel += simptr->ta[i] + simptr->tl[i] + simptr->prop[i].burntime;
     }	       
     return 0.0;
}



/**********************************************************************
 * model_2                                                            *
 * modele de simulation simple de la fus�e qui ne tient pas compte    *
 * des parametre aerodynamique                                        *
 *********************************************************************/

int model_2(const int& neq, const double& time, 
	    double* z, double* dy, int& ierr) 
{
//  double s = z[0];
     double x = z[1];
     double y = z[2];
     double theta = z[3];
     double v = z[4];

     double h = sqrt(pow(x,2) + pow(y,2));
     double g = G*simptr->p_data[masse]/(pow(h,2));
//     double delta = phi - theta;
     double rho = simptr->densiteatm(h - simptr->p_data[rayon]);
     double drag_correction = coef(v)/0.15;
     double Drag = -simptr->r_data[Kdrag]*rho*pow(simptr->r_data[Diameter],2)*pow(v,2)*drag_correction;     

     //il faut changer cette fonction pour tenir compte du plan de vol
     //et pour v�rifier si le propulseur existe (shit)
     double thrust = T(time); //la pousse
     double dmasse = 0; //le debit massique     
     double angle = Atan(x, y);
    
     dy[0] = v;
     dy[1] = v*cos(theta);
     dy[2] = v*sin(theta);
     dy[3] = (thrust - mass(time)*g*cos(theta+PI/2-angle) )/(mass(time)*v); //dtheta
     dy[4] = (thrust-mass(time)*g*sin(theta+PI/2-angle) + Drag )/mass(time);

     return 0;
}






