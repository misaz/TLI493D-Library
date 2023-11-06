#include "cybsp.h"
#include "cy_utils.h"
#include "TLI493D.h"
#include "UART.h"
#include <stdio.h>

int is_interrupt_triggered = 0;

void eru_0_ogu_2_INTERRUPT_HANDLER() {
	is_interrupt_triggered = 1;
}

int main(void) {
	cy_rslt_t cStatus;
	TLI493D_Status tStatus;

	TLI493D_Device tli;
	TLI493D_Configuration config;

	cStatus = cybsp_init();
	CY_ASSERT(cStatus == CY_RSLT_SUCCESS);

	XMC_Delay(1000);

	TLI493D_GetDefaultConfiguration(&config);
	config.updateRate = TLI493D_UpdateRate_3Hz;

	tStatus = TLI493D_Init(&tli, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	CY_ASSERT(tStatus == TLI493D_Status_Ok);

	NVIC_SetPriority(eru_0_ogu_2_IRQN, 1);
	NVIC_ClearPendingIRQ(eru_0_ogu_2_IRQN);
	NVIC_EnableIRQ(eru_0_ogu_2_IRQN);

	UART_PrintString("Init OK\r\n");

	while (1) {
		if (is_interrupt_triggered) {
			is_interrupt_triggered = 0;

			float proximity;
			tStatus = TLI493D_GetProximity(&tli, &proximity);
			CY_ASSERT(tStatus == TLI493D_Status_Ok);

			int proximity1000 = (int)(proximity * 1000);
			char sign = ' ';
			if (proximity1000 < 0) {
				sign = '-';
				proximity1000 = -proximity1000;
			}


			char buff[64];
			snprintf(buff, sizeof(buff), "Proximity=%c%d.%03d\r\n", sign, proximity1000 / 1000, proximity1000 % 1000);
			UART_PrintString(buff);
		} else {
			__WFI();
		}
	}
}
