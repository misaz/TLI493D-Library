/* SPDX-License-Identifier: MIT */

#include "TLI493D.h"
#include "TLI493D_PlatformSpecific.h"

#include "xmc_i2c.h"
#include "xmc_gpio.h"

#include "XMC1300.h"
#include "cybsp.h"
#include "cy_utils.h"

TLI493D_Status TLI493D_PlatformSpecific_Init() {
	// clock out bus for prevent deadlock when MCU reset in the middle
	// of ongoing I2C transaction

	XMC_GPIO_CONFIG_t scl;
	scl.mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN;
	scl.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD;
	scl.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH;
	XMC_GPIO_Init(TLI493D_SCL_PORT, TLI493D_SCL_PIN_NO, &scl);

	for (int i = 0; i < 10 * 2; i++) {
		XMC_GPIO_ToggleOutput(TLI493D_SCL_PORT, TLI493D_SCL_PIN_NO);
		XMC_Delay(1);
	}

	// reinitialize GPIO back
	cy_rslt_t cStatus = cybsp_init();
	CY_ASSERT(cStatus == CY_RSLT_SUCCESS);

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_PlatformSpecific_Deinit() {
	return TLI493D_Status_Ok;
}

static TLI493D_Status TLI493D_PlatformSpecific_WaitForAck() {
	uint32_t timeout = 250;

	uint32_t tragetFlags = XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED | XMC_I2C_CH_STATUS_FLAG_NACK_RECEIVED;

	while ((XMC_I2C_CH_GetStatusFlag(TLI493D_I2C_INTERFACE) & tragetFlags) == 0 && timeout > 0) {
		timeout--;
		XMC_Delay(1);
	}
	if (timeout == 0) {
		return TLI493D_Status_I2CTimeout;
	}
	uint32_t status = XMC_I2C_CH_GetStatusFlag(TLI493D_I2C_INTERFACE);
	if (status & XMC_I2C_CH_STATUS_FLAG_NACK_RECEIVED) {
		return TLI493D_Status_I2CNack;
	} else if ((status & XMC_I2C_CH_STATUS_FLAG_ACK_RECEIVED) == 0) {
		return TLI493D_Status_I2CError;
	}

	XMC_I2C_CH_ClearStatusFlag(TLI493D_I2C_INTERFACE, tragetFlags);

	return TLI493D_Status_Ok;
}

static TLI493D_Status TLI493D_PlatformSpecific_WaitForReception() {
	uint32_t timeout = 250;

	uint32_t tragetFlags = XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION | XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION;

	while ((XMC_I2C_CH_GetStatusFlag(TLI493D_I2C_INTERFACE) & tragetFlags) == 0 && timeout > 0) {
		timeout--;
		XMC_Delay(1);
	}
	if (timeout == 0) {
		return TLI493D_Status_I2CTimeout;
	}
	uint32_t status = XMC_I2C_CH_GetStatusFlag(TLI493D_I2C_INTERFACE);
	if (!(status & XMC_I2C_CH_STATUS_FLAG_RECEIVE_INDICATION) && !(status & XMC_I2C_CH_STATUS_FLAG_ALTERNATIVE_RECEIVE_INDICATION)) {
		return TLI493D_Status_I2CError;
	}

	XMC_I2C_CH_ClearStatusFlag(TLI493D_I2C_INTERFACE, tragetFlags);

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_PlatformSpecific_Write(TLI493D_Device* dev, uint8_t* buffer, size_t bufferSize) {
	TLI493D_Status status;

	XMC_I2C_CH_ClearStatusFlag(TLI493D_I2C_INTERFACE, 0x1FFFF);
	XMC_I2C_CH_MasterStart(TLI493D_I2C_INTERFACE, TLI493D_I2C_WRITE_ADDRESS(dev), XMC_I2C_CH_CMD_WRITE);
	status = TLI493D_PlatformSpecific_WaitForAck();
	if (status) {
		XMC_I2C_CH_MasterStop(TLI493D_I2C_INTERFACE);
		return status;
	}

	while (bufferSize--) {
		XMC_I2C_CH_MasterTransmit(TLI493D_I2C_INTERFACE, *buffer++);
		status = TLI493D_PlatformSpecific_WaitForAck();
		if (status) {
			XMC_I2C_CH_MasterStop(TLI493D_I2C_INTERFACE);
			return status;
		}
	}

	XMC_I2C_CH_MasterStop(TLI493D_I2C_INTERFACE);

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_PlatformSpecific_IsTwoByteReadSupported() {
	return TLI493D_Status_NotSupported;
}

TLI493D_Status TLI493D_PlatformSpecific_ReadTwoByteMode(TLI493D_Device* dev, uint8_t writeByte, uint8_t* buffer, size_t bufferSize) {
	return TLI493D_Status_NotSupported;
}

TLI493D_Status TLI493D_PlatformSpecific_ReadOneByteMode(TLI493D_Device* dev, uint8_t* buffer, size_t bufferSize) {
	TLI493D_Status status;

	XMC_I2C_CH_ClearStatusFlag(TLI493D_I2C_INTERFACE, 0x1FFFF);
	XMC_I2C_CH_MasterStart(TLI493D_I2C_INTERFACE, TLI493D_I2C_READ_ADDRESS(dev), XMC_I2C_CH_CMD_WRITE);
	status = TLI493D_PlatformSpecific_WaitForAck();
	if (status) {
		XMC_I2C_CH_MasterStop(TLI493D_I2C_INTERFACE);
		return status;
	}

	while (bufferSize--) {
		if (bufferSize == 0) {
			XMC_I2C_CH_MasterReceiveNack(TLI493D_I2C_INTERFACE);
		} else {
			XMC_I2C_CH_MasterReceiveAck(TLI493D_I2C_INTERFACE);
		}
		status = TLI493D_PlatformSpecific_WaitForReception();
		if (status) {
			XMC_I2C_CH_MasterStop(TLI493D_I2C_INTERFACE);
			return status;
		}
		*buffer++ = XMC_I2C_CH_GetReceivedData(TLI493D_I2C_INTERFACE);
	}

	XMC_I2C_CH_MasterStop(TLI493D_I2C_INTERFACE);

	return TLI493D_Status_Ok;
}
