//
//  main.c
//  5moduler_tilst
//
//  Created by Thomas Hellum on 02/03/2018.
//  Copyright © 2018 Thomas Hellum. All rights reserved.
//

#include "elev.h"
#include "timer.h"
#include "queue.h"
#include "FSM.h"
#include <stdio.h>



int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    
    printf("Press STOP button to stop elevator and exit program.\n");
    
    
    
    //Initialize elevator position
    elev_set_motor_direction(DIRN_UP);
    FSM_set_direction(DIRN_UP);
    FSM_set_state(INIT);
    
    
    //Elevatorprogram starts
    while (1) {
        int curr_floor_temp = elev_get_floor_sensor_signal();
        if(curr_floor_temp != -1){
            FSM_set_current_floor(curr_floor_temp);
            elev_set_floor_indicator(FSM_get_current_floor());
        }
        
        //Change state to one of either:
        //IDLE = 0,
        //RUNNING = 1,
        //DOOR_OPEN = 2,
        //EMERGENCY_STOP = 3,
        //Takes care of action for all states
        FSM_update_state();        
        
        //Collects all orders
        for(int floor = 0; floor < N_FLOORS; floor++){
            for(int button = 0; button < N_BUTTONS; button++){
                if(!(floor <= FIRST && button == BUTTON_CALL_DOWN)){ //Ønsker kun gyldige calls
                    if(!(floor >= FOURTH && button == BUTTON_CALL_UP)){
                        if(elev_get_button_signal(button, floor)){
                            Q_set_order(floor, button, 1); //setter inn bestilling i set order
                            elev_set_button_lamp(button, floor, 1); //lyser opp bestillingsknapp
                        }
                    }
                }
            }
        }   
    }
    return 0;
}
