#ifndef __RL_FE__
#define __RL_FE__

/* Includes */
#include <stdint.h>
#include <stddef.h>
#include <raylib.h>
#include <sys/time.h>
#include <unistd.h>

/* Define constants */
#define PIXEL_W       WINDOW_RESX / VIEWPORT_RESX
#define PIXEL_H       WINDOW_RESY / VIEWPORT_RESY

/* Define structures */
typedef struct {uint8_t red, green, blue;} color;

/* Define variables */
/* Set to true to close window on next process-loop */
color viewport[VIEWPORT_RESX][VIEWPORT_RESY];
bool shouldclose = 0;
uint32_t starttime;

/* Returns true if application should quit */
bool windowshouldclose(void)
{
	return(shouldclose || WindowShouldClose());
}

uint32_t fegettimestamp(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return(((tv.tv_sec * 1000000) + tv.tv_usec) / 1000);
}

void feinitialize(void)
{
	InitWindow(WINDOW_RESX, WINDOW_RESY, WINDOW_TITLE);
	starttime = fegettimestamp();
}

Color colortofe(color c)
{
	Color C;
	C.r = c.red;
	C.g = c.green;
	C.b = c.blue;
	C.a = 255;
	return(C);
}

void renderviewport(void)
{
	BeginDrawing();

	for(uint16_t x = 0; x != VIEWPORT_RESX; x ++)
	{
		for(uint16_t y = 0; y != VIEWPORT_RESY; y ++)
		{
			DrawRectangle(x * PIXEL_W, y * PIXEL_H,
				      PIXEL_W, PIXEL_H,
				      colortofe(viewport[x][y]));
		}
	}

	EndDrawing();
}

void fedeinitialize(void)
{

}



/* Get time in ms since window opened */
uint32_t timesinceopened(void)
{
	uint32_t currenttime = fegettimestamp();
	return(currenttime - starttime);
}

void sleepms(uint32_t delay)
{
	usleep(delay * 1000);
}

#endif

