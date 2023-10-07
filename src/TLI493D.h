#ifndef TLI493D_H_
#define TLI493D_H_

#include <stdint.h>
#include <stddef.h>

#define TLI493D_I2C_7BIT_ADDRESS_A0			(0x6A >> 1)
#define TLI493D_I2C_7BIT_ADDRESS_A1			(0x44 >> 1)
#define TLI493D_I2C_7BIT_ADDRESS_A2			(0xF0 >> 1)
#define TLI493D_I2C_7BIT_ADDRESS_A3			(0x88 >> 1)
#define TLI493D_I2C_7BIT_ADDRESS_DEFAULT	TLI493D_I2C_7BIT_ADDRESS_A0

#define TLI493D_I2C_READ_ADDRESS(dev)		(((dev)->i2cAddress << 1) | 0x01)
#define TLI493D_I2C_WRITE_ADDRESS(dev)		(((dev)->i2cAddress << 1) | 0x00)

#define TLI493D_REG_BX						0x00
#define TLI493D_REG_BY						0x01
#define TLI493D_REG_BZ						0x02
#define TLI493D_REG_TEMP					0x03
#define TLI493D_REG_BX2						0x04
#define TLI493D_REG_TEMP2					0x05
#define TLI493D_REG_DIAG					0x06
#define TLI493D_REG_XL						0x07
#define TLI493D_REG_XH						0x08
#define TLI493D_REG_YL						0x09
#define TLI493D_REG_YH						0x0A
#define TLI493D_REG_ZL						0x0B
#define TLI493D_REG_ZH						0x0C
#define TLI493D_REG_WU						0x0D
#define TLI493D_REG_YHL2					0x0E
#define TLI493D_REG_ZHL2					0x0F
#define TLI493D_REG_CONFIG					0x10
#define TLI493D_REG_MOD1					0x11
#define TLI493D_REG_RESERVED				0x12
#define TLI493D_REG_MOD2					0x13
#define TLI493D_REG_CONFIG2					0x14
#define TLI493D_REG_VER						0x16

#define TLI493D_EXPAND_MACRO(content)		content
#define TLI493D_FIELD(NAME)					NAME##_MASK, NAME##_OFFSET
#define TLI493D_GET_FIELD_IMPL(FIELD_MASK,FIELD_OFFSET,value)	(((value) >> FIELD_OFFSET) & FIELD_MASK)
#define TLI493D_SET_FIELD_IMPL(FIELD_MASK,FIELD_OFFSET,value)	(((value) & FIELD_MASK) << FIELD_OFFSET)
#define TLI493D_GET_FIELD(...)				TLI493D_EXPAND_MACRO(TLI493D_GET_FIELD_IMPL(__VA_ARGS__))
#define TLI493D_SET_FIELD(...)				TLI493D_EXPAND_MACRO(TLI493D_SET_FIELD_IMPL(__VA_ARGS__))
#define TLI493D_MIN(a,b)					((a) < (b) ? (a): (b))
#define TLI493D_BYTE_PARITY(b)				(((b) & 0x01) ^ (((b) & 0x02) >> 1) ^ (((b) & 0x04) >> 2) ^ (((b) & 0x08) >> 3) ^ \
											(((b) & 0x10) >> 4) ^ (((b) & 0x20) >> 5) ^ (((b) & 0x40) >> 6) ^ (((b) & 0x80) >> 7))

#define TLI493D_BX2_BX_MASK					0xF
#define TLI493D_BX2_BX_OFFSET				4
#define TLI493D_BX2_BX_FIELD				TLI493D_FIELD(TLI493D_BX2_BX)

#define TLI493D_BX2_BY_MASK					0xF
#define TLI493D_BX2_BY_OFFSET				0
#define TLI493D_BX2_BY_FIELD				TLI493D_FIELD(TLI493D_BX2_BY)

#define TLI493D_TEMP2_TEMP_MASK				0x3
#define TLI493D_TEMP2_TEMP_OFFSET			6
#define TLI493D_TEMP2_TEMP_FIELD			TLI493D_FIELD(TLI493D_TEMP2_TEMP)

#define TLI493D_TEMP2_ID_MASK				0x3
#define TLI493D_TEMP2_ID_OFFSET				4
#define TLI493D_TEMP2_ID_FIELD				TLI493D_FIELD(TLI493D_TEMP2_ID)

#define TLI493D_TEMP2_BZ_MASK				0xF
#define TLI493D_TEMP2_BZ_OFFSET				0
#define TLI493D_TEMP2_BZ_FIELD				TLI493D_FIELD(TLI493D_TEMP2_BZ)

