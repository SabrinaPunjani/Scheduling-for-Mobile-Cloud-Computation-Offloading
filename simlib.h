/*
 * 
 * Simlib Simulation_Run Library
 * 
 * Copyright (C) 2014 Terence D. Todd Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/******************************************************************************/

#ifndef _SIMLIB_H_
#define _SIMLIB_H_

/******************************************************************************/

#include <stdlib.h>

/******************************************************************************/

/*
 * Declare the objects below so that the typedef ordering does not
 * matter.
 */

struct _simulation_run_;
struct _clock_;
struct _event_;
struct _event_container_;
struct _event_list_;

/*
 * Define some convenient typedefs to use when writing simulation_runs.
 *
 * The simulation_run consists of an event list, clock and a pointer for
 * passing user data between various functions.
 */

typedef struct _simulation_run_
{
  struct _eventlist_ * eventlist;
  struct _clock_ * clock;
  void * data;
} Simulation_Run, * Simulation_Run_Ptr;

typedef struct _clock_
{
  double time;
} Clock, * Clock_Ptr;

/*
 * Typedefs defined for various event processing functions. These are used
 * within simlib itself and not by user code.
 */

typedef struct _event_
{
  const char * description;
  void (* function)(struct _simulation_run_*, void *);
  void * attachment;
} Event, * Event_Ptr;

typedef struct _event_container_
{
  struct _event_container_ * next_container;
  struct _event_container_ * previous_container;
  struct _event_ event;
  double occurrence_time;
  void * data_ptr;
  long int event_id;
} Event_Container, * Event_Container_Ptr;

typedef struct _eventlist_
{
  struct _event_container_ * front_ptr;
  struct _event_container_ * back_ptr;
  int size;
} Eventlist, * Eventlist_Ptr;

/******************************************************************************/

/*
 * FIFO queue object keeps the queue size and contains pointers to containers
 * at the front and back of the queue. The queue container objects are kept on
 * a singly linked list. Each container has a content pointer that tracks the
 * object placed on the FIFO queue.
 */

struct _queue_container_;

typedef struct _fifoqueue_
{
  struct _queue_container_ * front_ptr;
  struct _queue_container_ * back_ptr;
  int size;
} Fifoqueue, * Fifoqueue_Ptr;

typedef struct _queue_container_
{
  void * content_ptr;
  struct _queue_container_ * next_ptr;
} Queue_Container, *Queue_Container_Ptr;

/******************************************************************************/

/*
 * Server object and definitions. 
 */

typedef enum{FREE, BUSY} Server_State;

typedef struct _server_
{
  Server_State state;
  void * customer_in_service;
} Server, * Server_Ptr;

/******************************************************************************/

/*
 * Random Number Generation
 *
 * The generator uses the library function rand() which must return an int
 * within the range 1 to RAND_MAX.  Note: RAND_MAX is defined for this purpose
 * in ANSI C.)
 */

/*
 * _rand_stream_ permits having multiple rand() streams at once. Multiple
 * Rand_Stream objects can be created and accessed via rand_stream_get.
 */

typedef struct _rand_stream_
{
  unsigned seed;
  unsigned rand_max;
  unsigned next;
} Rand_Stream, * Rand_Stream_Ptr;

/* #ifndef RAND_MAX
 * #define RAND_MAX 2147483647.0
 * #endif */

/******************************************************************************/

/*
 * Prototypes for functions that are available and defined in simlib.c.
 */

Simulation_Run_Ptr
simulation_run_new(void);

void
simulation_run_execute_event(Simulation_Run_Ptr);

double
simulation_run_get_time(Simulation_Run_Ptr);

void *
simulation_run_data(Simulation_Run_Ptr);

void
simulation_run_set_data(Simulation_Run_Ptr, void*);

/* Create an alias for simulation_run_set_data. */
#define simulation_run_attach_data simulation_run_set_data

long int
simulation_run_schedule_event(Simulation_Run_Ptr, Event, double);

void *
simulation_run_deschedule_event(Simulation_Run_Ptr, long int);

Fifoqueue_Ptr
fifoqueue_new(void);

void
fifoqueue_put(Fifoqueue_Ptr, void*);

void *
fifoqueue_get(Fifoqueue_Ptr);

int
fifoqueue_size(Fifoqueue_Ptr);

void *
fifoqueue_see_front(Fifoqueue_Ptr);

Server_Ptr
server_new(void);

void
server_put(Server_Ptr, void*);

void *
server_get(Server_Ptr);

Server_State
server_state(Server_Ptr);

double
exponential_generator(double);

double
uniform_generator(void);

void
random_generator_initialize(unsigned);

Rand_Stream_Ptr
rand_stream_new(unsigned);

unsigned
rand_stream_get(Rand_Stream_Ptr);

void
rand_stream_initialize(Rand_Stream_Ptr, unsigned);

double
rand_stream_uniform_generator(Rand_Stream_Ptr);

double
rand_stream_exponential_generator(Rand_Stream_Ptr, double);

void *
xmalloc(unsigned);

void *
xcalloc(unsigned, unsigned);

void
xfree(void*);

void
simulation_run_free_memory(Simulation_Run_Ptr);

/******************************************************************************/

#endif /* simlib.h */


