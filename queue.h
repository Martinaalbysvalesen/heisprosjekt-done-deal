//
//  queue.h
//  5moduler_tilst
//
//  Created by Thomas Hellum on 02/03/2018.
//  Copyright © 2018 Thomas Hellum. All rights reserved.
//

#ifndef queue_h
#define queue_h

#include <stdio.h>
#include <stdbool.h>

#include "elev.h"

/*Enums and definitions*/

#define N_FLOORS 4
#define N_BUTTONS 3



//Notice: We declare the enums again to avoid including elev.h and other modules that aren't
//necessary. This is to ensure a clear structure and prevent a chaotic web of communication.



//floor_nr and elev_motor_direction_t are defined i elev.h
//All buttons are set to 1 if order, and 0 if completed

/*Functions*/
int Q_should_stop(floor_nr floor, int dirn);
int Q_check_if_orders();

void Q_set_order(floor_nr floor, elev_button_type_t button, int value);
int Q_get_order(floor_nr floor, elev_button_type_t button);

int Q_iterate_upwards_w_call_up(floor_nr initial_floor);
int Q_iterate_upwards_w_call_down(floor_nr initial_floor);
int Q_iterate_downwards_w_call_down(floor_nr initial_floor);
int Q_iterate_downwards_w_call_up(floor_nr initial_floor);

int Q_get_next_floor(floor_nr current_floor, int dirn);
int Q_get_next_dir(floor_nr current_floor, int next_floor);


//Fjerne bestillinger
void Q_remove_order(floor_nr floor);
void Q_clear_all_orders();


#endif /* queue_h */