#define TLI493D_DIAG_P_MASK					0x1
#define TLI493D_DIAG_P_OFFSET				7
#define TLI493D_DIAG_P_FIELD				TLI493D_FIELD(TLI493D_DIAG_P)

#define TLI493D_DIAG_FF_MASK				0x1
#define TLI493D_DIAG_FF_OFFSET				6
#define TLI493D_DIAG_FF_FIELD				TLI493D_FIELD(TLI493D_DIAG_FF)

#define TLI493D_DIAG_CF_MASK				0x1
#define TLI493D_DIAG_CF_OFFSET				5
#define TLI493D_DIAG_CF_FIELD				TLI493D_FIELD(TLI493D_DIAG_CF)

#define TLI493D_DIAG_T_MASK					0x1
#define TLI493D_DIAG_T_OFFSET				4
#define TLI493D_DIAG_T_FIELD				TLI493D_FIELD(TLI493D_DIAG_T)

#define TLI493D_DIAG_PD3_MASK				0x1
#define TLI493D_DIAG_PD3_OFFSET				3
#define TLI493D_DIAG_PD3_FIELD				TLI493D_FIELD(TLI493D_DIAG_PD3)

#define TLI493D_DIAG_PD0_MASK				0x1
#define TLI493D_DIAG_PD0_OFFSET				2
#define TLI493D_DIAG_PD0_FIELD				TLI493D_FIELD(TLI493D_DIAG_PD0)

#define TLI493D_DIAG_FRM_MASK				0x3
#define TLI493D_DIAG_FRM_OFFSET				0
#define TLI493D_DIAG_FRM_FIELD				TLI493D_FIELD(TLI493D_DIAG_FRM)

#define TLI493D_WU_WA_MASK					0x1
#define TLI493D_WU_WA_OFFSET				7
#define TLI493D_WU_WA_FIELD					TLI493D_FIELD(TLI493D_WU_WA)

#define TLI493D_WU_WU_MASK					0x1
#define TLI493D_WU_WU_OFFSET				6
#define TLI493D_WU_WU_FIELD					TLI493D_FIELD(TLI493D_WU_WU)

#define TLI493D_WU_XH_MASK					0x7
#define TLI493D_WU_XH_OFFSET				3
#define TLI493D_WU_XH_FIELD					TLI493D_FIELD(TLI493D_WU_XH)

#define TLI493D_WU_XL_MASK					0x7
#define TLI493D_WU_XL_OFFSET				0
#define TLI493D_WU_XL_FIELD					TLI493D_FIELD(TLI493D_WU_XL)

#define TLI493D_YHL2_YH_MASK				0x7
#define TLI493D_YHL2_YH_OFFSET				3
#define TLI493D_YHL2_YH_FIELD				TLI493D_FIELD(TLI493D_YHL2_YH)

#define TLI493D_YHL2_YL_MASK				0x7
#define TLI493D_YHL2_YL_OFFSET				0
#define TLI493D_YHL2_YL_FIELD				TLI493D_FIELD(TLI493D_YHL2_YL)

#define TLI493D_ZHL2_ZH_MASK				0x7
#define TLI493D_ZHL2_ZH_OFFSET				3
#define TLI493D_ZHL2_ZH_FIELD				TLI493D_FIELD(TLI493D_ZHL2_ZH)

#define TLI493D_ZHL2_ZL_MASK				0x7
#define TLI493D_ZHL2_ZL_OFFSET				0
#define TLI493D_ZHL2_ZL_FIELD				TLI493D_FIELD(TLI493D_ZHL2_ZL)

#define TLI493D_CONFIG_DT_MASK				0x1
#define TLI493D_CONFIG_DT_OFFSET			7
#define TLI493D_CONFIG_DT_FIELD				TLI493D_FIELD(TLI493D_CONFIG_DT)

#define TLI493D_CONFIG_AM_MASK				0x1
#define TLI493D_CONFIG_AM_OFFSET			6
#define TLI493D_CONFIG_AM_FIELD				TLI493D_FIELD(TLI493D_CONFIG_AM)

#define TLI493D_CONFIG_TRIG_MASK			0x3
#define TLI493D_CONFIG_TRIG_OFFSET			4
#define TLI493D_CONFIG_TRIG_FIELD			TLI493D_FIELD(TLI493D_CONFIG_TRIG)

#define TLI493D_CONFIG_X2_MASK				0x1
#define TLI493D_CONFIG_X2_OFFSET			3
#define TLI493D_CONFIG_X2_FIELD				TLI493D_FIELD(TLI493D_CONFIG_X2)

