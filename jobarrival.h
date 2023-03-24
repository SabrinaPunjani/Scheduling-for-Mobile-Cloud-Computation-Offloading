
#ifndef _JobARRIVAL_H_
#define _JobARRIVAL_H_

/******************************************************************************/

#include "simlib.h"


/******************************************************************************/

/*
 * Function prototypes
 */

int
job_arrival_event1(Simulation_Run_Ptr, void*);


int
job_arrival_event2(Simulation_Run_Ptr, void*);

long
schedule_job_arrival_event1(Simulation_Run_Ptr, double);

long
schedule_job_arrival_event2(Simulation_Run_Ptr, double);

/******************************************************************************/

#endif /* jobarrival.h */