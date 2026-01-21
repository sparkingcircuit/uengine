#include "config.h"
#include "physics.h"
#include "ramfont.h"
#include "graphics.h"
#include <stdio.h>
#include "game.h"

#define TICKMS  (1000 / TPS)
#define FRAMEMS (1000 / FPS)

/* Declare variables */
uint32_t lasttick  = 0;
uint32_t lastframe = 0;
uint32_t nexttick  = 0;
uint32_t nextframe = 0;

int main(void)
{
	feinitialize();
	gameinitialize();
	while(!windowshouldclose())
	{
		if(nexttick <= timesinceopened())
		{
			lasttick = timesinceopened();
			nexttick = lasttick + TICKMS;
			physicsloop();
		}
		if(nextframe <= timesinceopened())
		{
			lastframe = timesinceopened();
			nextframe = lastframe + FRAMEMS;
			graphicsloop();
		}

		if(nexttick <= nextframe)
		{
			sleepms(timesinceopened() - nexttick);
		}
		else
		{
			sleepms(timesinceopened() - nextframe);
		}
	}
	fedeinitialize();
	return(0);
}