#define TLI493D_CONFIG_TL_MAG_MASK			0x3
#define TLI493D_CONFIG_TL_MAG_OFFSET		1
#define TLI493D_CONFIG_TL_MAG_FIELD			TLI493D_FIELD(TLI493D_CONFIG_TL_MAG)

#define TLI493D_CONFIG_CP_MASK				0x1
#define TLI493D_CONFIG_CP_OFFSET			0
#define TLI493D_CONFIG_CP_FIELD				TLI493D_FIELD(TLI493D_CONFIG_CP)

#define TLI493D_MOD1_FP_MASK				0x1
#define TLI493D_MOD1_FP_OFFSET				7
#define TLI493D_MOD1_FP_FIELD				TLI493D_FIELD(TLI493D_MOD1_FP)

#define TLI493D_MOD1_IICADR_MASK			0x3
#define TLI493D_MOD1_IICADR_OFFSET			5
#define TLI493D_MOD1_IICADR_FIELD			TLI493D_FIELD(TLI493D_MOD1_IICADR)

#define TLI493D_MOD1_PR_MASK				0x1
#define TLI493D_MOD1_PR_OFFSET				4
#define TLI493D_MOD1_PR_FIELD				TLI493D_FIELD(TLI493D_MOD1_PR)

#define TLI493D_MOD1_CA_MASK				0x1
#define TLI493D_MOD1_CA_OFFSET				3
#define TLI493D_MOD1_CA_FIELD				TLI493D_FIELD(TLI493D_MOD1_CA)

#define TLI493D_MOD1_INT_MASK				0x1
#define TLI493D_MOD1_INT_OFFSET				2
#define TLI493D_MOD1_INT_FIELD				TLI493D_FIELD(TLI493D_MOD1_INT)

#define TLI493D_MOD1_MODE_MASK				0x3
#define TLI493D_MOD1_MODE_OFFSET			0
#define TLI493D_MOD1_MODE_FIELD				TLI493D_FIELD(TLI493D_MOD1_MODE)

#define TLI493D_MOD2_PRD_MASK				0x7
#define TLI493D_MOD2_PRD_OFFSET				5
#define TLI493D_MOD2_PRD_FIELD				TLI493D_FIELD(TLI493D_MOD2_PRD)

#define TLI493D_MOD2_RESERVED_MASK			0x1F
#define TLI493D_MOD2_RESERVED_OFFSET		0
#define TLI493D_MOD2_RESERVED_FIELD			TLI493D_FIELD(TLI493D_MOD2_RESERVED)

#define TLI493D_CONFIG2_X4_MASK				0x1
#define TLI493D_CONFIG2_X4_OFFSET			0
#define TLI493D_CONFIG2_X4_FIELD			TLI493D_FIELD(TLI493D_CONFIG2_X4)

#define TLI493D_VER_TYPE_MASK				0x3
#define TLI493D_VER_TYPE_OFFSET				4
#define TLI493D_VER_TYPE_FIELD				TLI493D_FIELD(TLI493D_VER_TYPE)

#define TLI493D_VER_HWV_MASK				0xF
#define TLI493D_VER_HWV_OFFSET				0
#define TLI493D_VER_HWV_FIELD				TLI493D_FIELD(TLI493D_VER_HWV)

typedef enum {
	TLI493D_Status_Ok = 0,
	TLI493D_Status_I2CNack = -10,
	TLI493D_Status_I2CError = -11,
	TLI493D_Status_I2CTimeout = -12,
	TLI493D_Status_ParityCheckFailed = -13,
	TLI493D_Status_SensorFailed = -14,
	TLI493D_Status_SensorMisconfigured = -15,
	TLI493D_Status_NotImplemented = -20,
	TLI493D_Status_BadArg = -21,
	TLI493D_Status_InvalidOperation = -22,
	TLI493D_Status_NotSupported = -24,
} TLI493D_Status;

typedef enum {
	TLI493D_EnabledChannels_BxByBzT = 0,
	TLI493D_EnabledChannels_BxByBz = 1,
	TLI493D_EnabledChannels_BxBy = 2,
} TLI493D_EnabledChannels;

typedef enum {
	TLI493D_TriggerMode_NoTrigger = 0,
	TLI493D_TriggerMode_AfterWrite = 1,
	TLI493D_TriggerMode_BeforeFirstMsb = 2,
	TLI493D_TriggerMode_AfterRegister05 = 3,
} TLI493D_TriggerMode;

