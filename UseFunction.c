#include "UseFunction.h"

float interpolate(float x, ThermalTable_t table[], size_t tableSize) 
{
    if (table == NULL || tableSize == 0 || x > table[0].voltage || x < table[tableSize - 1].voltage) {
        fprintf(stderr, "Error: voltage is out of table range\n");
        return 0.0;
    }

    size_t i;
    for (i = 1; i < tableSize; ++i) {
        if (x > table[i].voltage) {
            break;
        }
    }

    float x1 = table[i].voltage;
    float y1 = table[i].temperature;
    float x0 = table[i - 1].voltage;
    float y0 = table[i - 1].temperature;

    //printf("x0 : %.4f, y0 : %.4f, x1 : %.4f, y1 : %.4f\n", x0, y0, x1, y1);

    return y0 - (x0 - x) * (y0 - y1) / (x0 - x1);
}

char *PrintHighLow(int value)
{
    char *Result;
    if(value == 0)
        Result = "LOW";
    else
        Result = "HIGH";
        
    return Result;
}

unsigned long FloatToIEEE754_simple(float fInput)
{
    H_Float tmp;
    tmp.value = fInput;
    return tmp.data;
}

float HexToIEEE754_simple(unsigned long hInput)
{
    H_Float tmp;
    tmp.data = hInput;
    return tmp.value;
}

