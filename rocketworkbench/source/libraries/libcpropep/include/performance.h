
#ifndef performance_h
#define performance_h

#include "libcompat/include/compat.h"
#include "return.h"

#include "equilibrium.h"
#include "derivative.h"


int frozen_performance(equilibrium_t *e, exit_condition_t exit_type,
                       double value);
int shifting_performance(equilibrium_t *e, exit_condition_t exit_type,
                         double value);

#endif

