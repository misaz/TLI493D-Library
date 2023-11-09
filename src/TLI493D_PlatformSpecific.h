/* SPDX-License-Identifier: MIT */

#ifndef TLI493D_PLATFORMSPECIFIC_H_
#define TLI493D_PLATFORMSPECIFIC_H_

#include "TLI493D.h"

#include <stdint.h>
#include <stddef.h>

TLI493D_Status TLI493D_PlatformSpecific_Init();
TLI493D_Status TLI493D_PlatformSpecific_Deinit();
TLI493D_Status TLI493D_PlatformSpecific_Write(TLI493D_Device* dev, uint8_t* buffer, size_t bufferSize);
TLI493D_Status TLI493D_PlatformSpecific_ReadOneByteMode(TLI493D_Device* dev, uint8_t* buffer, size_t bufferSize);
TLI493D_Status TLI493D_PlatformSpecific_IsTwoByteReadSupported();
TLI493D_Status TLI493D_PlatformSpecific_ReadTwoByteMode(TLI493D_Device* dev, uint8_t writeByte, uint8_t* buffer, size_t bufferSize);

#endif
