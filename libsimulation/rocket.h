#if !defined (rocket_h)
#define rocket_h 1

#include <iostream>

#include "c++rocket.h"

// enumeration contenant les donnes possibles *******************************

typedef enum
{
  pression_sol,   // pression au sol en Pa
  masse_molaire,  // masse molaire moyenne de l'atmosphère
  temperature,    // temperature moyenne de l'atmosphere
  ATM_DATA_LAST
} AtmData;

typedef enum
{
  masse,          // masse de la planete en Kg
  rayon,          // rayon de la planete en m
  PLANETE_DATA_LAST
} PlaneteData;

typedef enum
{
  Kdrag,          // drag coefficient
  Kdd,            // pour tenir compte de la variation de Kd avec delta
  Klift,          // lift coefficient
  Kspin,          // cross-spin coefficient
  Kmoment,        // moment coefficient
  Kdamping,       // damping coefficient
  dmoment,        // asymétrie p/r au restoring moment
  dlift,          // asymétrie p/r au lift
  Diameter,       // diametre de la fusée en metres (la charge seulement
  Mass,           // masse de la fusée en kg (sans moteur)
  rcm_throat,     // distance entre le cm et le throat
  rcm_exit,       // distance entre le cm et le 'exit area'
  k,              // radius of geryration about a transverse axis in meter
  // ce champ deviendra une fonction
  dt,             // angle de désalignement de la tuyère p/r à l'axe longitudinal
  PARA_ROCKET_LAST
} ParaRocket;

// Classes ********************************************

class propulseur {

  char name[32];
  double diameter;
  double length;     
  
  double masse_poudre;
  double masse_totale;
  double **thrust; // table des possées ou fonction poussé
  int check(void);
 public:
  double burntime;
  int ndata;
  int load_data(char*); //, double&, double&, int&, int&, double&);
  int print_data(void);
  
#ifdef WITH_GTK
  int plot_data(void);  //plot a graph of the thrust as a fonction of time
#endif
  
  propulseur(void) { ndata = 0; }
  ~propulseur(void);
  
  double T(double time); // poussée en fonction du temps
  double M(double time); // masse en fonction du temps
  
  // À vérifier!!!!!!!!!!!!!
  double impulse(void);  //impulsion totale du moteur
  double favg(void);     //force moyenne du moteur
  //vitesse des gaz
  double vg(void);
  double isp(void);
  
  double propellant_mass_fraction(void);
  double impulse_to_weight(void);
};



class rocket {
  
  int nprop;    // nombre de propulseur
     
 public:
  char name[32];
  
  double r_data[PARA_ROCKET_LAST];
  class propulseur prop[MAXPROP];
  
  int set_propulseurs(int etage, char file[128]);
  int n_prop(void) { return nprop; }
  rocket(void);
  rocket(double *);
  ~rocket() { cout << "Destroying rocket\n"; }
  
};



class atmosphere {
 public:
  double a_data[ATM_DATA_LAST];
  atmosphere(void);
  atmosphere(double *);
  ~atmosphere() { cout << "Destroying atmosphere\n";}
};


class planete : public atmosphere {
 public:  
  double p_data[PLANETE_DATA_LAST];
  
  double densiteatm(double);   // g/cm^3
  
  planete(void);
  planete(double *);
  planete(double *planete_data, double *atm_data);
  
  ~planete() { cout << "Destroying planete\n"; }
};




class flight_program {

 public:
  double ta[MAXPROP]; //temps de l'allumage (depuis le largage du précédent
  double tl[MAXPROP]; //temps du largage (après la fin de la combustion)
  
  int set_prog(int stage, double allumage, double largage);
  
  flight_program(void);
  ~flight_program(void) { }
};


#endif









