#include "TLI493D.h"
#include "TLI493D_PlatformSpecific.h"

#include <stdint.h>
#include <string.h>
#include <math.h>

static int initializedDevices = 0;

static TLI493D_Status TLI493D_ReadRegs(TLI493D_Device* dev, uint8_t regAddress, uint8_t* buffer, size_t bufferSize);
static TLI493D_Status TLI493D_GetReadTrigBits(TLI493D_TriggerMode trigger, uint8_t* trigBits);
static TLI493D_Status TLI493D_GetWriteTrigBits(TLI493D_TriggerMode trigger, uint8_t* trigBits);
static TLI493D_Status TLI493D_LoadRegsFallback(TLI493D_Device* dev, uint8_t regAddress, uint8_t* buffer, size_t bufferSize);

TLI493D_Status TLI493D_Init(TLI493D_Device* dev, uint8_t i2cAddress, TLI493D_Configuration* config) {
	TLI493D_Status status;

	if (initializedDevices == 0) {
		status = TLI493D_PlatformSpecific_Init();
		if (status) {
			return status;
		}
	}

	dev->i2cAddress = i2cAddress;

	// at begining we do not know if sensor is in one-byte read mode or two-byte read mode
	// we will try one-byte mode which is always implemented. If the actual mode is 
	// two-byte, then nobody will drive I2C bus at the time of registers transmition which 
	// will result to reading register at address 255 which do not exists in the chip and 
	// TLI493D NACK such request. If it already is in one-byte mode, then nothing will 
	// happen and we just read all register map (we already need it for saving reserved 
	// regs 12 and 13) which are "reserved" but require read before write access.
	// 
	// TLI493D_REG_VER is the last register, TLI493D_REG_BX is the first register
	dev->useOneByteProtocol = 1;
	uint8_t regmap[TLI493D_REG_VER + 1];
	status = TLI493D_ReadRegs(dev, TLI493D_REG_BX, regmap, sizeof(regmap));
	if (status == TLI493D_Status_I2CNack) {
		// now we know that device is configured in two-byte read mode. We need to read 
		// registers differently.
		if (TLI493D_PlatformSpecific_IsTwoByteReadSupported() == TLI493D_Status_Ok) {
			dev->useOneByteProtocol = 0;
			status = TLI493D_ReadRegs(dev, TLI493D_REG_BX, regmap, sizeof(regmap));
			if (status) {
				if (initializedDevices == 0) {
					TLI493D_PlatformSpecific_Deinit();
				}
				return status;
			}
		} else {
			status = TLI493D_LoadRegsFallback(dev, TLI493D_REG_BX, regmap, sizeof(regmap));
			if (status) {
				if (initializedDevices == 0) {
					TLI493D_PlatformSpecific_Deinit();
				}
				return status;
			}
		}
	} else if (status) {
		if (initializedDevices == 0) {
			TLI493D_PlatformSpecific_Deinit();
		}
		return status;
	}

	dev->reg12Reserved = regmap[TLI493D_REG_RESERVED];
	dev->regMod2Reserved = TLI493D_GET_FIELD(TLI493D_MOD2_RESERVED_FIELD, regmap[TLI493D_REG_MOD2]);

	status = TLI493D_SetConfiguration(dev, config);
	if (status) {
		if (initializedDevices == 0) {
			TLI493D_PlatformSpecific_Deinit();
		}
		return status;
	}

	initializedDevices++;

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_Deinit(TLI493D_Device* dev) {
	initializedDevices--;

	if (initializedDevices == 0) {
		TLI493D_PlatformSpecific_Deinit();
	}

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_Reset(TLI493D_Device* dev) {
	TLI493D_Status status;

	TLI493D_Device fakeDevice;
	fakeDevice.i2cAddress = 0x7F;

	status = TLI493D_PlatformSpecific_ReadOneByteMode(&fakeDevice, NULL, 0);
	if (status) {
		return status;
	}

	status = TLI493D_PlatformSpecific_ReadOneByteMode(&fakeDevice, NULL, 0);
	if (status) {
		return status;
	}

	fakeDevice.i2cAddress = 0x00;

	status = TLI493D_PlatformSpecific_Write(&fakeDevice, NULL, 0);
	if (status) {
		return status;
	}

	status = TLI493D_PlatformSpecific_Write(&fakeDevice, NULL, 0);
	if (status) {
		return status;
	}

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_GetDeviceVersion(TLI493D_Device* dev, uint8_t* type, uint8_t* hardwareVersion) {
	TLI493D_Status status;
	uint8_t ver;

	status = TLI493D_ReadRegs(dev, TLI493D_REG_VER, &ver, 1);
	if (status) {
		return status;
	}

	*type = TLI493D_GET_FIELD(TLI493D_VER_TYPE_FIELD, ver);
	*hardwareVersion = TLI493D_GET_FIELD(TLI493D_VER_HWV_FIELD, ver);

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_GetDefaultConfiguration(TLI493D_Configuration* config) {
	config->powerMode = TLI493D_PowerMode_LowPower;
	config->enabledChannels = TLI493D_EnabledChannels_BxByBzT;
	config->sensitivity = TLI493D_Sensitivity_X1;
	config->updateRate = TLI493D_UpdateRate_770Hz;
	config->enableWakeup = 0;

	config->wakeupThresholdXLow = 0xF800;
	config->wakeupThresholdXHigh = 0x07FF;
	config->wakeupThresholdYLow = 0xF800;
	config->wakeupThresholdYHigh = 0x07FF;
	config->wakeupThresholdZLow = 0xF800;
	config->wakeupThresholdZHigh = 0x07FF;

	if (TLI493D_PlatformSpecific_IsTwoByteReadSupported() == TLI493D_Status_Ok) {
		config->useOneByteProtocol = 0;
	} else {
		config->useOneByteProtocol = 1;
	}
	config->useColisionAvoidance = 0;
	config->enableInterrupt = 0;
	config->oneByteTriggerMode = TLI493D_TriggerMode_NoTrigger;
	config->temperatureCompensationMode = TLI493D_TemperatureCompensation_TC0;

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_GetConfiguration(TLI493D_Device* dev, TLI493D_Configuration* config) {
	TLI493D_Status status;

	// TLI493D_REG_VER is the last register
	uint8_t regs[TLI493D_REG_VER + 1];

	if (dev->useOneByteProtocol) {
		status = TLI493D_PlatformSpecific_ReadOneByteMode(dev, regs, sizeof(regs));
	} else {
		status = TLI493D_PlatformSpecific_ReadTwoByteMode(dev, 0, regs, sizeof(regs));
	}
	if (status) {
		return status;
	}

	// TODO Parity Check

	config->powerMode = TLI493D_GET_FIELD(TLI493D_MOD1_MODE_FIELD, regs[TLI493D_REG_MOD1]);

	uint8_t dt = TLI493D_GET_FIELD(TLI493D_CONFIG_DT_FIELD, regs[TLI493D_REG_CONFIG]);
	uint8_t am = TLI493D_GET_FIELD(TLI493D_CONFIG_AM_FIELD, regs[TLI493D_REG_CONFIG]);
	if (dt) {
		if (am) {
			config->enabledChannels = TLI493D_EnabledChannels_BxBy;
		} else {
			config->enabledChannels = TLI493D_EnabledChannels_BxByBz;
		}
	} else {
		if (am) {
			return TLI493D_Status_SensorMisconfigured;
		} else {
			config->enabledChannels = TLI493D_EnabledChannels_BxByBzT;
		}
	}

	uint8_t x2 = TLI493D_GET_FIELD(TLI493D_CONFIG_X2_FIELD, regs[TLI493D_REG_CONFIG]);
	uint8_t x4 = TLI493D_GET_FIELD(TLI493D_CONFIG2_X4_FIELD, regs[TLI493D_REG_CONFIG2]);
	if (x2) {
		if (x4) {
			config->sensitivity = TLI493D_Sensitivity_X4;
		} else {
			config->sensitivity = TLI493D_Sensitivity_X2;
		}
	} else {
		if (x4) {
			return TLI493D_Status_SensorMisconfigured;
		} else {
			config->sensitivity = TLI493D_Sensitivity_X1;
		}
	}

	config->updateRate = TLI493D_GET_FIELD(TLI493D_MOD2_PRD_FIELD, regs[TLI493D_REG_MOD2]);

	config->enableWakeup = TLI493D_GET_FIELD(TLI493D_WU_WU_FIELD, regs[TLI493D_REG_WU]);

	config->wakeupThresholdXLow = (((uint16_t)(regs[TLI493D_REG_XL])) << 3) | TLI493D_GET_FIELD(TLI493D_WU_XL_FIELD, regs[TLI493D_REG_WU]);
	config->wakeupThresholdXHigh = (((uint16_t)(regs[TLI493D_REG_XH])) << 3) | TLI493D_GET_FIELD(TLI493D_WU_XH_FIELD, regs[TLI493D_REG_WU]);
	config->wakeupThresholdYLow = (((uint16_t)(regs[TLI493D_REG_YL])) << 3) | TLI493D_GET_FIELD(TLI493D_YHL2_YL_FIELD, regs[TLI493D_REG_YHL2]);
	config->wakeupThresholdYHigh = (((uint16_t)(regs[TLI493D_REG_YH])) << 3) | TLI493D_GET_FIELD(TLI493D_YHL2_YH_FIELD, regs[TLI493D_REG_YHL2]);
	config->wakeupThresholdZLow = (((uint16_t)(regs[TLI493D_REG_ZL])) << 3) | TLI493D_GET_FIELD(TLI493D_ZHL2_ZL_FIELD, regs[TLI493D_REG_ZHL2]);
	config->wakeupThresholdZHigh = (((uint16_t)(regs[TLI493D_REG_ZH])) << 3) | TLI493D_GET_FIELD(TLI493D_ZHL2_ZH_FIELD, regs[TLI493D_REG_ZHL2]);

	config->useOneByteProtocol = TLI493D_GET_FIELD(TLI493D_MOD1_PR_FIELD, regs[TLI493D_REG_MOD1]);
	config->useColisionAvoidance = TLI493D_GET_FIELD(TLI493D_MOD1_CA_FIELD, regs[TLI493D_REG_MOD1]);
	config->enableInterrupt = TLI493D_GET_FIELD(TLI493D_MOD1_INT_FIELD, regs[TLI493D_REG_MOD1]);

	uint8_t trig = TLI493D_GET_FIELD(TLI493D_CONFIG_TRIG_FIELD, regs[TLI493D_REG_CONFIG]);
	if (trig == 0) {
		config->oneByteTriggerMode = TLI493D_TriggerMode_NoTrigger;
	} else if (trig == 1) {
		config->oneByteTriggerMode = TLI493D_TriggerMode_BeforeFirstMsb;
	} else {
		config->oneByteTriggerMode = TLI493D_TriggerMode_AfterRegister05;
	}

	config->temperatureCompensationMode = TLI493D_GET_FIELD(TLI493D_CONFIG_TL_MAG_FIELD, regs[TLI493D_REG_CONFIG]);

	return TLI493D_Status_Ok;

}

TLI493D_Status TLI493D_SetConfiguration(TLI493D_Device* dev, TLI493D_Configuration* config) {
	return TLI493D_SetConfigurationAndTrigger(dev, config, TLI493D_TriggerMode_NoTrigger);
}

TLI493D_Status TLI493D_SetConfigurationAndTrigger(TLI493D_Device* dev, TLI493D_Configuration* config, TLI493D_TriggerMode triggerMode) {
	TLI493D_Status status;

	uint8_t buff[15];

	uint8_t trigBits;
	status = TLI493D_GetWriteTrigBits(triggerMode, &trigBits);
	if (status) {
		return status;
	}

	// write register address + trig bits
	buff[0] = TLI493D_REG_XL | trigBits;

	// XL
	buff[1] = config->wakeupThresholdXLow >> 4;

	// XH
	buff[2] = config->wakeupThresholdXHigh >> 4;

	// YL
	buff[3] = config->wakeupThresholdYLow >> 4;

	// YH
	buff[4] = config->wakeupThresholdYHigh >> 4;

	// ZL
	buff[5] = config->wakeupThresholdZLow >> 4;

	// ZH
	buff[6] = config->wakeupThresholdZHigh >> 4;

	// WU
	buff[7] =
		TLI493D_SET_FIELD(TLI493D_WU_WU_FIELD, config->enableWakeup) |
		TLI493D_SET_FIELD(TLI493D_WU_XH_FIELD, ((config->wakeupThresholdXHigh & 0xF) >> 1)) |
		TLI493D_SET_FIELD(TLI493D_WU_XL_FIELD, ((config->wakeupThresholdXLow & 0xF) >> 1));

	// YHL2
	buff[8] =
		TLI493D_SET_FIELD(TLI493D_YHL2_YH_FIELD, ((config->wakeupThresholdYHigh & 0xF) >> 1)) | 
		TLI493D_SET_FIELD(TLI493D_YHL2_YL_FIELD, ((config->wakeupThresholdYLow & 0xF) >> 1));

	// ZHL2
	buff[9] =
		TLI493D_SET_FIELD(TLI493D_ZHL2_ZH_FIELD, ((config->wakeupThresholdZHigh & 0xF) >> 1)) | 
		TLI493D_SET_FIELD(TLI493D_ZHL2_ZL_FIELD, ((config->wakeupThresholdZLow & 0xF) >> 1));

	// CONFIG
	uint8_t dt;
	uint8_t am;
	if (config->enabledChannels == TLI493D_EnabledChannels_BxByBzT) {
		dt = 0;
		am = 0;
	} else if (config->enabledChannels == TLI493D_EnabledChannels_BxByBz) {
		dt = 1;
		am = 0;
	} else if (config->enabledChannels == TLI493D_EnabledChannels_BxBy) {
		dt = 1;
		am = 1;
	} else {
		return TLI493D_Status_BadArg;
	}
	uint8_t trig;
	if (config->oneByteTriggerMode == TLI493D_TriggerMode_NoTrigger) {
		trig = 0x00;
	} else if (config->oneByteTriggerMode == TLI493D_TriggerMode_BeforeFirstMsb) {
		trig = 0x01;
	} else if (config->oneByteTriggerMode == TLI493D_TriggerMode_AfterRegister05) {
		trig = 0x02;
	} else {
		return TLI493D_Status_BadArg;
	}
	uint8_t x2;
	uint8_t x4;
	if (config->sensitivity == TLI493D_Sensitivity_X1) {
		x2 = 0;
		x4 = 0;
	} else if (config->sensitivity == TLI493D_Sensitivity_X2) {
		x2 = 1;
		x4 = 0;
	} else if (config->sensitivity == TLI493D_Sensitivity_X4) {
		x2 = 1;
		x4 = 1;
	} else {
		return TLI493D_Status_BadArg;
	}
	buff[10] =
		TLI493D_SET_FIELD(TLI493D_CONFIG_DT_FIELD, dt) |
		TLI493D_SET_FIELD(TLI493D_CONFIG_AM_FIELD, am) |
		TLI493D_SET_FIELD(TLI493D_CONFIG_TRIG_FIELD, trig) |
		TLI493D_SET_FIELD(TLI493D_CONFIG_X2_FIELD, x2) |
		TLI493D_SET_FIELD(TLI493D_CONFIG_TL_MAG_FIELD, config->temperatureCompensationMode);

	// MOD1
	uint8_t iicAddr = 0;
	if (dev->i2cAddress == TLI493D_I2C_7BIT_ADDRESS_A0) {
		iicAddr = 0x00;
	} else if (dev->i2cAddress == TLI493D_I2C_7BIT_ADDRESS_A1) {
		iicAddr = 0x01;
	} else if (dev->i2cAddress == TLI493D_I2C_7BIT_ADDRESS_A2) {
		iicAddr = 0x02;
	} else if (dev->i2cAddress == TLI493D_I2C_7BIT_ADDRESS_A3) {
		iicAddr = 0x03;
	} else {
		return TLI493D_Status_BadArg;
	}
	buff[11] =
		TLI493D_SET_FIELD(TLI493D_MOD1_IICADR_FIELD, iicAddr) |
		TLI493D_SET_FIELD(TLI493D_MOD1_PR_FIELD, config->useOneByteProtocol) |
		TLI493D_SET_FIELD(TLI493D_MOD1_CA_FIELD, config->useColisionAvoidance) |
		TLI493D_SET_FIELD(TLI493D_MOD1_INT_FIELD, config->enableInterrupt) |
		TLI493D_SET_FIELD(TLI493D_MOD1_MODE_FIELD, config->powerMode);

	// RESERVED
	buff[12] = dev->reg12Reserved;

	// MOD2
	buff[13] =
		TLI493D_SET_FIELD(TLI493D_MOD2_PRD_FIELD, config->updateRate) |
		TLI493D_SET_FIELD(TLI493D_MOD2_RESERVED_FIELD, dev->regMod2Reserved);

	// CONFIG2
	buff[14] = TLI493D_SET_FIELD(TLI493D_CONFIG2_X4_FIELD, x4);

	uint8_t cp =
		1 ^
		TLI493D_BYTE_PARITY(buff[1]) ^
		TLI493D_BYTE_PARITY(buff[2]) ^
		TLI493D_BYTE_PARITY(buff[3]) ^
		TLI493D_BYTE_PARITY(buff[4]) ^
		TLI493D_BYTE_PARITY(buff[5]) ^
		TLI493D_BYTE_PARITY(buff[6]) ^
		TLI493D_BYTE_PARITY(buff[7]) ^
		TLI493D_BYTE_PARITY(buff[8]) ^
		TLI493D_BYTE_PARITY(buff[9]) ^
		TLI493D_BYTE_PARITY(buff[10]);
	buff[10] |= TLI493D_SET_FIELD(TLI493D_CONFIG_CP_FIELD, cp);

	uint8_t fp =
		1 ^
		TLI493D_BYTE_PARITY(buff[11]) ^
		TLI493D_BYTE_PARITY(TLI493D_SET_FIELD(TLI493D_MOD2_PRD_FIELD, config->updateRate));
	buff[11] |= TLI493D_SET_FIELD(TLI493D_MOD1_FP_FIELD, fp);

	status = TLI493D_PlatformSpecific_Write(dev, buff, sizeof(buff));
	if (status) {
		return status;
	}

	dev->useOneByteProtocol = config->useOneByteProtocol;
	dev->actualSensitivity = config->sensitivity;

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_GetData(TLI493D_Device* dev, float* x, float* y, float* z, float* temperature) {
	return TLI493D_GetDataAndTrigger(dev, x, y, z, temperature, TLI493D_TriggerMode_NoTrigger);
}

TLI493D_Status TLI493D_GetDataRaw(TLI493D_Device* dev, int16_t* x, int16_t* y, int16_t* z, int16_t* temperature) {
	return TLI493D_GetDataRawAndTrigger(dev, x, y, z, temperature, TLI493D_TriggerMode_NoTrigger);
}

TLI493D_Status TLI493D_GetDataAndTrigger(TLI493D_Device* dev, float* x, float* y, float* z, float* temperature, TLI493D_TriggerMode triggerMode) {
	TLI493D_Status status;
	int16_t xi;
	int16_t yi;
	int16_t zi;
	int16_t ti;

	status = TLI493D_GetDataRawAndTrigger(dev, &xi, &yi, &zi, &ti, triggerMode);
	if (status) {
		return status;
	}

	if (dev->actualSensitivity == TLI493D_Sensitivity_X1) {
		*x = (float)xi / 7.7;
		*y = (float)yi / 7.7;
		if (z) {
			*z = (float)zi / 7.7;
		}
	} else if (dev->actualSensitivity == TLI493D_Sensitivity_X2) {
		*x = (float)xi / 15.4;
		*y = (float)yi / 15.4;
		if (z) {
			*z = (float)zi / 15.4;
		}
	} else if (dev->actualSensitivity == TLI493D_Sensitivity_X4) {
		*x = (float)xi / 30.8;
		*y = (float)yi / 30.8;
		if (z) {
			*z = (float)zi / 30.8;
		}
	} else {
		return TLI493D_Status_InvalidOperation;
	}
	if (temperature) {
		*temperature = (xi - 1180) * 0.24 + 25;
	}

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_GetDataRawAndTrigger(TLI493D_Device* dev, int16_t* x, int16_t* y, int16_t* z, int16_t* temperature, TLI493D_TriggerMode triggerMode) {
	TLI493D_Status status;
	uint8_t regs[7];

	if (dev->useOneByteProtocol) {
		if (triggerMode != TLI493D_TriggerMode_NoTrigger) {
			return TLI493D_Status_InvalidOperation;
		}
		status = TLI493D_PlatformSpecific_ReadOneByteMode(dev, regs, sizeof(regs));
	} else {
		uint8_t trigBits;
		status = TLI493D_GetReadTrigBits(triggerMode, &trigBits);
		if (status) {
			return status;
		}
		status = TLI493D_PlatformSpecific_ReadTwoByteMode(dev, 0x00 | trigBits, regs, sizeof(regs));
	}
	if (status) {
		return status;
	}

	// TODO parity check

	uint16_t ux = (((uint16_t)(regs[TLI493D_REG_BX])) << 4) | TLI493D_GET_FIELD(TLI493D_BX2_BX_FIELD, regs[TLI493D_REG_BX2]);
	uint16_t uy = (((uint16_t)(regs[TLI493D_REG_BY])) << 4) | TLI493D_GET_FIELD(TLI493D_BX2_BY_FIELD, regs[TLI493D_REG_BX2]);

	// sign extension + uint to int conversion
	*x = ((int16_t)(ux << 4) >> 4);
	*y = ((int16_t)(uy << 4) >> 4);
	if (z) {
		uint16_t uz = (((uint16_t)(regs[TLI493D_REG_BZ])) << 4) | TLI493D_GET_FIELD(TLI493D_TEMP2_BZ_FIELD, regs[TLI493D_REG_TEMP2]);
		*z = ((int16_t)(uz << 4) >> 4);
	}
	if (temperature) {
		uint16_t ut = (((uint16_t)(regs[TLI493D_REG_TEMP])) << 4) | ((TLI493D_GET_FIELD(TLI493D_TEMP2_TEMP_FIELD, regs[TLI493D_REG_TEMP2])) << 2);
		*temperature = ((int16_t)(ut << 4) >> 4);
	}

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_GetAngle(TLI493D_Device* dev, float* angle) {
	return TLI493D_GetAngleAndTrigger(dev, angle, TLI493D_TriggerMode_NoTrigger);
}

TLI493D_Status TLI493D_GetAngleAndTrigger(TLI493D_Device* dev, float* angle, TLI493D_TriggerMode triggerMode) {
	TLI493D_Status status;
	float x, y, z, temp;

	status = TLI493D_GetDataAndTrigger(dev, &x, &y, &z, &temp, triggerMode);
	if (status) {
		return status;
	}

	*angle = atan2f(y, x);

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_GetProximity(TLI493D_Device* dev, float* proximity) {
	return TLI493D_GetProximityAndTrigger(dev, proximity, TLI493D_TriggerMode_NoTrigger);
}

TLI493D_Status TLI493D_GetProximityAndTrigger(TLI493D_Device* dev, float* proximity, TLI493D_TriggerMode triggerMode) {
	TLI493D_Status status;
	float x, y, z, temp;

	status = TLI493D_GetDataAndTrigger(dev, &x, &y, &z, &temp, triggerMode);
	if (status) {
		return status;
	}

	*proximity = sqrtf(x * x + y * y + z * z);

	return TLI493D_Status_Ok;
}

TLI493D_Status TLI493D_TriggerConversion(TLI493D_Device* dev) {
	uint8_t trigBits = 0x40;
	return TLI493D_PlatformSpecific_Write(dev, &trigBits, 1);
}

static TLI493D_Status TLI493D_ReadRegs(TLI493D_Device* dev, uint8_t regAddress, uint8_t* buffer, size_t bufferSize) {
	TLI493D_Status status;

	if (regAddress + bufferSize > TLI493D_REG_VER + 1) {
		return TLI493D_Status_BadArg;
	}

	if (dev->useOneByteProtocol) {
		uint8_t scratchpad[TLI493D_REG_VER + 1];
		status = TLI493D_PlatformSpecific_ReadOneByteMode(dev, scratchpad, regAddress + bufferSize);
		if (status) {
			return status;
		}

		for (size_t i = 0; i < bufferSize; i++) {
			buffer[i] = scratchpad[regAddress + i];
		}

		return TLI493D_Status_Ok;
	} else {
		return TLI493D_PlatformSpecific_ReadTwoByteMode(dev, regAddress, buffer, bufferSize);
	}
}

static TLI493D_Status TLI493D_GetWriteTrigBits(TLI493D_TriggerMode triggerMode, uint8_t* trigBits) {
	if (triggerMode == TLI493D_TriggerMode_NoTrigger) {
		*trigBits = 0x00;
	} else if (triggerMode == TLI493D_TriggerMode_AfterWrite) {
		*trigBits = 0x20;
	} else {
		return TLI493D_Status_BadArg;
	}
	return TLI493D_Status_Ok;
}

static TLI493D_Status TLI493D_GetReadTrigBits(TLI493D_TriggerMode triggerMode, uint8_t* trigBits) {
	if (triggerMode == TLI493D_TriggerMode_NoTrigger) {
		*trigBits = 0x00;
	} else if (triggerMode == TLI493D_TriggerMode_BeforeFirstMsb) {
		*trigBits = 0x40;
	} else if (triggerMode == TLI493D_TriggerMode_AfterRegister05) {
		*trigBits = 0x80;
	} else {
		return TLI493D_Status_BadArg;
	}
	return TLI493D_Status_Ok;
}

static TLI493D_Status TLI493D_LoadRegsFallback(TLI493D_Device* dev, uint8_t regAddress, uint8_t* buffer, size_t bufferSize) {
	TLI493D_Status status;

	uint8_t mod1 = 0x00;
	if (dev->i2cAddress == TLI493D_I2C_7BIT_ADDRESS_A0) {
		mod1 = 0x10;
	} else if (dev->i2cAddress == TLI493D_I2C_7BIT_ADDRESS_A1) {
		mod1 = 0xB0;
	} else if (dev->i2cAddress == TLI493D_I2C_7BIT_ADDRESS_A2) {
		mod1 = 0xD0;
	} else if (dev->i2cAddress == TLI493D_I2C_7BIT_ADDRESS_A3) {
		mod1 = 0x70;
	} else {
		return TLI493D_Status_BadArg;
	}
	uint8_t tempConfigTransaction[2] = { TLI493D_REG_CONFIG, mod1 };

	status = TLI493D_PlatformSpecific_Write(dev, tempConfigTransaction, sizeof(tempConfigTransaction));
	if (status) {
		if (initializedDevices == 0) {
			TLI493D_PlatformSpecific_Deinit();
		}
		return status;
	}

	dev->useOneByteProtocol = 1;

	status = TLI493D_ReadRegs(dev, regAddress, buffer, bufferSize);
	if (status) {
		return status;
	}

	return TLI493D_Status_Ok;
}
