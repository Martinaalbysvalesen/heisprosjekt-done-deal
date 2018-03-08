//
//  timer.c
//  5moduler_tilst
//
//  Created by Thomas Hellum on 02/03/2018.
//  Copyright © 2018 Thomas Hellum. All rights reserved.
//

#include "timer.h"

static clock_t timeout_time;

void timer_init( void ) { //unødvendig
    timeout_time = 0;
}

void timer_reset( void ) {
    timeout_time = clock() + TIMEOUT_PERIOD_SECONDS * CLOCKS_PER_SEC;
}

bool timer_done( void ) {
    return (clock() >= timeout_time);
}
