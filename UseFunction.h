#ifndef USE_FUNCTION_H
#define USE_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct ThermalTable
{
    float voltage;
    int  temperature;
} ThermalTable_t;

typedef union H_Float
{
    unsigned long data;
    float value;
}H_Float;

float interpolate(float x, ThermalTable_t table[], size_t tableSize);
char *PrintHighLow(int value);
unsigned long FloatToIEEE754_simple(float fInput);
float HexToIEEE754_simple(unsigned long hInput);

#endif
