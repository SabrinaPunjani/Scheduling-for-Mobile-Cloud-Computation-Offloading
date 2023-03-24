
/*
 * 
 * Simulation_Run of the ALOHA Protocol
 * 
 * Copyright (C) 2014 Terence D. Todd
 * Hamilton, Ontario, CANADA
 * todd@mcmaster.ca
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 */

/**********************************************************************/

#ifndef _MAIN_H_
#define _MAIN_H_

/**********************************************************************/

#include "simlib.h"
#include "simparameters.h"
#include "channel.h"

/**********************************************************************/

typedef double Time;
typedef Fifoqueue_Ptr Buffer_Ptr;

/**********************************************************************/

typedef struct _station_
{
  int id;
  Buffer_Ptr buffer;
 
  long int packet_count;
  double accumulated_delay;
  double mean_delay;
} Station, * Station_Ptr;

/**********************************************************************/

typedef enum {WAITING, TRANSMITTING} Packet_Status;

typedef struct _packet_ 
{
  double arrive_time;
  double service_time;
  int station_id;
  Packet_Status status;
  int collision_count;
} Packet, * Packet_Ptr;

typedef struct _simulation_run_data_
{
  Station_Ptr stations;
  Station_Ptr server;
  Channel_Ptr channel;
  long int blip_counter;
  long int arrival_count;
  long int packets_processed;
  long int number_of_packets_processed;
  long int number_of_collisions;
  double accumulated_delay;
  unsigned random_seed;
  unsigned number_of_stations;
  unsigned mean_backoff_duration;
  double arrival_rate;
} Simulation_Run_Data, * Simulation_Run_Data_Ptr;

/**********************************************************************/

/*
 * Function prototypes
 */

int
main(void);

/**********************************************************************/

#endif /* main.h */






