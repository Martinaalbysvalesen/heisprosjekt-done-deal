//
//  timer.h
//  5moduler_tilst
//
//  Created by Thomas Hellum on 02/03/2018.
//  Copyright © 2018 Thomas Hellum. All rights reserved.
//

#ifndef timer_h
#define timer_h

#include <stdio.h>
#include <stdbool.h>
#include <time.h>


#define TIMEOUT_PERIOD_SECONDS 3


/*Functions*/
void timer_init( void );

void timer_reset( void );
bool timer_done( void );

#endif /* timer_h */
