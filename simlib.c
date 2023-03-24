/*
 *
 * Simlib Simulation_Run Library
 *
 * Copyright (C) 2014 Terence D. Todd, Hamilton, Ontario, CANADA,
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "trace.h"
#include "simlib.h"

/*******************************************************************************/

/*
 * Prototype static functions that are local to simlib.
 */

static Clock_Ptr
clock_new(void);

static void
simulation_run_set_time (Simulation_Run_Ptr, double);

static Eventlist_Ptr
eventlist_new(void);

static Eventlist_Ptr
simulation_run_get_eventlist(Simulation_Run_Ptr);

static Event_Container_Ptr
simulation_run_get_event(Simulation_Run_Ptr);

#ifdef TRACE_ON /* This is only used when tracing is active. */
static void event_print_type(Event);
#endif /* TRACE_ON */

/******************************************************************************/

/*
 * Create a new simulation_run. The simulation_run will include a clock, an
 * event list, and a data pointer to simulation_run data.
 */

Simulation_Run_Ptr
simulation_run_new(void)
{
  Simulation_Run_Ptr new_simulation_run;

  new_simulation_run = (Simulation_Run_Ptr) xmalloc(sizeof(Simulation_Run));
  new_simulation_run->eventlist = eventlist_new();
  new_simulation_run->clock = clock_new();
  new_simulation_run->data = NULL;
  return new_simulation_run;
}

/*
 * When a new simulation_run is defined and created, a clock is created which is
 * part of the simulation_run.
 */
 
static
Clock_Ptr clock_new (void)
{
  Clock_Ptr new_clock;

  new_clock = (Clock_Ptr) xmalloc(sizeof(Clock));
  new_clock->time = 0.0;
  return new_clock;
}

/*
 * Given a pointer to a simulation_run, find out the current clock time.
 */

double
simulation_run_get_time (Simulation_Run_Ptr this_simulation_run)
{
  return this_simulation_run->clock->time;
}

/*
 * Given a pointer to a simulation_run, set the clock time.
 */

static
void simulation_run_set_time (Simulation_Run_Ptr this_simulation_run,
			      double time)
{
  this_simulation_run->clock->time = time;
}

/*
 * Given a pointer to a simulation_run, get simulation_run data.
 */

void *
simulation_run_data (Simulation_Run_Ptr this_simulation_run)
{
  return this_simulation_run->data;
}

/*
 * Given a pointer to a simulation_run, put simulation_run data.
 */

void
simulation_run_set_data (Simulation_Run_Ptr this_simulation_run, void * data)
{
  this_simulation_run->data = data;
}


/*
 * This function makes an entry on the event list. It must be passed the
 * simulation_run, the type of event, and the time that the event is to occur. An
 * event_contents pointer can also be passed which can be recovered when the
 * event function is called. The event list itself is a double linked list.
 */

long int
simulation_run_schedule_event(Simulation_Run_Ptr simulation_run,
			      Event new_event, double new_event_time)
{
  Event_Container_Ptr current_container, new_container, 
    next_container;

  double current_time;
  Eventlist_Ptr event_list;
  static long int event_id = 1;

  current_time = simulation_run_get_time(simulation_run);
  event_list = simulation_run_get_eventlist(simulation_run);

  TRACE(printf("At %.3f : ", current_time);)
  TRACE(event_print_type(new_event);)
  TRACE(printf("Scheduled for  %.3f \n", new_event_time);)

  /* Test for time scheduling error. */
  if (new_event_time < current_time) {
    printf("Error: Scheduling backwards in time: ");
    printf("Event time = %f (Clock time = %f) \n", new_event_time, 
    current_time);
    printf("Event scheduled = \"%s\"\n", new_event.description);
    exit(1);
  }

  new_container = (Event_Container_Ptr) xmalloc(sizeof(Event_Container));
  new_container->occurrence_time = new_event_time;
  new_container->event = new_event;
  new_container->next_container = NULL;
  new_container->previous_container = NULL;
  new_container->event_id = event_id;

  if (event_list->size == 0) {
    /* The list is empty. */
    event_list->front_ptr = new_container;
    event_list->back_ptr = new_container;
    event_list->size++;
    return event_id++;
  }

  if (event_list->front_ptr->occurrence_time > new_event_time) {
    /* Add to front of the list. */
    event_list->front_ptr->previous_container = new_container;
    new_container->next_container = event_list->front_ptr;
    event_list->front_ptr = new_container;

    event_list->size++;
    return event_id++;
  }

  if (event_list->back_ptr->occurrence_time <= new_event_time) {
    /* Add to the back of the list. */
    event_list->back_ptr->next_container = new_container;
    new_container->previous_container = event_list->back_ptr;
    event_list->back_ptr = new_container;

    event_list->size++;
    return event_id++;
  }

  /* Add to the middle of the list. */
  current_container = event_list->front_ptr;
  next_container = event_list->front_ptr->next_container;

  while(next_container->occurrence_time <= new_event_time) {
    current_container = next_container;
    next_container = current_container->next_container;
  }
  current_container->next_container = new_container;
  new_container->previous_container = current_container;
  next_container->previous_container = new_container;
  new_container->next_container = next_container;

  event_list->size++;
  return event_id++;
}

