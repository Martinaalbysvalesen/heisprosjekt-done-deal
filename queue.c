//
//  queue.c
//  5moduler_tilst
//
//  Created by Thomas Hellum on 02/03/2018.
//  Copyright © 2018 Thomas Hellum. All rights reserved.
//

#include "queue.h"


/*Variables*/

//All types of orders (4 floors, 3 button types for each floor)
static int floor_orders[N_FLOORS][N_BUTTONS];


/*Functions*/

int Q_check_if_orders(){
    for(int floor = 0; floor < N_FLOORS; floor++){
        for(int button = 0; button < N_BUTTONS; button++){
            if(!(floor <= FIRST && button == BUTTON_CALL_DOWN)){ //Ønsker kun gyldige calls
                if(!(floor >= FOURTH && button == BUTTON_CALL_UP)){
                    if (Q_should_stop(floor, button)){
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}


int Q_should_stop(floor_nr floor, int dirn){
    if (floor_orders[floor][elev_dirn_to_button(dirn)] || floor_orders[floor][BUTTON_COMMAND])
        return 1;
    
    switch (dirn) {
        case DIRN_UP:
            return (Q_iterate_upwards_w_call_down(FIRST) == floor);
            
        case DIRN_DOWN:
            return (Q_iterate_downwards_w_call_up(FOURTH) == floor);
    }
    return 0;
}


void Q_set_order(floor_nr floor, elev_button_type_t button, int value){
    if(!(floor <= FIRST && button == BUTTON_CALL_DOWN)){ //Ønsker kun gyldige calls
        if(!(floor >= FOURTH && button == BUTTON_CALL_UP)){
            floor_orders[floor][button] = value;
        }
    }
}

int Q_get_order(floor_nr floor, elev_button_type_t button){
    return floor_orders[floor][button];
}




//Iterators to make the code simpler and easier to read
int Q_iterate_upwards_w_call_up(floor_nr initial_floor){
    for(int floor = initial_floor; floor < N_FLOORS; floor++){
        if (floor_orders[floor][BUTTON_CALL_UP] || floor_orders[floor][BUTTON_COMMAND]){
            return floor;
        }
    }
    return UNDEFINED;
}


int Q_iterate_upwards_w_call_down(floor_nr initial_floor){
    for(int floor = initial_floor; floor < N_FLOORS; floor++){
        if (floor_orders[floor][BUTTON_CALL_DOWN] || floor_orders[floor][BUTTON_COMMAND]){
            return floor;
        }
    }
    return UNDEFINED;
}


int Q_iterate_downwards_w_call_down(floor_nr initial_floor){
    for(int floor = initial_floor; floor >= 0; floor--){
        if (floor_orders[floor][BUTTON_CALL_DOWN] || floor_orders[floor][BUTTON_COMMAND]){
            return floor;
        }
    }
    return UNDEFINED;
}


int Q_iterate_downwards_w_call_up(floor_nr initial_floor){
    for(int floor = initial_floor; floor >= 0; floor--){
        if (floor_orders[floor][BUTTON_CALL_UP] || floor_orders[floor][BUTTON_COMMAND]){
            return floor;
        }
    }
    return UNDEFINED;
}


//get next floor and direction
int Q_get_next_floor(floor_nr current_floor, int dirn){
    int next_floor;
    
    switch (dirn) {
        case DIRN_UP:
            next_floor = Q_iterate_upwards_w_call_up(current_floor);
            if(next_floor != UNDEFINED){
                printf("1. returns: %d \n", next_floor);
                return next_floor;
            }
            
            next_floor = Q_iterate_downwards_w_call_down(N_FLOORS);
            if(next_floor != UNDEFINED){
                printf("2. returns: %d \n", next_floor);
                return next_floor;
            }
            
            next_floor = Q_iterate_downwards_w_call_up(current_floor);
            if(next_floor != UNDEFINED){
                printf("3. returns: %d \n", next_floor);
                return next_floor;
            }
            
            break;
            
        case DIRN_DOWN:
            next_floor = Q_iterate_downwards_w_call_down(current_floor);
            if(next_floor != UNDEFINED){
                printf("1. returns: %d \n", next_floor);
                return next_floor;
            }
            
            next_floor = Q_iterate_upwards_w_call_up(N_FLOORS);
            if(next_floor != UNDEFINED){
                printf("2. returns: %d \n", next_floor);
                return next_floor;
            }
            
            next_floor = Q_iterate_upwards_w_call_down(current_floor);
            if(next_floor != UNDEFINED){
                printf("3. returns: %d \n", next_floor);
                return next_floor;
            }
            
            break;
        case DIRN_STOP:
            break;
    }
    return UNDEFINED;
}




int Q_get_next_dir(floor_nr current_floor, int next_floor){
    if (next_floor == UNDEFINED)
        return DIRN_STOP;
    if (next_floor > current_floor)
        return DIRN_UP;
    if (next_floor < current_floor)
        return DIRN_DOWN;
    
    return DIRN_STOP;
}



//Fjerne etasjer
void Q_remove_order(floor_nr floor, elev_motor_direction_t dirn){
    floor_orders[floor][BUTTON_COMMAND] = 0;
    if(dirn == DIRN_UP){
        floor_orders[floor][BUTTON_CALL_UP] = 0;
    }
    if (dirn == DIRN_DOWN){
        floor_orders[floor][BUTTON_CALL_DOWN] = 0;
    }
}



void Q_clear_all_orders(){
    for(int floor = 0; floor < N_FLOORS; floor++){
        floor_orders[floor][BUTTON_CALL_DOWN] = 0;
        floor_orders[floor][BUTTON_CALL_UP] = 0;
        floor_orders[floor][BUTTON_COMMAND] = 0;
    }
}
