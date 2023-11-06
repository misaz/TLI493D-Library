#include "TLI493D.h"
#include "TLI493D_PlatformSpecific.h"

TLI493D_Status TLI493D_PlatformSpecific_Init() {
	// this function must initialize I2C bus driver. It should not generate any 
	// transaction to the TLI493D device. It is recommended to generate 9 clock  
	// cycles when SDA line is asserted as low and then generate stop sequence.

	return TLI493D_Status_NotImplemented;
}

TLI493D_Status TLI493D_PlatformSpecific_Deinit() {
	// this function must deinitialize I2C bus driver. It should not generate any 
	// transaction to the TLI493D device.

	return TLI493D_Status_NotImplemented;
}

TLI493D_Status TLI493D_PlatformSpecific_Write(TLI493D_Device* dev, uint8_t* buffer, size_t bufferSize) {
	// this function must generate following on the I2C bus:
	//
	// 1) Generate STRAT condition.
	// 2) Transmit dev->i2cAddress and write direction. If you are using platform 
	//    (library) which requires shifted 7bit address use 
	//    TLI493D_I2C_WRITE_ADDRESS(dev->i2cAddress)
	// 3) Check that device ACKed transaction. Otherwise generate STOP sequence and 
	//    return TLI493D_Status_I2CNack.
	// 4) transmit buffer of size bufferSize. After transmiting every byte check that 
	//    device ACKed transaction. Otherwise generate STOP sequence and return
	//    TLI493D_Status_I2CNack.
	// 5) generate STOP sequence and return TLI493D_Status_Ok.
	//
	// In all steps check for errors like arbitration lost error. Use return code 
	// TLI493D_Status_I2CNack for errors related to NACKing and TLI493D_Status_I2CError 
	// for other I2C errors. 
	//
	// Before returning any status code remember to generate STOP sequence.
	//
	// Prevent using loops which may end up in infinete loop in case of unpredictable 
	// behaviour of I2C peripheral. Try implement platform specific timeout detection
	// and return TLI493D_Status_I2CTimeout when timeout occur. In case of timeout 
	// also generate STOP sequence and turn platform specific driver to the state that 
	// it can be reused for further communication again. (In the other words any I2C 
	// error should not make library stuck in error-generating state forever).

	return TLI493D_Status_NotImplemented;
}

TLI493D_Status TLI493D_PlatformSpecific_IsTwoByteReadSupported() {
	// this function must state if two byte read mode is support.
	// It must return
	// a) TLI493D_Status_Ok if TLI493D_PlatformSpecific_ReadTwoByteMode is implemented
	// b) TLI493D_Status_NotSupported if it is not
	// both states are valid. In case of not supporting this method, library will not call
	// TLI493D_PlatformSpecific_ReadTwoByteMode.

	return TLI493D_Status_NotSupported;
}

TLI493D_Status TLI493D_PlatformSpecific_ReadTwoByteMode(TLI493D_Device* dev, uint8_t writeByte, uint8_t* buffer, size_t bufferSize) {
	// this function must generate following on the I2C bus:
	//
	// 1) Generate STRAT condition.
	// 2) Transmit dev->i2cAddress and read direction. If you are using platform 
	//    (library) which requires shifted 7bit address use 
	//    TLI493D_I2C_READ_ADDRESS(dev->i2cAddress)
	// 3) Check that device ACKed transaction. Otherwise generate STOP sequence and 
	//    return TLI493D_Status_I2CNack.
	// 4) transmit writeByte byte.
	// 5) check that device ACKed transaction. Otherwise generate STOP sequence and
	//    return MAX40080_Status_I2CNack.
	// 6) Read buffer of size bufferSize. After receiving every byte except the last byte, 
	//    ACK the transaction. Generate NACK instead of ACK after receiving last byte.
	// 7) Generate STOP sequence and return MAX40080_Status_Ok.
	//
	// In all steps check for errors like arbitration lost error. Use return code 
	// TLI493D_Status_I2CNack for errors related to NACKing and TLI493D_Status_I2CError 
	// for other I2C errors. 
	//
	// Before returning any status code remember to generate STOP sequence.
	//
	// Prevent using loops which may end up in infinete loop in case of unpredictable 
	// behaviour of I2C peripheral. Try implement platform specific timeout detection
	// and return TLI493D_Status_I2CTimeout when timeout occur. In case of timeout 
	// also generate STOP sequence and turn platform specific driver to the state that 
	// it can be reused for further communication again. (In the other words any I2C 
	// error should not make library stuck in error-generating state forever).
	return TLI493D_Status_NotSupported;
}

TLI493D_Status TLI493D_PlatformSpecific_ReadOneByteMode(TLI493D_Device* dev, uint8_t* buffer, size_t bufferSize) {
	// this function must generate following on the I2C bus:
	//
	// 1) Generate STRAT condition.
	// 2) Transmit dev->i2cAddress and read direction. If you are using platform 
	//    (library) which requires shifted 7bit address use 
	//    TLI493D_I2C_READ_ADDRESS(dev->i2cAddress)
	// 3) Check that device ACKed transaction. Otherwise generate STOP sequence and 
	//    return TLI493D_Status_I2CNack.
	// 4) Read buffer of size bufferSize. After receiving every byte except the last byte 
	//    ACK the transaction. Generate NACK instead of ACK after receiving last byte.
	// 5) Generate STOP sequence and return TLI493D_Status_Ok.
	//
	// In all steps check for errors like arbitration lost error. Use return code 
	// TLI493D_Status_I2CNack for errors related to NACKing and TLI493D_Status_I2CError 
	// for other I2C errors. 
	//
	// Before returning any status code remember to generate STOP sequence.
	//
	// Prevent using loops which may end up in inifinete loop in case of unpredictable 
	// behaviour of I2C peripheral. Try implement platform specific timeout detection
	// and return TLI493D_Status_I2CTimeout when timeout occur. In case of timeout 
	// also generate STOP sequence and turn platform specific driver to the state that 
	// it can be reused for further communication again. (In the other words any I2C 
	// error should not make library stuck in error-generating state forever).

	return TLI493D_Status_NotImplemented;
}
