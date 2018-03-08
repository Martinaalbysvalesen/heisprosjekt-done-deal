//
//  FSM.h
//  5moduler_tilst
//
//  Created by Thomas Hellum on 02/03/2018.
//  Copyright © 2018 Thomas Hellum. All rights reserved.
//

#ifndef FSM_h
#define FSM_h

#include <stdio.h>
#include "elev.h"


typedef enum {
    INIT = -1,
    IDLE = 0,
    RUNNING = 1,
    DOOR_OPEN = 2,
    EMERGENCY_STOP = 3,
} FSM_state;


/*Functions*/

void FSM_update_state();

FSM_state FSM_get_state();
int FSM_get_current_floor();
int FSM_get_direction();

void FSM_set_state(FSM_state set_state);
void FSM_set_current_floor(floor_nr floor);
void FSM_set_direction(int direction);



#endif /* FSM_h */
