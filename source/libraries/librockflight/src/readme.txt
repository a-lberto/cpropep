

eom : equations of motion

aero : compute aerodynamic forces and moments on rocket

compute engine thrust at a given time
int engine(double time, double *y, state_t *s)

How to represent a rocket engine or booster
 - propellant_mass
 - thrust vs time data
 - constant thrust
 - mass of propellant expelled linear with burn time ?
 - constant mass flow rate
 - mass flow rate in linear relation with thrust

 - thrust is in the direction of rocket
 - thrust is not in the direction of rocket
    - position of the exit plane of the nozzle (with reference to CG)
    - direction vector of the thrust

atmos : atmosphere model

mass : compute mass the mass of the rocket at a given time