/*
 * Given an existing event id, remove the corresponding event from the event
 * list. The event content pointer is returned (which could be NULL). If the
 * requested event does not exist, it will return a NULL pointer.
 */

void *
simulation_run_deschedule_event(Simulation_Run_Ptr simulation_run,
				long int event_id)
{
  int i;
  Event_Container_Ptr current_container, found_container=NULL,
    next_container, previous_container;
  void * content_ptr = NULL;

  Eventlist_Ptr event_list;

  event_list = simulation_run_get_eventlist(simulation_run);

  current_container = event_list->front_ptr;
  next_container = current_container->next_container;

  for(i=0; i<event_list->size; i++) {

    if(current_container->event_id == event_id) {

      found_container = current_container;
      previous_container = found_container->previous_container;
      next_container = found_container->next_container;

      /* Front of list. Adjust the front pointer. */
      if (event_list->front_ptr == found_container)
        event_list->front_ptr = next_container;

      /* Back of list. Adjust the back pointer (could be both fron and
	 back). */
      if (event_list->back_ptr == found_container)
        event_list->back_ptr = previous_container;

      /* If the next event exists, adjust its previous event pointer. */
      if (next_container != NULL)
        next_container->previous_container = previous_container;

      /* If the previous event exists, adjust its next event pointer. */
      if (previous_container != NULL)
        previous_container->next_container = next_container;

      content_ptr = found_container->data_ptr;

      TRACE(printf("At %.2f : ", simulation_run_get_time(simulation_run));)
      TRACE(event_print_type(found_container->event);)
      TRACE(printf("descheduled\n");)

      free((void*) found_container);
      event_list->size--;
      break;
    }
    current_container = current_container->next_container;

  }
  return content_ptr;
}

/*
 * Retrieve the event at the top of the event list, i.e., the next event to
 * occur. This is called by execute_next_event which then passes execution to
 * its event function.
 */

static Event_Container_Ptr
simulation_run_get_event(Simulation_Run_Ptr simulation_run)
{
  Eventlist_Ptr event_list;
  Event_Container_Ptr top_container;

  event_list = simulation_run_get_eventlist(simulation_run);

  if (event_list->size == 0) {
    printf("*** Error: No Events are scheduled ... cannot continue! ***\n");
    exit(1);
  }

  top_container = event_list->front_ptr;

  if (event_list->size == 1) {
    event_list->front_ptr = NULL;
    event_list->back_ptr = NULL;
    event_list->size--;
  } else {

    top_container->next_container->previous_container = NULL;
    event_list->front_ptr = top_container->next_container;
    event_list->size--;
  }
  return (top_container);
}

/*
 * Get the next event from the event list and pass program execution to its
 * event function.
 */

void
simulation_run_execute_event(Simulation_Run_Ptr simulation_run)
{
  Event_Container_Ptr current_container;

  current_container = simulation_run_get_event(simulation_run);
  simulation_run_set_time(simulation_run, 
			  current_container->occurrence_time);

  TRACE(printf("\n");)
  TRACE(event_print_type(current_container->event);)
  TRACE(printf("occurring at %.3f\n", simulation_run_get_time(simulation_run));)

  (*(current_container->event.function))(simulation_run,
			current_container->event.attachment);
  xfree(current_container);
}

/*
 * Free up simulation_run memory.
 */

