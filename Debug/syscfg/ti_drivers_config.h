/*
 *  ======== ti_drivers_config.h ========
 *  Configured TI-Drivers module declarations
 *
 *  DO NOT EDIT - This file is generated for the CC3220SF_LAUNCHXL
 *  by the SysConfig tool.
 */
#ifndef ti_drivers_config_h
#define ti_drivers_config_h

#define CONFIG_SYSCONFIG_PREVIEW

#define CONFIG_CC3220SF_LAUNCHXL

#ifndef DeviceFamily_CC3220
#define DeviceFamily_CC3220
#endif

#include <ti/devices/DeviceFamily.h>

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== Crypto ========
 */

#define CONFIG_Crypto_0             0

/*
 *  ======== GPIO ========
 */

/* P64, LaunchPad LED D10 (Red) */
#define CONFIG_GPIO_LED_0           0
/* P01, LaunchPad LED D9 (Yellow) */
#define CONFIG_GPIO_LED_1           1
/* P02, LaunchPad LED D8 (Green) */
#define CONFIG_GPIO_LED_2           2
/* P45 */
#define CONFIG_GPIO_PIN_1           3
/* P53 */
#define CONFIG_GPIO_PIN_2           4
/* P58 */
#define CONFIG_GPIO_PIN_3           5
/* P59 */
#define CONFIG_GPIO_PIN_4           6
/* P18 */
#define CONFIG_GPIO_PIN_5           7
/* P50 */
#define CONFIG_GPIO_PIN_7           8
/* P21 */
#define SPI_Encoder1                9
/* P61 */
#define SPI_Encoder2                10
/* P08 */
#define SPI_Encoder3                11

/* LEDs are active high */
#define CONFIG_GPIO_LED_ON  (1)
#define CONFIG_GPIO_LED_OFF (0)

#define CONFIG_LED_ON  (CONFIG_GPIO_LED_ON)
#define CONFIG_LED_OFF (CONFIG_GPIO_LED_OFF)


/*
 *  ======== SPI ========
 */

#define CONFIG_NWP_SPI               0

/*
 *  MOSI: 07
 *  MISO: 06
 *  SCLK: 05
 */
#define CONFIG_SPI_0                  1


/*
 *  ======== Timer ========
 */

#define CONFIG_TIMER_0              0
#define CONFIG_TIMER_1              1

/*
 *  ======== UART ========
 */

/*
 *  TX: P03
 *  RX: Unassigned
 */
#define CONFIG_UART_0               0
/*
 *  TX: P55
 *  RX: P57
 *  XDS110 UART
 */
#define CONFIG_UART_1               1


/*
 *  ======== Watchdog ========
 */

#define CONFIG_WATCHDOG_0           0


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
