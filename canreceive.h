#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

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

float RESULT_DCIN_SENSE;
float RESULT_P12VSUS_SENSE;
float CURRENT_P12VSUS_ISMON;
float RESULT_P5VSUS_SENSE;

unsigned char TMS320_SUS_PWR_OK;
unsigned char TMS320_MODULE_POWER_ON;
unsigned char TMS320_BIOS_OK;
unsigned char TMS320_MAIN_PWR_OK;
unsigned char TMS320_MODULE_SHDN_N;
unsigned char TMS320_CARRIER_POWER_ON;


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

unsigned short PC_RAM;
unsigned short PC_CPU0;
unsigned short PC_CPU1;
unsigned short PC_CPU2;
unsigned short PC_CPU3;
unsigned short PC_CPU4;
unsigned short PC_CPU5;
unsigned short PC_CPU6;
unsigned short PC_CPU7;
unsigned long PC_CV0_TEMP;
unsigned long PC_CV1_TEMP;
unsigned long PC_CV2_TEMP;
unsigned long PC_CPU_TEMP;
unsigned long PC_GPU_TEMP;
unsigned long PC_BOARD_TEMP;
unsigned long PC_SOC0_TEMP;
unsigned long PC_SOC1_TEMP;
unsigned long PC_SOC2_TEMP;
unsigned long PC_TDIODE_TEMP;
unsigned long PC_TJ_TEMP;

#endif