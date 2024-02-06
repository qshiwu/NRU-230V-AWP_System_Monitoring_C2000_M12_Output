
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

			Voltage_DCIN_SENSE = ((float)ADC_DCIN_SENSE / pow(2,11)) * 3.3;
			Voltage_P12VSUS_SENSE = ((float)ADC_P12VSUS_SENSE / pow(2,11)) * 3.3;
			Voltage_P12VSUS_ISMON = ((float)ADC_P12VSUS_ISMON / pow(2,11)) * 3.3;
			Voltage_P5VSUS_SENSE = ((float)ADC_P5VSUS_SENSE / pow(2,11)) * 3.3;

			CURRENT_P12VSUS_ISMON = (Voltage_P12VSUS_ISMON - 0.25) * 28.57;

			printf("%s 0x%04X %.2fV\r\n", "ADC_DCIN_SENSE", ADC_DCIN_SENSE, Voltage_DCIN_SENSE);
			printf("%s 0x%04X %.2fV\r\n", "ADC_P12VSUS_SENSE", ADC_P12VSUS_SENSE, Voltage_P12VSUS_SENSE);
			printf("%s 0x%04X %.2fV %.2fmA\r\n", "ADC_P12VSUS_ISMON", ADC_P12VSUS_ISMON, Voltage_P12VSUS_ISMON, CURRENT_P12VSUS_ISMON);
			printf("%s 0x%04X %.2fV\r\n", "ADC_P5VSUS_SENSE", ADC_P5VSUS_SENSE, Voltage_P5VSUS_SENSE);
			printf("\r\n");
		}
		else if(frame.can_id == 0x1B0A0002)
		{
			ADC_TMP_SENSE0 = (frame.data[0] << 8) | frame.data[1];
			ADC_TMP_SENSE1 = (frame.data[2] << 8) | frame.data[3];
			ADC_TMP_SENSE2 = (frame.data[4] << 8) | frame.data[5];
			ADC_TMP_SENSE3 = (frame.data[6] << 8) | frame.data[7];

			Voltage_TMP_SENSE0 = ((float)ADC_TMP_SENSE0 / pow(2,11)) * 3.3;
			Voltage_TMP_SENSE1 = ((float)ADC_TMP_SENSE1 / pow(2,11)) * 3.3;
			Voltage_TMP_SENSE2 = ((float)ADC_TMP_SENSE2 / pow(2,11)) * 3.3;
			Voltage_TMP_SENSE3 = ((float)ADC_TMP_SENSE3 / pow(2,11)) * 3.3;

			DEGREE_TMP_SENSE0 = interpolate(Voltage_TMP_SENSE0, Voltage2Temp, tableSize);
			DEGREE_TMP_SENSE1 = interpolate(Voltage_TMP_SENSE1, Voltage2Temp, tableSize);
			DEGREE_TMP_SENSE2 = interpolate(Voltage_TMP_SENSE2, Voltage2Temp, tableSize);
			DEGREE_TMP_SENSE3 = interpolate(Voltage_TMP_SENSE3, Voltage2Temp, tableSize);


			printf("%s 0x%04X %.2fV %.2fC\r\n", "ADC_TMP_SENSE0", ADC_TMP_SENSE0, Voltage_TMP_SENSE0, DEGREE_TMP_SENSE0);
			printf("%s 0x%04X %.2fV %.2fC\r\n", "ADC_TMP_SENSE1", ADC_TMP_SENSE1, Voltage_TMP_SENSE1, DEGREE_TMP_SENSE1);
			printf("%s 0x%04X %.2fV %.2fC\r\n", "ADC_TMP_SENSE2", ADC_TMP_SENSE2, Voltage_TMP_SENSE2, DEGREE_TMP_SENSE2);
			printf("%s 0x%04X %.2fV %.2fC\r\n", "ADC_TMP_SENSE3", ADC_TMP_SENSE3, Voltage_TMP_SENSE3, DEGREE_TMP_SENSE3);
		}
		else if(frame.can_id == 0x1B0A0003)
		{
			ADC_TMP_SENSE4 = (frame.data[0] << 8) | frame.data[1];

			Voltage_TMP_SENSE4 = ((float)ADC_TMP_SENSE4 / pow(2,11)) * 3.3;
			DEGREE_TMP_SENSE4 = interpolate(Voltage_TMP_SENSE4, Voltage2Temp, tableSize);

			printf("%s 0x%04X %.2fV %.2fC\r\n", "ADC_TMP_SENSE4", ADC_TMP_SENSE4, Voltage_TMP_SENSE4, DEGREE_TMP_SENSE4);
			printf("\r\n");
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
