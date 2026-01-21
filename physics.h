#include "fixedpoint.h"

#define MAXOBJECTS 255

typedef enum {rectangle, image} objecttype;

typedef struct {
	bool north; bool east; bool south; bool west;
} direction;

typedef struct {fixedp x; fixedp y;} vector;
typedef struct
{
	bool exists; vector loc; vector size; vector vel; fixedp mass;
	vector bounds0; vector bounds1; bool active; bool collisions;
	ufixedp bounciness; bool bound; direction collisiondirection;
	bool visible; objecttype type; uint16_t objectscriptnumber; bool stasis;
	bool collided;
} object;

object objects[MAXOBJECTS];
uint16_t numofobjects = 0;

/* Adds object and returns object ID */
uint16_t addobject(fixedp x, fixedp y, fixedp w, fixedp h,
		   fixedp mass, uint8_t type)
{
	numofobjects ++;
	uint16_t i;

	for(uint16_t j = 0; j != MAXOBJECTS; j ++)
	{
		if(!objects[j].exists)
		{
			i = j;
			break;
		}
	}

	objects[i].exists	= 1;
	objects[i].active	= 1;
	objects[i].collisions	= 1;
	objects[i].visible	= 1;
	objects[i].stasis	= 0;
	objects[i].loc.x	= x;
	objects[i].loc.y	= y;
	objects[i].size.x	= w;
	objects[i].size.y	= h;
	objects[i].mass		= mass;
	objects[i].type		= type;
	objects[i].bounciness	= int32tofixedp(1) >> 1;

	return(i);
}

void removeobject(uint16_t i)
{
	numofobjects --;
	objects[i].exists	= 0;
}

void setobjectlocation(uint16_t i, fixedp x, fixedp y)
{
	objects[i].loc.x = x;
	objects[i].loc.y = y;
}

void changeobjectlocation(uint16_t i, fixedp x, fixedp y)
{
	objects[i].loc.x += x;
	objects[i].loc.y += y;
}

void setobjectlocationv(uint16_t i, vector loc)
{
	objects[i].loc = loc;
}

void setobjectspeed(uint16_t i, fixedp x, fixedp y)
{
	objects[i].vel.x = x;
	objects[i].vel.y = y;
}

void changeobjectspeed(uint16_t i, fixedp x, fixedp y)
{
	objects[i].vel.x += x;
	objects[i].vel.y += y;
}

void setobjectspeedv(uint16_t i, vector vel)
{
	objects[i].vel = vel;
}

void setobjectmass(uint16_t i, fixedp mass)
{
	objects[i].mass = mass;
}

void changeobjectmass(uint16_t i, fixedp mass)
{
	objects[i].mass += mass;
}

void setobjectsize(uint16_t i, fixedp x, fixedp y)
{
	objects[i].size.x = x;
	objects[i].size.y = y;
}

void changeobjectsize(uint16_t i, fixedp w, fixedp h)
{
	objects[i].size.x = w;
	objects[i].size.y = h;
}

void toggleobjectbounded(uint16_t i)
{
	objects[i].bound = !objects[i].bound;
}

void setobjectbounded(uint16_t i, bool value)
{
	objects[i].bound = value;
}

void setobjectbounds(uint16_t i, fixedp x0, fixedp y0, fixedp x1, fixedp y1)
{
	objects[i].bounds0.x = x0;
	objects[i].bounds0.y = y0;
	objects[i].bounds1.x = x1;
	objects[i].bounds1.y = y1;
}

void setobjectboundsv(uint16_t i, vector b0, vector b1)
{
	objects[i].bounds0 = b0;
	objects[i].bounds1 = b1;
}

void toggleobjectcollisions(uint16_t i)
{
	objects[i].collisions = !objects[i].collisions;
}

void setobjectcollisions(uint16_t i, bool value)
{
	objects[i].collisions = value;
}

void setobjectbounciness(uint16_t i, ufixedp bounciness)
{
	objects[i].bounciness = bounciness;
}

void toggleobjectvisibility(uint16_t i)
{
	objects[i].visible = !objects[i].visible;
}

void setobjectvisibility(uint16_t i, bool value)
{
	objects[i].visible = value;
}

void setobjecttype(uint16_t i, uint8_t type)
{
	objects[i].type = type;
}

void toggleobjectstasis(uint16_t i)
{
	objects[i].stasis = !objects[i].stasis;
}

void setobjectstasis(uint16_t i, bool value)
{
	objects[i].stasis = value;
}

void movement(uint16_t i)
{
	if(!objects[i].stasis)
	{
		objects[i].loc.x += objects[i].vel.x;
		objects[i].loc.y += objects[i].vel.y;
	}
}

fixedp rightside(uint16_t i)
{
	return(objects[i].loc.x + objects[i].size.x);
}

fixedp bottomside(uint16_t i)
{
	return(objects[i].loc.y + objects[i].size.y);
}

