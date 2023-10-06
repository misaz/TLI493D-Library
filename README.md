- Support for multiple devices on bus

# Examples

Here are few generic examples how to use the library. They target no specific MCU and you will need to provide MCU specific commands and initialization yourself according to comments.

## Simple usage

Following example configure sensor in default mode and in infinite loop it retrieves (and prints) measured value from sensor. If you use sensor with different address replace `TLI493D_I2C_7BIT_ADDRESS_A0` in the example by constant coresponding to your sensor.

| Product Name | I2C 7-bit Address  | 8-bit Write Addr  | 8-bit Read Addr | TLI493D_Init Address Constant | 
|---|---|---|---|---|
| TLI493D-W2BW A0 | 0x35 | 0x6A | 0x6B | `TLI493D_I2C_7BIT_ADDRESS_A0` |
| TLI493D-W2BW A1 | 0x22 | 0x44 | 0x45 | `TLI493D_I2C_7BIT_ADDRESS_A1` |
| TLI493D-W2BW A2 | 0x78 | 0xF0 | 0xF1 | `TLI493D_I2C_7BIT_ADDRESS_A2` |
| TLI493D-W2BW A3 | 0x44 | 0x88 | 0x89 | `TLI493D_I2C_7BIT_ADDRESS_A3` |

```
#include "TLI493D.h"

int main(void) {
	TLI493D_Status tStatus;
	TLI493D_Device tli;
	TLI493D_Configuration config;

	TLI493D_GetDefaultConfiguration(&config);
	tStatus = TLI493D_Init(&tli, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	// handle error code in tStatus

	while (1) {
		int16_t x, y, z, t;
		tStatus = TLI493D_GetDataRaw(&tli, &x, &y, &z, &t);
		// handle error code in tStatus

		printf("X=%d\t Y=%d\t Z=%d\t T=%d\r\n", x, y, z, t);
	}
}
```

## Multiple sensors
Basicaly exmaples is almost same as in case in previous case. You need to allocate `TLI493D_Device` for every device and call `TLI493D_Init` (with different `TLI493D_I2C_7BIT_ADDRESS_Ax` I2C address constant) for every device. Config structure you can reuse. Library do not use it after initialization call, so you can dealocate it or change it and use for other sensor.

```
#include "TLI493D.h"

int main(void) {
	TLI493D_Status tStatus;
	TLI493D_Device tli1, tli2;
	TLI493D_Configuration config;

	TLI493D_GetDefaultConfiguration(&config);
	tStatus = TLI493D_Init(&tli1, TLI493D_I2C_7BIT_ADDRESS_A0, &config);
	tStatus = TLI493D_Init(&tli2, TLI493D_I2C_7BIT_ADDRESS_A1, &config);
	// handle error code in tStatus

	while (1) {
		int16_t x1, y1, z1, t1, x2, y2, z2, t2;

		tStatus = TLI493D_GetDataRaw(&tli1, &x1, &y1, &z1, &t1);
		// handle error code in tStatus

		tStatus = TLI493D_GetDataRaw(&tli1, &x1, &y1, &z1, &t1);
		// handle error code in tStatus

		printf("X1=%d\t Y1=%d\t Z1=%d\t T1=%d\tX2=%d\t Y2=%d\t Z2=%d\t T2=%d\r\n", x1, y1, z1, t1, x2, y2, z2, t2);
	}
}
```

