#include "basic2d.h"
#include "ui.h"

void graphicsloop(void)
{
	clearviewport(colorpallete[0]);
	for(uint32_t i = 0; i != MAXOBJECTS; i++)
	{
		if(!objects[i].exists)
		{
			break;
		}
		if(!objects[i].visible)
		{
			continue;
		}
		if(objects[i].type == rectangle)
		{
			drawrectangle(ufixedptouint32(objects[i].loc.x),
				      ufixedptouint32(objects[i].loc.y),
				      ufixedptouint32(objects[i].size.x),
				      ufixedptouint32(objects[i].size.y),
				      colorpallete[3]);
		}
		else if(objects[i].type == image)
		{

		}
	}

	renderviewport();
}
