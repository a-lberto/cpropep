#if !defined (cxx_rocket_h)
#define cxx_rocket_h 1

//#include "simulation.h"

class simulation;

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define MAXPROP 3  // le nombre maximal d'étages (propulseurs)

extern simulation *simptr;

extern const double R;      // constante des Gaz parfait
extern const double g;    // accélération gravitationelle de la terre
extern const double G; // contante de gravitation universelle


typedef enum
{
  AERO_MODEL,
  SIMPLE_MODEL,
  MODEL_LAST
} Model_t;

#endif






