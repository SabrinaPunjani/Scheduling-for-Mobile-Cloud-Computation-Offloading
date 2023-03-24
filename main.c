
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "trace.h"
#include "simparameters.h"
#include "cleanup.h"
#include "packet_arrival.h"
#include "packet_transmission.h"
#include "main.h"

/*******************************************************************************/
//Q5
int
main(void)
{


    fclose(fopen("Lab5Part1_results.csv", "w"));
    FILE* fpt;
    fpt = fopen("Lab5Part1_results.csv", "w+");
    fprintf(fpt, "Mean Backoff Duration, Arrival Rate, Mean Delay, mean collisions,Gaurd Time, Stations\n");
    printf("Mean Backoff Duration, Gaurd Time, Mean Delay, Collision per packet, Arrival Rate, Number of Stations\n");
    /* Get the list for varriables defined in simparameters.h */
    unsigned random_seed;
    unsigned RANDOM_SEEDS[] = { RANDOM_SEED_LIST, 0 };

    unsigned mean_backoff_duration;


    unsigned number_of_stations;
    unsigned NUMBER_OF_STATIONSL[] = { NUMBER_OF_STATIONS_LIST, 0 };


    Simulation_Run_Ptr simulation_run;
    Simulation_Run_Data data;
    
    double arrival_rate = PACKET_ARRIVAL_RATE;
    
    while (arrival_rate < 0.11) {
        
        int l = 0;
        while ((number_of_stations = NUMBER_OF_STATIONSL[l++]) != 0) {
            printf("Number of Stations = %u\n", number_of_stations);
            printf("Gaurd time = %.2f\n", GAURD_TIME);
            printf("Arrival Rate = %.2f\n",arrival_rate);


            int i, j = 0;

            /* Do a new simulation_run for each random number generator seed. */
            while ((random_seed = RANDOM_SEEDS[j++]) != 0) {

                /* Set the random generator seed. */
                random_generator_initialize(random_seed);

                /* Create a new simulation_run. This gives a clock and
                    eventlist. Clock time is set to zero. */
                simulation_run = (Simulation_Run_Ptr)simulation_run_new();

                /* Add our data definitions to the simulation_run. */
                simulation_run_set_data(simulation_run, (void*)&data);

                /* Create and initalize the stationsand clould server */
                data.stations = (Station_Ptr)xcalloc((unsigned int)number_of_stations,
                    sizeof(Station));
                data.server = (Station_Ptr)xcalloc(1, sizeof(Station));
                
                /* Initialize various simulation_run variables. */
                data.blip_counter = 0;
                data.arrival_count = 0;
                data.number_of_packets_processed = 0;
                data.number_of_collisions = 0;
                data.accumulated_delay = 0.0;
                data.random_seed = random_seed;
                data.number_of_stations = number_of_stations;
                data.mean_backoff_duration = 10;
                data.arrival_rate = arrival_rate;
                data.server->buffer = fifoqueue_new(); //cloud server buffer
                data.server->packet_count = 0;
                data.server->accumulated_delay = 0.0;
                data.server->mean_delay = 0;
                data.server->id = 0;
                /* Initialize the stations. */
                for (i = 0; i < number_of_stations; i++) {
                    (data.stations + i)->id = i;
                    (data.stations + i)->buffer = fifoqueue_new();
                    (data.stations + i)->packet_count = 0;
                    (data.stations + i)->accumulated_delay = 0.0;
                    (data.stations + i)->mean_delay = 0;
                }
                
                /* Create and initialize the channel. */
                data.channel = channel_new();

                /* Schedule initial packet arrival. */
                schedule_packet_arrival_event(simulation_run,
                    simulation_run_get_time(simulation_run) +
                    exponential_generator((double)1 / data.arrival_rate));

                /* Execute events until we are finished. */
                while (data.number_of_packets_processed < RUNLENGTH) {
                    simulation_run_execute_event(simulation_run);
                }

                /* Print out some results. */
                output_results(simulation_run);
                fprintf(fpt, "%i, %.2f, %.1f, %.3f, %.2f, %u\n", MEAN_BACKOFF_DURATION,
                    data.arrival_rate,
                    data.accumulated_delay / data.number_of_packets_processed, (double)data.number_of_collisions / data.number_of_packets_processed,
                    GAURD_TIME, data.number_of_stations);

                /* Clean up memory. */
                cleanup(simulation_run);



            }
        }
        arrival_rate = arrival_rate + 0.01;
    } printf("done");
    /* Pause before finishing. */
    fclose(fpt);
    getchar();

    return 0;
}












