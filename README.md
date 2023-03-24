# Scheduling-for-Mobile-Cloud-Computation-Offloading

Assume that there are M = 2 mobile devices, one with a good cellular channel connection and one
with a bad one. Remote execution job arrivals occur to the devices via a Poisson arrival process
with a total arrival rate of λ. Each job has an equal probability to arrive at one of the two devices.
The job execution times needed are fixed and given by Jg and Jb for the good and bad connection
devices, respectively. Each job has a fixed cellular upload time for it to reach the cloud server,
given by Ug and Ub for the two devices.
1
When a job arrives to a station, it is placed into a FIFO queue and transmitted to the cellular base
station (with a service time of Ug or Ub). The cellular base station forwards it to the cloud server
with a latency which can be ignored compared to the upload times. Once the job arrives at the
cloud server it is placed in a FIFO queue and executed (using a service time of Jg or Jb).
