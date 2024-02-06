#ifndef USE_FUNCTION_H
#define USE_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned short ADC_DCIN_SENSE;
unsigned short ADC_P12VSUS_SENSE;
unsigned short ADC_P12VSUS_ISMON;
unsigned short ADC_TMP_SENSE0;
unsigned short ADC_TMP_SENSE1;
unsigned short ADC_TMP_SENSE2;
unsigned short ADC_TMP_SENSE3;
unsigned short ADC_TMP_SENSE4;
unsigned short ADC_P5VSUS_SENSE;

float Voltage_DCIN_SENSE;
float Voltage_P12VSUS_SENSE;
float Voltage_P12VSUS_ISMON;
float Voltage_P5VSUS_SENSE;

float CURRENT_P12VSUS_ISMON;

float Voltage_TMP_SENSE0;
float Voltage_TMP_SENSE1;
float Voltage_TMP_SENSE2;
float Voltage_TMP_SENSE3;
float Voltage_TMP_SENSE4;

float DEGREE_TMP_SENSE0;
float DEGREE_TMP_SENSE1;
float DEGREE_TMP_SENSE2;
float DEGREE_TMP_SENSE3;
float DEGREE_TMP_SENSE4;

typedef struct ThermalTable
{
    float voltage;
    int  temperature;
} ThermalTable_t;

float interpolate(float x, ThermalTable_t table[], size_t tableSize);

#endif