void
simulation_run_free_memory(Simulation_Run_Ptr this_simulation_run)
{
  Eventlist_Ptr event_list;

  /* Clean out the event list. */
  event_list = this_simulation_run->eventlist;

  while (event_list->size > 0) {
    xfree((void*) simulation_run_get_event(this_simulation_run));
  }

  /* Clean up the simulation_run. */
  xfree(this_simulation_run->eventlist);
  xfree(this_simulation_run->clock);
  xfree(this_simulation_run);
}

/*
 * Functions for handling various event list operations.
 *
 * Create a new event list. This is called when a simulation_run is
 * created. The event list is included in the simulation_run.
 */

static Eventlist_Ptr
eventlist_new(void)
{
  Eventlist_Ptr new_event_list;

  new_event_list = (Eventlist_Ptr) xmalloc(sizeof(Eventlist));

  new_event_list->front_ptr = NULL;
  new_event_list->back_ptr = NULL;
  new_event_list->size = 0;
  return new_event_list;
}

/*
 * Get a pointer to the eventlist. This is intended for use only by simlib.
 */

static Eventlist_Ptr
simulation_run_get_eventlist(Simulation_Run_Ptr simulation_run)
{
  return simulation_run->eventlist;
}

/*
 * Some functions that are only needed if trace is enabled.
 */

#ifdef TRACE_ON

static void
event_print_type(Event event)
{
  printf("%s ", event.description);
}

#endif /* TRACE_ON */

/*
 * FIFO queue functions
 *
 * Make a new (empty) FIFO queue. This will return a pointer to the created
 * Fifoqueue. The FIFO queue is a singly linked list.
 */

Fifoqueue_Ptr
fifoqueue_new(void)
{
  Fifoqueue_Ptr queue_id;

  queue_id = (Fifoqueue_Ptr) xmalloc(sizeof(Fifoqueue));
  queue_id->size = 0;
  queue_id->front_ptr = NULL;
  queue_id->back_ptr  = NULL;
  return queue_id;
}

/*
 * Put something into a FIFO queue. Whatever it is should be cast to a void
 * pointer.
 */

void
fifoqueue_put(Fifoqueue_Ptr queue_ptr, void * content_ptr)
{
  Queue_Container_Ptr queue_container_ptr;

  queue_container_ptr = (Queue_Container_Ptr) xmalloc(sizeof(Queue_Container));
  queue_container_ptr->content_ptr = content_ptr;
  queue_container_ptr->next_ptr = NULL;

  if (queue_ptr->size == 0) {
    queue_ptr->front_ptr = queue_container_ptr;
    queue_ptr->back_ptr =  queue_container_ptr;
  }
  else {
    queue_ptr->back_ptr->next_ptr = queue_container_ptr;
    queue_ptr->back_ptr = queue_container_ptr;
  }
  queue_ptr->size++;
}

/*
 * Take something out of a FIFO queue. Whatever it is should be cast to a void
 * pointer.
 */

void *
fifoqueue_get(Fifoqueue_Ptr queue_ptr)
{
  Queue_Container_Ptr removed_container_ptr;
  void* content_ptr;

  if (queue_ptr->size > 0) {
    removed_container_ptr = queue_ptr->front_ptr;
    queue_ptr->front_ptr = removed_container_ptr->next_ptr;
    content_ptr = removed_container_ptr->content_ptr;
    free((char*) removed_container_ptr);

    if(queue_ptr->size == 1) queue_ptr->back_ptr = NULL;
    queue_ptr->size--;
  }
  else {
    content_ptr = NULL;
  }
  return content_ptr;
}

/*
 * Get the number of objects currently in the Fifoqueue.
 */

int
fifoqueue_size(Fifoqueue_Ptr queue_ptr)
{
  return queue_ptr->size;
}

/*
 * Get a pointer to the object at the front of the Fifoqueue.
 */

void*
fifoqueue_see_front(Fifoqueue_Ptr queue_ptr)
{
  return queue_ptr->front_ptr->content_ptr;
}

/*
 * Server functions.
 *
 * Create and return a pointer to an empty server. The server is marked FREE.
 */

Server_Ptr
server_new(void)
{
  Server_Ptr server_ptr;

  server_ptr = (Server_Ptr) xmalloc(sizeof(Server));
  server_ptr->customer_in_service = NULL;
  server_ptr->state = FREE;
  return server_ptr;
}

