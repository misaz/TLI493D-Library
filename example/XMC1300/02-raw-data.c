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
		int16_t x, y, z, t;
		tStatus = TLI493D_GetDataRaw(&tli, &x, &y, &z, &t);
		CY_ASSERT(tStatus == TLI493D_Status_Ok);

		char buff[64];
		snprintf(buff, sizeof(buff), "X=%d\t Y=%d\t Z=%d\t T=%d\r\n", x, y, z, t);
		UART_PrintString(buff);
	}
}
