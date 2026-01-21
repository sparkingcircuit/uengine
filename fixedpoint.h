#include "bitfiddling.h"

#define SHIFTVALUE 8
#define SHIFTSTANDIN 0xff

typedef uint32_t ufixedp;
typedef int32_t fixedp;

/* Mathmatic operations */
bool checkfixednegative(fixedp i)
{
	return(i >> 31);
}

bool checkint32negitive(int32_t i)
{
	return(i >> 31);
}

fixedp fixedpmult(fixedp a, fixedp b)
{
	return((a * b) >> SHIFTVALUE);
}

fixedp fixedpdiv(fixedp a, fixedp b)
{
	if((a == 0) || (b == 0))
	{
		return(0);
	}
	return((a << SHIFTVALUE) / b);
}

fixedp fixedpabs(fixedp value)
{
	if(value > 0)
	{
		return(value);
	}
	else
	{
		return(-value);
	}
}

/* Type conversions */
ufixedp uint32toufixedp(uint32_t i)
{
	return(i << SHIFTVALUE);
}

uint32_t ufixedptouint32(fixedp i)
{
	return(i >> SHIFTVALUE);
}

fixedp int32tofixedp(int32_t i)
{
	bool neg = checkint32negitive(i);

	i = i << SHIFTVALUE;
	if (neg)
	{
		i |= SHIFTSTANDIN;
	}
	
	return(i);
}

int32_t fixedptoint32(fixedp i)
{
	bool neg = checkfixednegative(i);

	i = i >> SHIFTVALUE;
	if (neg)
	{
		i |= (SHIFTSTANDIN << (32 - SHIFTVALUE));
	}
	return(i);
}