/*
 * Pass a server pointer and an object pointer to place the object in the
 * server. The server is marked BUSY.
 */

void
server_put(Server_Ptr server, void* content_ptr)
{
  if (server_state(server) == BUSY) {
      printf("Error: Cannot put into busy server.\n");
      exit(1);
    }

  server->customer_in_service = content_ptr;
  server->state = BUSY;
}

/*
 * Remove whatever is in the server. The simulation_run will exit if this is
 * called when the server is FREE. So the server state should be tested first to
 * be sure.
 */

void *
server_get(Server_Ptr server)
{
  void *entry;

  if (server_state(server) == FREE) {
      printf("Error: Cannot get from free server.\n");
      exit(1);
    }

  entry = server->customer_in_service;
  server->customer_in_service = NULL;
  server->state = FREE;
  return entry;
}

/*
 * Test if the server is FREE or BUSY.
 */

Server_State
server_state(Server_Ptr a_server)
{
  return(a_server->state);
}

/*
 * Random number generator functions.
 */

/*
 * Functions for random streams, which permit multiple overlapping random number
 * generator streams (and seeds) at once.
 */

Rand_Stream_Ptr
rand_stream_new(unsigned seed)
{
  Rand_Stream_Ptr new_stream;

  new_stream = (Rand_Stream_Ptr) xmalloc(sizeof(Rand_Stream));
  rand_stream_initialize(new_stream, seed);
  return new_stream;
}

void
rand_stream_initialize(Rand_Stream_Ptr rand_stream, unsigned seed)
{
  rand_stream->rand_max = 32767;
  rand_stream->seed  = seed;
  rand_stream->next = seed;
}

/*
 * Use our own code for rand so that we can make it thread-safe. This generator
 * is a pretty poor one but sufficient for our purposes.
 */

unsigned
rand_stream_get(Rand_Stream_Ptr rand_stream)
{
  rand_stream->next = rand_stream->next * 1103515245 + 12345;
  return((unsigned)(rand_stream->next/65536) % rand_stream->rand_max);
}

double
rand_stream_uniform_generator(Rand_Stream_Ptr rand_stream)
{
  double r;

  do {
    r = (double) rand_stream_get(rand_stream)/(double)rand_stream->rand_max;
  } while (r == 1 || r == 0);

if (r > 1.0) {
  printf ("***** ERROR: Random() out of bounds! ***** \n");
  exit(0);
 } else return r;
}

double
rand_stream_exponential_generator(Rand_Stream_Ptr rand_stream, double mean)
{
  double u = 0.0;

  while (u == 0.0 || u == 1) u = rand_stream_uniform_generator(rand_stream);
  return -1.0 * log(u) * mean;
}

void
random_generator_initialize(unsigned iseed)
{
  srand(iseed);
}

/*
 * Generate a random number uniformly distributed over (0, 1).
 */

double
uniform_generator(void)
{
  double r;

  do {
    r = (double) rand()/(double) RAND_MAX;
  } while (r == 1 || r == 0);

if (r > 1.0) {
  printf ("***** ERROR: Random() out of bounds! ***** \n");
  exit(0);
 } else return r;
}

/*
 * Generate exponentially distributed random numbers.
 */

double
exponential_generator(double mean)
{
  double u = 0.0;

  while (u == 0.0 || u == 1) u = uniform_generator();
  return -1.0 * log(u) * mean;
}

/*
 * Create a front-end fo malloc that performs out-of-memory testing.
 */

void *
xmalloc(unsigned size)
{
  void * a_ptr;

  if((a_ptr = (void *) malloc(size)) != NULL) return a_ptr;
  else {
    printf("***** ERROR: Out of memory ***** \n");
    exit(1);
  }
}

/*
 * Create a front-end fo calloc that performs out-of-memory testing.
 */

void *
xcalloc(unsigned num, unsigned size)
{
  void * a_ptr;

  if((a_ptr = (void *) calloc(num, size)) != NULL) return a_ptr;
  else {
    printf(" ***** WARNING: Out of memory ***** \n");
    exit(1);
  }
}

/*
 * Create a front-end for free that checks for null pointers.
 */

void
xfree(void * ptr)
{
  if(ptr == NULL) {
    printf("Warning: Attempting to free a NULL pointer.\n");
  }
  else free(ptr);
}




