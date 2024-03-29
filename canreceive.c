
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "UseFunction.h"
#include "ThermalTable.h"

int main(int argc, char **argv)
{
	int s, i; 
	int nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;

	printf("CAN Sockets Receive Demo %s\r\n", argv[1]);

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, argv[1] );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	size_t tableSize = sizeof(Voltage2Temp) / sizeof(Voltage2Temp[0]);

	while(1)
	{
		nbytes = read(s, &frame, sizeof(struct can_frame));
			
		if (nbytes < 0) {
	    	perror("Read");
        	return 1;
 	    }


		
		frame.can_id &= 0x7FFFFFFF;

		if(frame.can_id == 0x1B0A0001)
		{
			ADC_DCIN_SENSE = (frame.data[0] << 8) | frame.data[1];
			ADC_P12VSUS_SENSE = (frame.data[2] << 8) | frame.data[3];
			ADC_P12VSUS_ISMON = (frame.data[4] << 8) | frame.data[5];
			ADC_P5VSUS_SENSE = (frame.data[6] << 8) | frame.data[7];

			Voltage_DCIN_SENSE = ((float)ADC_DCIN_SENSE / pow(2,11)) * 3.3f;
			Voltage_P12VSUS_SENSE = ((float)ADC_P12VSUS_SENSE / pow(2,11)) * 3.3f;
			Voltage_P12VSUS_ISMON = ((float)ADC_P12VSUS_ISMON / pow(2,11)) * 3.3f;
			Voltage_P5VSUS_SENSE = ((float)ADC_P5VSUS_SENSE / pow(2,11)) * 3.3f;

			RESULT_DCIN_SENSE = Voltage_DCIN_SENSE * 32 / 2;
			RESULT_P12VSUS_SENSE = Voltage_P12VSUS_SENSE * 12 / 2;
			CURRENT_P12VSUS_ISMON = (Voltage_P12VSUS_ISMON - 0.25f) * 28.57f;
			RESULT_P5VSUS_SENSE = Voltage_P5VSUS_SENSE * 12 / 2;

			printf("%s %.2fV\r\n", "ADC_DCIN_SENSE", RESULT_DCIN_SENSE);
			printf("%s %.2fV\r\n", "ADC_P12VSUS_SENSE", RESULT_P12VSUS_SENSE);
			printf("%s %.2fmA\r\n", "ADC_P12VSUS_ISMON", CURRENT_P12VSUS_ISMON);
			printf("%s %.2fV\r\n", "ADC_P5VSUS_SENSE", RESULT_P5VSUS_SENSE);
			printf("\r\n");
		}
		else if(frame.can_id == 0x1B0A0002)
		{
			ADC_TMP_SENSE0 = (frame.data[0] << 8) | frame.data[1];
			ADC_TMP_SENSE1 = (frame.data[2] << 8) | frame.data[3];
			ADC_TMP_SENSE2 = (frame.data[4] << 8) | frame.data[5];
			ADC_TMP_SENSE3 = (frame.data[6] << 8) | frame.data[7];

			Voltage_TMP_SENSE0 = ((float)ADC_TMP_SENSE0 / pow(2,11)) * 3.3f;
			Voltage_TMP_SENSE1 = ((float)ADC_TMP_SENSE1 / pow(2,11)) * 3.3f;
			Voltage_TMP_SENSE2 = ((float)ADC_TMP_SENSE2 / pow(2,11)) * 3.3f;
			Voltage_TMP_SENSE3 = ((float)ADC_TMP_SENSE3 / pow(2,11)) * 3.3f;

			DEGREE_TMP_SENSE0 = interpolate(Voltage_TMP_SENSE0, Voltage2Temp, tableSize);
			DEGREE_TMP_SENSE1 = interpolate(Voltage_TMP_SENSE1, Voltage2Temp, tableSize);
			DEGREE_TMP_SENSE2 = interpolate(Voltage_TMP_SENSE2, Voltage2Temp, tableSize);
			DEGREE_TMP_SENSE3 = interpolate(Voltage_TMP_SENSE3, Voltage2Temp, tableSize);


			printf("%s %.2fC\r\n", "ADC_TMP_SENSE0", DEGREE_TMP_SENSE0);
			printf("%s %.2fC\r\n", "ADC_TMP_SENSE1", DEGREE_TMP_SENSE1);
			printf("%s %.2fC\r\n", "ADC_TMP_SENSE2", DEGREE_TMP_SENSE2);
			printf("%s %.2fC\r\n", "ADC_TMP_SENSE3", DEGREE_TMP_SENSE3);
		}
		else if(frame.can_id == 0x1B0A0003)
		{
			ADC_TMP_SENSE4 = (frame.data[0] << 8) | frame.data[1];
			PC_RAM = (frame.data[2] << 8) | frame.data[3];

			Voltage_TMP_SENSE4 = ((float)ADC_TMP_SENSE4 / pow(2,11)) * 3.3f;
			DEGREE_TMP_SENSE4 = interpolate(Voltage_TMP_SENSE4, Voltage2Temp, tableSize);

			printf("%s %.2fC\r\n", "ADC_TMP_SENSE4", DEGREE_TMP_SENSE4);
			printf("%s %d\r\n", "PC_RAM", PC_RAM);
			printf("\r\n");
		}
		else if(frame.can_id == 0x1B0A0004)
		{
			PC_CPU0 = (frame.data[0] << 8) | frame.data[1];
			PC_CPU1 = (frame.data[2] << 8) | frame.data[3];
			PC_CPU2 = (frame.data[4] << 8) | frame.data[5];
			PC_CPU3 = (frame.data[6] << 8) | frame.data[7];

			printf("%s %d%%\r\n", "PC_CPU0", PC_CPU0);
			printf("%s %d%%\r\n", "PC_CPU1", PC_CPU1);
			printf("%s %d%%\r\n", "PC_CPU2", PC_CPU2);
			printf("%s %d%%\r\n", "PC_CPU3", PC_CPU3);
		}
		else if(frame.can_id == 0x1B0A0005)
		{
			PC_CPU4 = (frame.data[0] << 8) | frame.data[1];
			PC_CPU5 = (frame.data[2] << 8) | frame.data[3];
			PC_CPU6 = (frame.data[4] << 8) | frame.data[5];
			PC_CPU7 = (frame.data[6] << 8) | frame.data[7];

			printf("%s %d%%\r\n", "PC_CPU4", PC_CPU4);
			printf("%s %d%%\r\n", "PC_CPU5", PC_CPU5);
			printf("%s %d%%\r\n", "PC_CPU6", PC_CPU6);
			printf("%s %d%%\r\n", "PC_CPU7", PC_CPU7);
		}
		else if(frame.can_id == 0x1B0A0006)
		{
			PC_CV0_TEMP = (frame.data[0] << 24) | (frame.data[1] << 16) | (frame.data[2] << 8) | frame.data[3];
			PC_CV1_TEMP = (frame.data[4] << 24) | (frame.data[5] << 16) | (frame.data[6] << 8) | frame.data[7];

			printf("%s 0x%08X %.3f\r\n", "PC_CV0_TEMP", PC_CV0_TEMP, HexToIEEE754_simple(PC_CV0_TEMP));
			printf("%s 0x%08X %.3f\r\n", "PC_CV1_TEMP", PC_CV1_TEMP, HexToIEEE754_simple(PC_CV1_TEMP));
		}
		else if(frame.can_id == 0x1B0A0007)
		{
			PC_CV2_TEMP = (frame.data[0] << 24) | (frame.data[1] << 16) | (frame.data[2] << 8) | frame.data[3];
			PC_CPU_TEMP = (frame.data[4] << 24) | (frame.data[5] << 16) | (frame.data[6] << 8) | frame.data[7];

			printf("%s 0x%08X %.3f\r\n", "PC_CV2_TEMP", PC_CV0_TEMP, HexToIEEE754_simple(PC_CV2_TEMP));
			printf("%s 0x%08X %.3f\r\n", "PC_CPU_TEMP", PC_CV1_TEMP, HexToIEEE754_simple(PC_CPU_TEMP));
		}
		else if(frame.can_id == 0x1B0A0008)
		{
			PC_GPU_TEMP = (frame.data[0] << 24) | (frame.data[1] << 16) | (frame.data[2] << 8) | frame.data[3];
			PC_BOARD_TEMP = (frame.data[4] << 24) | (frame.data[5] << 16) | (frame.data[6] << 8) | frame.data[7];

			printf("%s 0x%08X %.3f\r\n", "PC_GPU_TEMP", PC_GPU_TEMP, HexToIEEE754_simple(PC_GPU_TEMP));
			printf("%s 0x%08X %.3f\r\n", "PC_BOARD_TEMP", PC_BOARD_TEMP, HexToIEEE754_simple(PC_BOARD_TEMP));
		}
		else if(frame.can_id == 0x1B0A0009)
		{
			PC_SOC0_TEMP = (frame.data[0] << 24) | (frame.data[1] << 16) | (frame.data[2] << 8) | frame.data[3];
			PC_SOC1_TEMP = (frame.data[4] << 24) | (frame.data[5] << 16) | (frame.data[6] << 8) | frame.data[7];

			printf("%s 0x%08X %.3f\r\n", "PC_SOC0_TEMP", PC_SOC0_TEMP, HexToIEEE754_simple(PC_SOC0_TEMP));
			printf("%s 0x%08X %.3f\r\n", "PC_SOC1_TEMP", PC_SOC1_TEMP, HexToIEEE754_simple(PC_SOC1_TEMP));
		}
		else if(frame.can_id == 0x1B0A000A)
		{
			PC_SOC2_TEMP = (frame.data[0] << 24) | (frame.data[1] << 16) | (frame.data[2] << 8) | frame.data[3];
			PC_TDIODE_TEMP = (frame.data[4] << 24) | (frame.data[5] << 16) | (frame.data[6] << 8) | frame.data[7];

			printf("%s 0x%08X %.3f\r\n", "PC_SOC2_TEMP", PC_SOC0_TEMP, HexToIEEE754_simple(PC_SOC2_TEMP));
			printf("%s 0x%08X %.3f\r\n", "PC_TDIODE_TEMP", PC_TDIODE_TEMP, HexToIEEE754_simple(PC_TDIODE_TEMP));
		}
		else if(frame.can_id == 0x1B0A000B)
		{
			PC_TJ_TEMP = (frame.data[0] << 24) | (frame.data[1] << 16) | (frame.data[2] << 8) | frame.data[3];

			printf("%s 0x%08X %.3f\r\n", "PC_TJ_TEMP", PC_TJ_TEMP, HexToIEEE754_simple(PC_TJ_TEMP));
		}
		else
		{
			printf("%s 0x%03X [%d] ", ifr.ifr_name, frame.can_id, frame.can_dlc);
	    	
        	for (i = 0; i < frame.can_dlc; i++)
	    		printf("%02X ",frame.data[i]);
        	
	    	printf("\r\n");
		}		
	}

	

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
