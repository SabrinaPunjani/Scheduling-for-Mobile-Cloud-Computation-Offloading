#include <math.h>
#include <stdio.h>
#include <math.h>
#include <stdio.h>


#include "jobarrival.h"
#include "main.h"
#include "simparameters.h"
#include "simlib.h"
#include "job_transmission.h"



/******************************************************************************/

/*
 * This function will schedule a job arrival at a time given by
 * event_time. At that time the function "job_arrival" (located in
 * job_arrival.c) is executed. An object can be attached to the event and
 * can be recovered in jobarrival.c.
 */

long int
schedule_job_arrival_event1(Simulation_Run_Ptr simulation_run, //all jobs arrive at first queue / station
	double event_time)
{
	Event event;

	event.description = "Job Arrival";
	event.function = job_arrival_event1;
	event.attachment = (void*)NULL;

	return simulation_run_schedule_event(simulation_run, event, event_time);
}

long int
schedule_job_arrival_event2(Simulation_Run_Ptr simulation_run, 
	double event_time)
{
	Event event;

	event.description = "Job Arrival";
	event.function = job_arrival_event2;
	event.attachment = (void*)NULL;

	return simulation_run_schedule_event(simulation_run, event, event_time);
}
/******************************************************************************/



/*
 * This function will schedule a job arrival at a time given by
 * event_time. At that time the function "job_arrival" (located in
 * job_arrival.c) is executed. An object can be attached to the event and
 * can be recovered in jobarrival.c.
 */

long int
schedule_job_arrival_event(Simulation_Run_Ptr simulation_run,
	double event_time)
{
	Event event;

	event.description = "Job Arrival";
	event.function = job_arrival_event1;
	event.attachment = (void*)NULL;

	return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when a packet arrival event
 * occurs. It creates a new packet object and places it in either the fifo
 * queue if the server is busy. Otherwise it starts the transmission of the
 * packet. It then schedules the next packet arrival event.
 */


void
job_arrival_event1(Simulation_Run_Ptr simulation_run, void* ptr) //arriving at first station 
{
	Simulation_Run_Data_Ptr data;
	Job_Ptr new_job;

	data = (Simulation_Run_Data_Ptr)simulation_run_data(simulation_run);

	data->job_arrival_count1++;

	new_job = (Job_Ptr)xmalloc(sizeof(Job));
	new_job->arrive_time = simulation_run_get_time(simulation_run);
	new_job->job_duration = get_job_transmission_time();

	data->job_arrival_count1++;

	new_job = (Job_Ptr)xmalloc(sizeof(Job));
	new_job->arrive_time = simulation_run_get_time(simulation_run);
	new_job->job_duration = get_job_transmission_time();

	new_job->status = WAITING;

	/*
	 * Start transmission if the data link is free. Otherwise put the packet into
	 * the buffer.
	 */

	if (server_state(data->link1) == BUSY) {
		fifoqueue_put(data->queue1, (void*)new_job);
	}
	else {
		//link1 not busy, we can start transmission
		start_transmission_on_link(simulation_run, new_job, data->link1);




		schedule_packet_arrival_event(simulation_run,
			simulation_run_get_time(simulation_run) +

			exponential_generator((double)1 / JOB_ARRIVAL_RATE));
	}
}

job_arrival_event2(Simulation_Run_Ptr simulation_run, void* ptr) //arriving at second station
{
	Simulation_Run_Data_Ptr data;
	Job_Ptr new_job;

	data = (Simulation_Run_Data_Ptr)simulation_run_data(simulation_run);
	data->job_arrival_count2++;

	new_job = (Job_Ptr)xmalloc(sizeof(Job));
	new_job->arrive_time = simulation_run_get_time(simulation_run);
	new_job->job_duration = get_job_transmission_time(); //ignore upload time between stations 
	new_job->status = WAITING;

	
	if (server_state(data->link2) == BUSY) {
		fifoqueue_put(data->queue2, (void*)new_job);
	}
	else {
		//link2 not busy, we can start transmission
		start_transmission_on_link(simulation_run, new_job, data->link2);
	}


}

/*
 * Get a packet transmission time. Value depends on if good or bad 
 */

double
get_packet_transmission_time(void)
{
	return ((double)JOB_XMT_TIME);
}



/*
		exponential_generator((double)1 / PACKET_ARRIVAL_RATE));
}


*/