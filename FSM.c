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
            printf("Init \n");
            //End INIT-state
            while(elev_get_floor_sensor_signal() == -1){
                elev_set_motor_direction(DIRN_UP);
            }
            elev_set_motor_direction(DIRN_STOP);
            state = IDLE;
            
            /*
            if(elev_get_floor_sensor_signal() != -1){
                elev_set_motor_direction(DIRN_STOP);
                state = IDLE;
            }
            printf("End init \n");
            */
            break;
        
        case IDLE:
            printf("Idle \n");
            //Check if queue contains orders
            //returns 1 if there are any orders, 0 otherwise
            if(Q_check_if_orders()){
                int next_floor = Q_get_next_dir(current_floor, dirn);
                printf("next_floor: %d \n", next_floor);
                if(Q_get_next_dir(current_floor, Q_get_next_floor(current_floor, dirn)) != DIRN_STOP){
                    dirn = Q_get_next_dir(current_floor, Q_get_next_floor(current_floor, dirn));
                } 
                elev_set_motor_direction(dirn);
                state = RUNNING;
            }
            break;
            
        case RUNNING:
            printf("Running \n");
            //Check if floor is ordered
            //returns 1 if ordered, 0 otherwise
            if(Q_should_stop(current_floor, elev_dirn_to_button(dirn))){ 
                printf("motor \n");
                elev_set_motor_direction(DIRN_STOP);
                printf("get next \n");
                if(Q_get_next_dir(current_floor, Q_get_next_floor(current_floor, dirn)) != DIRN_STOP){
                    printf("set direction \n");
                    dirn = Q_get_next_dir(current_floor, Q_get_next_floor(current_floor, dirn));
                    printf("Sets direction to: %d \n", dirn);
                }
                printf("door open \n");
                elev_set_door_open_lamp(1);
                printf("remove order q \n");
                Q_remove_order(current_floor, elev_dirn_to_button(dirn));
                printf("set lamp dirn: %d \n",elev_dirn_to_button(dirn));
                
                elev_set_button_lamp(elev_dirn_to_button(dirn), current_floor, 0);
                printf("set lamp button command \n");
                elev_set_button_lamp(BUTTON_COMMAND, current_floor, 0);
                timer_reset();
                
                state = DOOR_OPEN;
            }
            printf("End running \n");
            break;
            
        case DOOR_OPEN:
            printf("Door open \n");
            //Check if timer is done
            //No need to update state (unless stop is pressed) while countdown
            //returns 1 if done, 0 otherwise
            if(timer_done()){ //kommer ikke inn i hit
                elev_set_door_open_lamp(0);
                printf("Timer done");
                //Check if queue contains orders
                //returns 1 if there are any orders, 0 otherwise
                if(Q_check_if_orders()){
                    //assert(2<1);
                    if(Q_get_next_dir(current_floor, Q_get_next_floor(current_floor, dirn)) != DIRN_STOP){
                        dirn = Q_get_next_dir(current_floor, Q_get_next_floor(current_floor, dirn));
                    }
                    elev_set_motor_direction(dirn);
                    state = RUNNING;
                    printf("End door open, running \n");
                    break;
                }
                
                //setts state to IDLE if no orders
                state = IDLE;
                printf("End door open, idle \n");
            }
            printf("End door open, door open \n");
            break;
            
        case EMERGENCY_STOP:
            printf("Emergency \n");
            //Resets the timer as long as the stop is pressed
            while(elev_get_stop_signal()) {
                timer_reset();
            }
            
            
            //Changes state to door open if elevator is at a floor
            if(elev_get_floor_sensor_signal() != -1){
                elev_set_stop_lamp(0);
                state = DOOR_OPEN;
                printf("End emergency, door open \n");
                break;
            }
            
            
            //Check if stop button still is pressed and if timer is done
            //returns 1 if stop is pressed, 0 otherwise
            if(timer_done()){
                elev_set_stop_lamp(0);
                state = IDLE;
                printf("End emergency, door open \n");
            }
            printf("End emergency \n");
            break;
    }

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

