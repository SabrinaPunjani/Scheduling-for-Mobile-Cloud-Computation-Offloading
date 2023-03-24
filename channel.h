
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

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

/**********************************************************************/

#include "simlib.h"
#include "simparameters.h"

/**********************************************************************/

typedef enum {IDLE, SUCCESS, COLLISION} Channel_State;

typedef struct _channel_
{
  Channel_State state;
  int transmitting_stn_count;
} Channel, * Channel_Ptr;

/**********************************************************************/

/*
 * Function prototypes
 */

Channel_Ptr
channel_new(void);

Channel_State
get_channel_state(Channel_Ptr);

void
set_channel_state(Channel_Ptr, Channel_State);

int
get_transmitting_stn_count(Channel_Ptr);

void
increment_transmitting_stn_count(Channel_Ptr);

void
decrement_transmitting_stn_count(Channel_Ptr);

void
reset_transmitting_stn_count(Channel_Ptr);

/**********************************************************************/

#endif /* channel.h */






