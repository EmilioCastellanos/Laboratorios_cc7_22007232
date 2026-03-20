#ifndef PARKING_H
#define PARKING_H

#include <semaphore.h>

// Config
#define NUM_SPOTS 5
#define NUM_CARS 10
#define MAX_WAIT_TIME 5

// Init / destroy
void init_parking();
void destroy_parking();

// Actions
void car_arrives(int car_id);
void car_parks(int car_id, double wait_time);
void car_leaves(int car_id);

// Stats
void print_stats();

#endif