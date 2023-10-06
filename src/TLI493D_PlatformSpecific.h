#ifndef TLI493D_PLATFORMSPECIFIC_H_
#define TLI493D_PLATFORMSPECIFIC_H_

#include "TLI493D.h"

#include "cybsp.h"

#include <stdint.h>
#include <stddef.h>

#define TLI493D_I2C_INTERFACE		usic_0_ch_0_HW

#define TLI493D_SDA_PORT			XMC_GPIO_PORT2
#define TLI493D_SDA_PIN_NO			1

#define TLI493D_SCL_PORT			XMC_GPIO_PORT2
#define TLI493D_SCL_PIN_NO			0

TLI493D_Status TLI493D_PlatformSpecific_Init();
TLI493D_Status TLI493D_PlatformSpecific_Deinit();
TLI493D_Status TLI493D_PlatformSpecific_Write(TLI493D_Device* dev, uint8_t* buffer, size_t bufferSize);
TLI493D_Status TLI493D_PlatformSpecific_ReadOneByteMode(TLI493D_Device* dev, uint8_t* buffer, size_t bufferSize);
TLI493D_Status TLI493D_PlatformSpecific_IsTwoByteReadSupported();
TLI493D_Status TLI493D_PlatformSpecific_ReadTwoByteMode(TLI493D_Device* dev, uint8_t writeByte, uint8_t* buffer, size_t bufferSize);

#endif
