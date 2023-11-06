#include "cybsp.h"
#include "cy_utils.h"
#include "TLI493D.h"
#include "UART.h"
#include <stdio.h>

int main(void) {
	cy_rslt_t cStatus;
	TLI493D_Status tStatus;

	TLI493D_Device tli;
	TLI493D_Configuration config;

	cStatus = cybsp_init();
	CY_ASSERT(cStatus == CY_RSLT_SUCCESS);

	XMC_Delay(1000);

	TLI493D_GetDefaultConfiguration(&config);

	tStatus = TLI493D_Init(&tli, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	CY_ASSERT(tStatus == TLI493D_Status_Ok);

	UART_PrintString("Init OK\r\n");

	while (1) {
		float angle;
		tStatus = TLI493D_GetAngle(&tli, &angle);
		CY_ASSERT(tStatus == TLI493D_Status_Ok);

		int angle1000 = (int)(angle * 1000);
		char sign = ' ';
		if (angle1000 < 0) {
			sign = '-';
			angle1000 = -angle1000;
		}


		char buff[64];
		snprintf(buff, sizeof(buff), "Angle=%c%d.%03d\r\n", sign, angle1000 / 1000, angle1000 % 1000);
		UART_PrintString(buff);
	}
}
