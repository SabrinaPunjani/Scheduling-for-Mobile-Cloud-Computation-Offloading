
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
 * 
 */

/*******************************************************************************/

#include <stdio.h>
#include "simparameters.h"
#include "main.h"
#include "output.h"

/*******************************************************************************/

void
output_blip_to_screen(Simulation_Run_Ptr simulation_run)
{
  double percentagedone;
  Simulation_Run_Data_Ptr data;

  data = (Simulation_Run_Data_Ptr) simulation_run_data(simulation_run);

  data->blip_counter++;

  if((data->blip_counter >= BLIPRATE)
     ||
     (data->number_of_packets_processed >= RUNLENGTH)) {

    data->blip_counter = 0;

    percentagedone =
      100 * (double) data->number_of_packets_processed/RUNLENGTH;

    printf("%3.0f%% ", percentagedone);

    printf("Successfully Xmtted Pkts  = %ld (Arrived Pkts = %ld) \r", 
	   data->number_of_packets_processed, 
	   data->arrival_count);

    fflush(stdout);
  }
}

/**********************************************************************/

void output_results(Simulation_Run_Ptr this_simulation_run)
{
  int i;
  double xmtted_fraction;
  Simulation_Run_Data_Ptr sim_data;

  sim_data = (Simulation_Run_Data_Ptr) simulation_run_data(this_simulation_run);

  printf("\n");
  printf("Random Seed = %d \n", sim_data->random_seed);
  printf("Pkt Arrivals = %ld \n", sim_data->arrival_count);

  xmtted_fraction = (double) sim_data->number_of_packets_processed /
    sim_data->arrival_count;

  printf("Xmtted Pkts  = %ld (Service Fraction = %.5f)\n",
	 sim_data->number_of_packets_processed, xmtted_fraction);

  printf("Mean Delay   = %.1f \n",
	 sim_data->accumulated_delay/sim_data->number_of_packets_processed);

  printf("Mean collisions per packet = %.3f\n",
	 (double) sim_data->number_of_collisions / 
	 sim_data->number_of_packets_processed);
  printf("Number of Stations = %u\n", sim_data->number_of_stations);
  for(i=0; i<sim_data->number_of_stations; i++) {

    printf("Station %2i Mean Delay = %8.1f \n", i,
	   (sim_data->stations+i)->accumulated_delay / 
	   (sim_data->stations+i)->packet_count);
  }
  printf("\n\n");
}



