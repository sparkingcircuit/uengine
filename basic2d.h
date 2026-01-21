#include <string.h>

uint32_t framebufferw = VIEWPORT_RESX;
uint32_t framebufferh = VIEWPORT_RESY;
color*   framebuffer  = *viewport;

color colorpallete[16] = {
	{0x28, 0x28, 0x28}, {0xcc, 0x24, 0x1d}, {0x98, 0x97, 0x1a},
	{0xd7, 0x99, 0x21}, {0x45, 0x85, 0x88}, {0xb1, 0x62, 0x86},
	{0x68, 0x9d, 0x6a}, {0xa8, 0x99, 0x84}, {0x92, 0x83, 0x74},
	{0xfb, 0x49, 0x34}, {0xb8, 0xbb, 0x26}, {0xfa, 0xbd, 0x2f},
	{0x83, 0xa5, 0x98}, {0xd3, 0x86, 0x9b}, {0x8e, 0xc0, 0x7c},
	{0xeb, 0xdb, 0xb2}
};

void setframebuffer(uint32_t w, uint32_t h, color *fb)
{
	framebufferw = w;
	framebufferh = h;
	framebuffer  = fb;
}

int getarrayoffset(uint32_t x, uint32_t y)
{
	return(y + (x * framebufferh));
}

/* Sets the value of a pixel, returns -1 if tries to set pixel not in buffer */
int setpixel(uint32_t x, uint32_t y, color c)
{
	if(x >= framebufferw)
	{
		return(-1);
	}
	if(y >= framebufferh)
	{
		return(-1);
	}

	(framebuffer + getarrayoffset(x, y))->red = c.red;
	(framebuffer + getarrayoffset(x, y))->green = c.green;
	(framebuffer + getarrayoffset(x, y))->blue = c.blue;

	return(0);
}
/*
void drawline(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, color c)
{
	uint32_t xdiff, ydiff, workingnumber;

	if(x1 < x0)
	{
		workingnumber = x1;
		x1 = x0;
		x0 = workingnumber;
	}
	if(y1 < y0)
	{
		workingnumber = y1;
		y1 = y0;
		y0 = workingnumber;
	}

	xdiff = x1 - x0;
	ydiff = y1 - y0;

	setpixel(x0, y0, c);
}
*/
void drawrectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, color c)
{
	for(uint32_t i = x; i < x + w; i ++)
	{
		for(uint32_t j = y; j < y + h; j ++)
		{
			setpixel(i, j, c);
		}
	}
}
/*
void drawcircle()
{

}
*/

#define FONTHEADERLENGTH 4
#define FONTWIDTH	font[0]
#define FONTHEIGHT	font[1]
#define FONTOFFSET	font[2] - 1
#define FONTCHARS	font[3]
int drawcharacter(char value, uint16_t x, uint16_t y, color c, uint8_t* font)
{
	uint16_t ch = value;
	ch -= FONTOFFSET;
	ch *= FONTWIDTH;
	ch -= FONTHEADERLENGTH;

	/* Fail on attempt to draw character without no font data */
	if (value > FONTCHARS + FONTOFFSET || value < FONTOFFSET)
	{
		return(-1);
	}

	for (uint8_t i = 0; i < FONTHEIGHT; i ++)
	{
		for (uint8_t j = 0; j < FONTWIDTH; j ++)
		{
			if (!isbitof8(i, font[ch + j]))
			{
				continue;
			}

			setpixel(x - i + FONTWIDTH, y + j, c);
		}
	}
	return(0);
}

void drawtext(char* string, uint16_t x, uint16_t y, color c, uint8_t* font)
{
	uint16_t dx = x;
	uint16_t dy = y;
	for(uint16_t i = 0; i < strlen(string); i ++)
	{
		if (string[i] == '\n')
		{
			dy += FONTHEIGHT;
			dx = x - FONTWIDTH;
		}
		if (string[i] == '\t')
		{
			dx += (FONTWIDTH << 3) - FONTWIDTH;
		}
		drawcharacter(string[i], dx, dy, c, font);
		dx += FONTWIDTH;
	}
}

/*
void drawimage()
{

}
*/
void clearviewport(color c)
{
	drawrectangle(0, 0, framebufferw, framebufferh, c);
}
