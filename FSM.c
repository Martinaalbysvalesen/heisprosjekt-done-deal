//
//  FSM.c
//  5moduler_tilst
//
//  Created by Thomas Hellum on 02/03/2018.
//  Copyright © 2018 Thomas Hellum. All rights reserved.
//

#include "FSM.h"


#include "FSM.h"
#include "timer.h"
#include "queue.h"
#include <assert.h>
//elev.h included in header

/*Variables*/

//static elev_motor_direction_t motor_dirn;
static int dirn;
static floor_nr current_floor;
static FSM_state state; 


/*Functions*/

//Could have been bulit with a minimized structure, but that would result i a much slower runtime
void FSM_update_state(){

    //Check if stop button is pressed
    //returns 1 if stop is pressed, 0 otherwise
    if(elev_get_stop_signal()){
        elev_set_motor_direction(DIRN_STOP);
        elev_set_stop_lamp(1);
        Q_clear_all_orders();
        elev_cancel_all_lights();
        timer_reset();
        if(elev_get_floor_sensor_signal() != -1){
            elev_set_door_open_lamp(1);
        }
        state = EMERGENCY_STOP;
    }
    
    switch (state){
        case INIT:

            //End INIT-state
            while(elev_get_floor_sensor_signal() == -1){
                elev_set_motor_direction(DIRN_UP);
            }
            elev_set_motor_direction(DIRN_STOP);
            state = IDLE;
            
            break;
        
        case IDLE:
            //Check if queue contains orders
            //returns 1 if there are any orders, 0 otherwise
            if(Q_check_if_orders()){
                int next_floor = Q_get_next_floor(current_floor, dirn);
                if(Q_get_next_dir(current_floor, next_floor) != DIRN_STOP){
                    dirn = Q_get_next_dir(current_floor, next_floor);
                }
                if (current_floor == next_floor)
                        dirn = dirn*(-1); //Change direction

                elev_set_motor_direction(dirn);
                state = RUNNING;
            }
            break;
            
        case RUNNING:
        
            //Check if floor is ordered
            //returns 1 if ordered, 0 otherwise
            if(Q_should_stop(current_floor, dirn) && elev_get_floor_sensor_signal() != -1){ 
                elev_set_motor_direction(DIRN_STOP);
            
                if(Q_get_next_dir(current_floor, Q_get_next_floor(current_floor, dirn)) != DIRN_STOP){
                    dirn = Q_get_next_dir(current_floor, Q_get_next_floor(current_floor, dirn));
                }
                elev_set_door_open_lamp(1);

                timer_reset();
                state = DOOR_OPEN;
            }
            break;
            
        case DOOR_OPEN:

            //Removes orders at once elevator reaches an ordered floor
            Q_remove_order(current_floor);
            elev_set_button_lamp(BUTTON_CALL_UP, current_floor, 0);
            elev_set_button_lamp(BUTTON_CALL_DOWN, current_floor, 0);
            elev_set_button_lamp(BUTTON_COMMAND, current_floor, 0);

            //Check if timer is done
            //No need to update state (unless stop is pressed) while countdown
            //returns 1 if done, 0 otherwise
            if(timer_done()){ //kommer ikke inn i hit
                elev_set_door_open_lamp(0);

                //Check if queue contains orders
                //returns 1 if there are any orders, 0 otherwise
                if(Q_check_if_orders()){
                    int next_floor = Q_get_next_floor(current_floor, dirn);
                    if(Q_get_next_dir(current_floor, next_floor) != DIRN_STOP){
                        dirn = Q_get_next_dir(current_floor, next_floor);
                    }
                    if (current_floor == next_floor)
                        dirn = dirn*(-1); //Change direction

                    elev_set_motor_direction(dirn);
                    state = RUNNING;
                    break;
                }
                
                //setts state to IDLE if no orders
                state = IDLE;
            }
            break;
            
        case EMERGENCY_STOP:

            //Resets the timer as long as the stop is pressed
            while(elev_get_stop_signal()) {
                timer_reset();
            }
            elev_set_stop_lamp(0);

            
            
            //Changes state to door open if elevator is at a floor
            if(elev_get_floor_sensor_signal() != -1){
                elev_set_stop_lamp(0);
                state = DOOR_OPEN;
                break;
            }            
            
            //Check if stop button still is pressed and if timer is done
            //returns 1 if stop is pressed, 0 otherwise
            if(timer_done()){
                state = IDLE;
            }
            break;
    }


/*
    printf("state = %d \n", state);
    printf("direction = %d \n", dirn);
    printf("current floor = %d \n", current_floor);


    for(int floor = 0; floor < N_FLOORS; floor++){
        for(int button = 0; button < N_BUTTONS; button++){
            printf("%d ", Q_get_order(floor, button));            //printer bestillinger for debugging
        }
        printf("\n");
    }

    printf("\n");    
    printf("\n");
*/
}


//get / set
FSM_state FSM_get_state(){
    return state;
}

void FSM_set_state(FSM_state set_state){
    state = set_state;
}



floor_nr FSM_get_current_floor(){
    return current_floor;
}


void FSM_set_current_floor(floor_nr floor){
    current_floor = floor;
}



int FSM_get_direction(){
    return dirn;
}
                                      
void FSM_set_direction(int direction){
    dirn = direction;
}