typedef enum {
	TLI493D_Sensitivity_X1 = 0,
	TLI493D_Sensitivity_X2 = 1,
	TLI493D_Sensitivity_X4 = 2,
} TLI493D_Sensitivity;

typedef enum {
	TLI493D_TemperatureCompensation_TC0 = 0x00,
	TLI493D_TemperatureCompensation_TC1 = 0x01,
	TLI493D_TemperatureCompensation_TC2 = 0x02,
	TLI493D_TemperatureCompensation_TC3 = 0x03,
} TLI493D_TemperatureCompensation;

typedef enum {
	TLI493D_PowerMode_LowPower = 0x00,
	TLI493D_PowerMode_PowerDown = 0x01,
	TLI493D_PowerMode_FastMode = 0x03,
} TLI493D_PowerMode;

typedef enum {
	TLI493D_UpdateRate_770Hz = 0x00,
	TLI493D_UpdateRate_97Hz = 0x01,
	TLI493D_UpdateRate_24Hz = 0x02,
	TLI493D_UpdateRate_12Hz = 0x03,
	TLI493D_UpdateRate_6Hz = 0x04,
	TLI493D_UpdateRate_3Hz = 0x05,
	TLI493D_UpdateRate_0_4Hz = 0x06,
	TLI493D_UpdateRate_0_05Hz = 0x07,
} TLI493D_UpdateRate;

typedef struct  {
	TLI493D_PowerMode powerMode;
	TLI493D_EnabledChannels enabledChannels;
	TLI493D_Sensitivity sensitivity;
	TLI493D_UpdateRate updateRate;
	int enableWakeup;

	uint16_t wakeupThresholdXLow;
	uint16_t wakeupThresholdXHigh;
	uint16_t wakeupThresholdYLow;
	uint16_t wakeupThresholdYHigh;
	uint16_t wakeupThresholdZLow;
	uint16_t wakeupThresholdZHigh;

	int useOneByteProtocol;
	int nebaleColisionAvoidance;
	int enableInterrupt;
	TLI493D_TriggerMode oneByteTriggerMode;
	TLI493D_TemperatureCompensation temperatureCompensationMode;
} TLI493D_Configuration;

typedef struct  {
	uint8_t i2cAddress;
	int useOneByteProtocol;
	TLI493D_Sensitivity actualSensitivity;
	uint8_t reg12Reserved;
	uint8_t regMod2Reserved;
} TLI493D_Device;

TLI493D_Status TLI493D_Init(TLI493D_Device* dev, uint8_t i2cAddress, TLI493D_Configuration* config);
TLI493D_Status TLI493D_Deinit(TLI493D_Device* dev);
TLI493D_Status TLI493D_Reset(TLI493D_Device* dev);
TLI493D_Status TLI493D_GetDeviceVersion(TLI493D_Device* dev, uint8_t* type, uint8_t* hardwareVersion);

TLI493D_Status TLI493D_GetDefaultConfiguration(TLI493D_Configuration* config);
TLI493D_Status TLI493D_GetConfiguration(TLI493D_Device* dev, TLI493D_Configuration* config);
TLI493D_Status TLI493D_SetConfiguration(TLI493D_Device* dev, TLI493D_Configuration* config);
TLI493D_Status TLI493D_SetConfigurationAndTrigger(TLI493D_Device* dev, TLI493D_Configuration* config, TLI493D_TriggerMode triggerMode);

TLI493D_Status TLI493D_GetData(TLI493D_Device* dev, float* x, float* y, float* z, float* t);
TLI493D_Status TLI493D_GetDataRaw(TLI493D_Device* dev, int16_t* x, int16_t* y, int16_t* z, int16_t* t);
TLI493D_Status TLI493D_GetDataAndTrigger(TLI493D_Device* dev, float* x, float* y, float* z, float* t, TLI493D_TriggerMode triggerMode);
TLI493D_Status TLI493D_GetDataRawAndTrigger(TLI493D_Device* dev, int16_t* x, int16_t* y, int16_t* z, int16_t* t, TLI493D_TriggerMode triggerMode);

TLI493D_Status TLI493D_GetAngle(TLI493D_Device* dev, float* angle);
TLI493D_Status TLI493D_GetAngleAndTrigger(TLI493D_Device* dev, float* angle, TLI493D_TriggerMode triggerMode);

TLI493D_Status TLI493D_GetProximity(TLI493D_Device* dev, float* proximity);
TLI493D_Status TLI493D_GetProximityAndTrigger(TLI493D_Device* dev, float* proximity, TLI493D_TriggerMode triggerMode);

TLI493D_Status TLI493D_TriggerConversion(TLI493D_Device* dev);
#endif
