
#include <stdio.h>
#include "trace.h"
#include "main.h"
#include "output.h"
#include "job_transmission.h"

long
schedule_end_job_transmission_event(Simulation_Run_Ptr simulation_run,
	double event_time,
	void* link)
{
	Event event;

	event.description = "Job Xmt End";
	event.function = end_job_transmission_event;
	event.attachment = (void*)link;

	return simulation_run_schedule_event(simulation_run, event, event_time);
}

/******************************************************************************/

/*
 * This is the event function which is executed when the end of a packet
 * transmission event occurs. It updates its collected data then checks to see
 * if there are other packets waiting in the fifo queue. If that is the case it
 * starts the transmission of the next packet.
 */

void
end_job_transmission_event(Simulation_Run_Ptr simulation_run,Server_Ptr link)
{
	Simulation_Run_Data_Ptr data;
	Job_Ptr this_job, next_job;

	TRACE(printf("End Of Job.\n"););

	data = (Simulation_Run_Data_Ptr)simulation_run_data(simulation_run);

	/*
	 * Job transmission is finished. Take the job off the data link.
	 */

	this_job = (Job_Ptr)server_get(link);

	/* Collect statistics. */
	if (link == data->link2) {
		data->number_of_jobs_processed++;
	}
	data->accumulated_delay += simulation_run_get_time(simulation_run) -
		this_job->arrive_time;

	/* Output activity blip every so often. */
	output_progress_msg_to_screen(simulation_run);

	/* This job is done ... give the memory back. */
	xfree((void*)this_job);

	/*
	 * See if there is are jobs waiting in the buffer. If so, take the next one
	 * out and transmit it immediately.
	*/

	if (fifoqueue_size(data->queue1) > 0) {
		next_job = (Job_Ptr)fifoqueue_get(data->queue1);
		start_transmission_on_link(simulation_run, next_job, link);
	}
}

/*
 * This function ititiates the transmission of the packet passed to the
 * function. This is done by placing the packet in the server. The packet
 * transmission end event for this packet is then scheduled.
 */

void
start_transmission_on_link(Simulation_Run_Ptr simulation_run,
	Job_Ptr this_job,
	Server_Ptr link)
{
	TRACE(printf("Start Of Packet.\n");)

		server_put(link, (void*)this_job);
	this_job->status = XMTTING;

	/* Schedule the end of packet transmission event. */
	schedule_end_job_transmission_event(simulation_run,
		simulation_run_get_time(simulation_run) + this_job->job_duration,
		(void*)link);
}

/*
 * Get a packet transmission time. For now it is a fixed value defined in
 * simparameters.h
 */

double
get_job_transmission_time(void)
{
	return ((double)JOB_XMT_TIME);
}


