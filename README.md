# TLI493D Magnetic Sensor C Library

This project contains library for controlling TLI493D 3D Magneti Sensor from Infineon. Library is written in C and is designed as multiplatform.

Library was originally developed as part of [Hackster Connect things with code! with Infineon Competition](https://www.hackster.io/contests/connectthingswithcode).

- Support for multiple devices on bus

## Features
- Written in C. Can be used in both C and C++ projects.
- Minimal RAM usage. No dynamic memory allocations.
- Library core depends on stdint.h (for uint8_t, uint16_t, int16_t and uint32_t types), stddef- (for size_t type) and math.h if geniometric computations are needed.
- Platform specific ports depends on vendor SDK (see section Supported platfroms and porting below).
- Support for multiple deveices on I2C Bus
- Support for getting data from sensor and convert them to mT for all 3 directions
- Support for getting data from sensor in RAW format for all 3 directions
- Support for computing angle and proximity based on measured data
- Support for setting thresholds for genreting interrupts
- Support for using both one byte and two byte read modes
- Support for disabling unused channels
- Support for triggering conversion when executing I2C transactions
- Support for sensing X1, X2 and X4 sensitivity modes
- Support for using low-power, fast and power-down modes
- Support for setting refresh rate from 0.05 to 770 Hz
- Support for 4 temperature compenstation modes

## Supporter Platforms

Currently Library is ported to following platforms. Library can be easily ported to any other platform by implementing 4 (or 6 if support for two byte read mode is needed) simple functions in `TLI493D_PlatformSpecific.c` file.

Currently is library ported to following platforms:

- Infineon XMC 1300 Microcontroller projects based on Modus Toolbox toolchain


## Getting started
1. Go to [Release page](https://github.com/misaz/TLI493D-Library/releases) and download ZIP file targetting your Platform or ZIP targeting Generic platform if your platform is not supported.
1. Unzip downloaded file and copy all files to folder of your project or IDE
1. Add `#include "TLI493D.h"` at the begining of your source code file.
1. Allocate `TLI493D_Device` structure. It must be live for whole time when device is used.
1. Allocate `TLI493D_Configuration` structure and fill its fields or use `TLI493D_GetDefaultConfiguration` for loading defaults to this structure.
1. Call `TLI493D_Init()` function at the begining of your program. Pass pointer to previously allocated device structure. For specifing use one `TLI493D_I2C_7BIT_ADDRESS_Ax` constant when x at the end is the last number from part number, for example `TLI493D_I2C_7BIT_ADDRESS_A0`. At last pass pointer to previously allocated and filled configuration structure.
1. Use functions starting with `TLI493D_` as needed.

## Functions
Library contains following functions (you can find this listing in `TLI493D.h` file):

```
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
```

## Examples

Here are few generic examples how to use the library. They target no specific MCU and you will need to provide MCU specific commands and initialization yourself according to comments.

### Simple usage

Following example configure sensor in default mode and in infinite loop it retrieves (and prints) measured value from sensor. If you use sensor with different address replace `TLI493D_I2C_7BIT_ADDRESS_A0` in the example by constant coresponding to your sensor.

| Product Name | I2C 7-bit Address  | 8-bit Write Addr  | 8-bit Read Addr | TLI493D_Init Address Constant | 
|---|---|---|---|---|
| TLI493D-W2BW A0 | 0x35 | 0x6A | 0x6B | `TLI493D_I2C_7BIT_ADDRESS_A0` |
| TLI493D-W2BW A1 | 0x22 | 0x44 | 0x45 | `TLI493D_I2C_7BIT_ADDRESS_A1` |
| TLI493D-W2BW A2 | 0x78 | 0xF0 | 0xF1 | `TLI493D_I2C_7BIT_ADDRESS_A2` |
| TLI493D-W2BW A3 | 0x44 | 0x88 | 0x89 | `TLI493D_I2C_7BIT_ADDRESS_A3` |

```
#include "TLI493D.h"
#include <stdio.h>

int main(void) {
	TLI493D_Status tStatus;
	TLI493D_Device tli;
	TLI493D_Configuration config;

	TLI493D_GetDefaultConfiguration(&config);
	tStatus = TLI493D_Init(&tli, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	// TODO: handle error code in tStatus

	while (1) {
		float x, y, z, t;
		tStatus = TLI493D_GetData(&tli, &x, &y, &z, &t);
		// TODO: handle error code in tStatus

		printf("X=%.3f mT\t Y=%.3f mT\t Z=%.3f mT\t T=%.3f mT\r\n", x, y, z, t);
	}
}
```

### RAW (Integer) Data

In previous example values from sensor was converted to mT unit by library. If you want to receive original unprocessed data (and, for example, process them yourself) you can use functions with Raw in name. In case of simple example replace `TLI493D_GetData` by `TLI493D_GetDataRaw` function call and remember to adjust data types from `float` to `int16_t`.

```
#include "TLI493D.h"
#include <stdio.h>

int main(void) {
	TLI493D_Status tStatus;
	TLI493D_Device tli;
	TLI493D_Configuration config;

	TLI493D_GetDefaultConfiguration(&config);
	tStatus = TLI493D_Init(&tli, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	// TODO: handle error code in tStatus

	while (1) {
		int16_t x, y, z, t;
		tStatus = TLI493D_GetDataRaw(&tli, &x, &y, &z, &t);
		// TODO: handle error code in tStatus

		printf("X=%d\t Y=%d\t Z=%d\t T=%d\r\n", x, y, z, t);
	}
}
```


### Multiple sensors
Basicaly exmaples is almost same as in case in previous case. You need to allocate `TLI493D_Device` for every device and call `TLI493D_Init` (with different `TLI493D_I2C_7BIT_ADDRESS_Ax` I2C address constant) for every device. Config structure you can reuse. Library do not use it after initialization call, so you can dealocate it or change it and use for other sensor. You can of course use both RAW and mT values independently.

```
#include "TLI493D.h"
#include <stdio.h>

int main(void) {
	TLI493D_Status tStatus;
	TLI493D_Device tli1, tli2;
	TLI493D_Configuration config;

	TLI493D_GetDefaultConfiguration(&config);
	tStatus = TLI493D_Init(&tli1, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	tStatus = TLI493D_Init(&tli2, TLI493D_I2C_7BIT_ADDRESS_A1, &config);
	// TODO: handle error code in tStatus

	while (1) {
		int16_t x1, y1, z1, t1, x2, y2, z2, t2;

		tStatus = TLI493D_GetDataRaw(&tli1, &x1, &y1, &z1, &t1);
		// TODO: handle error code in tStatus

		tStatus = TLI493D_GetDataRaw(&tli2, &x2, &y2, &z2, &t2);
		// TODO: handle error code in tStatus

		printf("X1=%d\t Y1=%d\t Z1=%d\t T1=%d\tX2=%d\t Y2=%d\t Z2=%d\t T2=%d\r\n", x1, y1, z1, t1, x2, y2, z2, t2);
	}
}
```

### Angle

Following example prints angle of nearby magnetic field (in radians). You can use it for detecting position of magnet.

```
#include "TLI493D.h"
#include <stdio.h>

int main(void) {
	TLI493D_Status tStatus;
	TLI493D_Device tli;
	TLI493D_Configuration config;

	TLI493D_GetDefaultConfiguration(&config);
	tStatus = TLI493D_Init(&tli, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	// TODO: handle error code in tStatus

	while (1) {
		float angle;
		tStatus = TLI493D_GetAngle(&tli, &angle);
		// TODO: handle error code in tStatus

		printf("Angle=%.3f\r\n", angle);
	}
}

```

### Proximity

Following code use sensor for proximity detection with directional compensation. Basicaly it computes magnet's inverse distance from the sensor based. It should report the same distance no matter of angle relative to the sensor.

```
#include "TLI493D.h"
#include <stdio.h>

int main(void) {
	TLI493D_Status tStatus;
	TLI493D_Device tli;
	TLI493D_Configuration config;

	TLI493D_GetDefaultConfiguration(&config);
	tStatus = TLI493D_Init(&tli, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	// TODO: handle error code in tStatus

	while (1) {
		float proximity;
		tStatus = TLI493D_GetProximity(&tli, &proximity);
		// TODO: handle error code in tStatus

		printf("Proximity=%.3f\r\n", proximity);
	}
}
```

### Interrupt
By default device generates interrupt on completing all requested ADC measurements which in default configuration happens periodicaly based on configured update rate. Following example shows proximity, but not continously, instead it waits for interrupt pulse. For demenstration it also lowers update rate to 3 Hz. Platform specific interrupt configuration you need to provide manually and library do not manage it. You do not need to call anything for handling interrupt, just process it in your own way.

```
#include "TLI493D.h"
#include <stdio.h>

int is_interrupt_triggered = 0;

void interrupt_handler() {
	is_interrupt_triggered = 1;
}

int main(void) {
	TLI493D_Status tStatus;
	TLI493D_Device tli;
	TLI493D_Configuration config;

	TLI493D_GetDefaultConfiguration(&config);
	config.updateRate = TLI493D_UpdateRate_3Hz;

	tStatus = TLI493D_Init(&tli, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	// TODO: handle error code in tStatus

	// TODO: configure and enable GPIO falling edge interrupt here

	while (1) {
		if (is_interrupt_triggered) {
			is_interrupt_triggered = 0;

			float proximity;
			tStatus = TLI493D_GetProximity(&tli, &proximity);
			// TODO: handle error code in tStatus

			printf("Proximity=%.3f\r\n", proximity);
		} else {
			__WFI();
		}
	}
}
```
### Wakeup

Wake up feature of the TLI493D chip allows suppressing interrupt pulses on ADC completion when the value is within configured range (which is specified by 6 trehsholds: min and max threshold for X, Y, and Z channel). This feature allow implement wake up mechanism for low-power microcontroller. Microcontroller will wake up only when the value drift from specified range. In most usefull setup is range adaptive and after every event it is readjusted. This mode of operation is highlighted in Infineon document [Usermanual TLI493D-W2BW](https://www.infineon.com/dgdl/Infineon-TLI_493D-W2BW-UserManual-v01_10-EN.pdf?fileId=5546d46273a5366f0173be229e1b1512) in Figure 15 on page 27. Following code show implementation of such mode of operation.

```
#include "TLI493D.h"

#include <stdio.h>
#include <stdint.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

int is_interrupt_triggered = 0;

void interrupt_handler() {
	is_interrupt_triggered = 1;
}

TLI493D_Status updateMagneticThresholds(TLI493D_Device* dev) {
	TLI493D_Status tStatus;
	int16_t x, y, z;

	tStatus = TLI493D_GetDataRaw(dev, &x, &y, &z, NULL);
	if (tStatus) {
		return tStatus;
	}

	TLI493D_Configuration config;
	TLI493D_GetDefaultConfiguration(&config);
	config.enableWakeup = 1;

	config.wakeupThresholdXHigh = MIN(x + 100, 2048);
	config.wakeupThresholdXLow = MAX(x - 100, -2048);

	config.wakeupThresholdYHigh = MIN(y + 100, 2048);
	config.wakeupThresholdYLow = MAX(y - 100, -2048);

	config.wakeupThresholdZHigh = MIN(z + 100, 2048);
	config.wakeupThresholdZLow = MAX(z - 100, -2048);

	tStatus = TLI493D_SetConfiguration(dev, &config);
	if (tStatus) {
		return tStatus;
	}

	return TLI493D_Status_Ok;
}

int main(void) {
	TLI493D_Status tStatus;
	TLI493D_Device tli;
	TLI493D_Configuration config;

	TLI493D_GetDefaultConfiguration(&config);
	tStatus = TLI493D_Init(&tli, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	// TODO: handle error code in tStatus

	tStatus = updateMagneticThresholds(&tli);
	// TODO: handle error code in tStatus

	// TODO: configure and enable GPIO falling edge interrupt here

	while (1) {
		if (is_interrupt_triggered) {
			is_interrupt_triggered = 0;

			float proximity;
			tStatus = TLI493D_GetProximity(&tli, &proximity);
			// TODO: handle error code in tStatus
			
			tStatus = updateMagneticThresholds(&tli);
			// TODO: handle error code in tStatus

			printf("Proximity=%.3f\r\n", proximity);
		} else {
			__WFI();
		}
	}
}
```

