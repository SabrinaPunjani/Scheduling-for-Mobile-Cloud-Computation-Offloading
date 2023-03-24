
/*
 * Simulation_Run of the ALOHA Protocol
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
 */

/*******************************************************************************/

#ifndef _PACKET_ARRIVAL_H_
#define _PACKET_ARRIVAL_H_

/*******************************************************************************/

/*
 * Function prototypes
 */

void
packet_arrival_event(Simulation_Run_Ptr, void *);

long int
schedule_packet_arrival_event(Simulation_Run_Ptr, Time);

void
packet_arrival_event2(Simulation_Run_Ptr, void*);

long int
schedule_packet_arrival_event2(Simulation_Run_Ptr, Time, void*);

/*******************************************************************************/

#endif /* packet_arrival.h */






