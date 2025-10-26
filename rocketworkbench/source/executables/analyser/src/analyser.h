#ifndef analyser_h
#define analyser_h

struct flags
{
  unsigned char a:1, b:1, c:1, d:1, e:1, f:1, g:1, h:1;
};

typedef unsigned char uchar;

typedef struct _motor
{
  uchar isprop;      /* prop mass have bben read */
  uchar istot;       /* total mass have been read */
  
  double prop_mass;  /* Propellant mass (kg)*/
  double total_mass; /* Total engine mass (kg)*/
} motor_t;

typedef struct _results
{
  
  double max_thrust;         /* Maximum thrust */
  double avg_thrust;         /* Mean thrust */
  double burn_time;          /* Burning time */
  double impulse;            /* Total impulse */
  double vg;                 /* Exhaust velocity */
  double isp;                /* Specific impulse */
  double prop_mass_fraction; /* Propellant mass fraction */
  double impulse_to_weight;  /* Impulse to weight ratio */
  
} results_t;

typedef struct _format
{
  int    col;      /* Number of columns */
  int    time;     /* Index of time column */
  double sctime;   /* convesion factor to put time in seconds */
  int    thrust;   /* Index of thrust column */
  double scthrust; /* Conversion factor to put thrust in N */
  char  *comments; /* Character beginning commented lines */
  
} format_t;

void print_results(results_t *res);
int analyse_data(double *data, int n, results_t *res, format_t *f, motor_t *m);
int parse_config(char *configfile, char **datafile, format_t *f, motor_t *m);
int parse_datafile(char *datafile, double **data, int *ndata, format_t *f);


#endif /* analyser_h */