void bounding(uint16_t i)
{
	if(rightside(i) > objects[i].bounds1.x)
	{
		objects[i].loc.x = objects[i].bounds1.x - objects[i].size.x;
	}
	if(objects[i].loc.x < objects[i].bounds0.x)
	{
		objects[i].loc.x = objects[i].bounds0.x;
	}
	if(bottomside(i) > objects[i].bounds1.y)
	{
		objects[i].loc.y = objects[i].bounds1.y - objects[i].size.y;
	}
	if(objects[i].loc.y < objects[i].bounds0.y)
	{
		objects[i].loc.y = objects[i].bounds0.y;
	}
}

fixedp bouncescaling(fixedp vel, fixedp i)
{
	return(fixedpmult(i, vel));
}

bool collision(uint16_t i)
{
	uint16_t k;

	/* Check for collisions and their direction */
	for(uint16_t j = 0; j < MAXOBJECTS; j ++)
	{
		if(!objects[j].collisions)
		{
			continue;
		}

		if(rightside(i) > objects[j].loc.x &&
		   objects[i].loc.x < objects[j].loc.x &&
		   bottomside(i) > objects[j].loc.y &&
		   objects[i].loc.y < bottomside(j))
		{
			k = j;
			objects[i].collided = 1;
			objects[i].collisiondirection.east = 1;
			objects[i].loc.x = objects[j].loc.x - objects[i].size.x;
		}
		if(objects[i].loc.x < rightside(j) &&
		   rightside(i) > rightside(j) &&
		   bottomside(i) > objects[j].loc.y &&
		   objects[i].loc.y < bottomside(j))
		{
			k = j;
			objects[i].collided = 1;
			objects[i].collisiondirection.west = 1;
			objects[i].loc.x = objects[j].loc.x + objects[j].size.x;
		}
		if(bottomside(i) > objects[j].loc.y &&
		   objects[i].loc.y < objects[j].loc.y &&
		   rightside(i) > objects[j].loc.x &&
		   objects[i].loc.x < rightside(j))
		{
			k = j;
			objects[i].collided = 1;
			objects[i].collisiondirection.south = 1;
			objects[i].loc.y = objects[j].loc.y - objects[i].size.y;
		}
		if(objects[i].loc.y < bottomside(j) &&
		   bottomside(i) > bottomside(j) &&
		   rightside(i) > objects[j].loc.x &&
		   objects[i].loc.x < rightside(j))
		{
			k = j;
			objects[i].collided = 1;
			objects[i].collisiondirection.north = 1;
			objects[i].loc.y = objects[j].loc.y + objects[j].size.y;
		}
	}

	if(!objects[i].collided)
	{
		return(0);
	}

	/* Apply any collisions found */
	vector energy0, energy1;
	fixedp tmpx, tmpy;
	energy0.x = fixedpabs(fixedpmult(objects[i].vel.x, objects[i].mass));
	energy1.x = fixedpabs(fixedpmult(objects[k].vel.x, objects[k].mass));
	energy0.y = fixedpabs(fixedpmult(objects[i].vel.y, objects[i].mass));
	energy1.y = fixedpabs(fixedpmult(objects[k].vel.y, objects[k].mass));

	if(objects[i].collisiondirection.north ||
	   objects[i].collisiondirection.south)
	{
		tmpy = fixedpdiv((fixedpmult(energy0.y, objects[i].bounciness) +
				  fixedpmult(energy1.y, objects[k].bounciness)),
				  int32tofixedp(2));
		if(energy0.y > energy1.y)
		{
			objects[i].vel.y = -fixedpdiv(tmpy, objects[i].mass);
			objects[k].vel.y =  fixedpdiv(tmpy, objects[k].mass);
		}
		else
		{
			objects[i].vel.y =  fixedpdiv(tmpy, objects[i].mass);
			objects[k].vel.y = -fixedpdiv(tmpy, objects[k].mass);
		}
	}
	if(objects[i].collisiondirection.east ||
	   objects[i].collisiondirection.west)
	{
		tmpx = fixedpdiv((fixedpmult(energy0.x, objects[i].bounciness) +
				  fixedpmult(energy1.x, objects[k].bounciness)),
				  int32tofixedp(2));
		if(energy0.x > energy1.x)
		{
			objects[i].vel.x = -fixedpdiv(tmpx, objects[i].mass);
			objects[k].vel.x =  fixedpdiv(tmpx, objects[k].mass);
		}
		else
		{
			objects[i].vel.x =  fixedpdiv(tmpx, objects[i].mass);
			objects[k].vel.x = -fixedpdiv(tmpx, objects[k].mass);
		}
	}

	objects[i].collided = 0;
	return(1);
}

void objectscript(uint16_t i)
{
	switch(objects[i].objectscriptnumber)
	{
	default:
		break;
	}
}

void physicsloop(void)
{
	for(uint16_t i = 0; i != MAXOBJECTS; i ++)
	{
		if(!objects[i].exists)
		{
			continue;
		}
		objectscript(i);
		if(!objects[i].active)
		{
			continue;
		}
		movement(i);
		if(objects[i].bound)
		{
			bounding(i);
		}
		if(objects[i].collisions)
		{
			collision(i);
		}
	}
}

