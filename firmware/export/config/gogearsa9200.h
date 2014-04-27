/*
 * This config file is for the Philips GoGear SA9200
 */

/* For Rolo and boot loader */
#define MODEL_NUMBER 34
#define MODEL_NAME   "Philips GoGear SA9200"

#define HW_SAMPR_CAPS       (SAMPR_CAP_44)

/* define this if you have a bitmap LCD display */
#define HAVE_LCD_BITMAP

/* define this if you have a colour LCD */
#define HAVE_LCD_COLOR

/* define this if you want album art for this target */
#define HAVE_ALBUMART

/* define this to enable bitmap scaling */
#define HAVE_BMP_SCALING

/* define this to enable JPEG decoding */
#define HAVE_JPEG

/* define this if you have a light associated with the buttons */
#define HAVE_BUTTON_LIGHT

/* define this if you have access to the quickscreen */
#define HAVE_QUICKSCREEN

/* define this if you would like tagcache to build on this target */
#define HAVE_TAGCACHE

/* LCD dimensions */
#define LCD_WIDTH  128
#define LCD_HEIGHT 160
/* sqrt(128^2 + 160^2) / 1.7 = 120.5 */
#define LCD_DPI 121
#define LCD_DEPTH  16   /* 65536 colours */
#define LCD_PIXELFORMAT RGB565 /* rgb565 */

#ifndef BOOTLOADER
/* define this if you have LCD enable function */
#define HAVE_LCD_ENABLE

/* Define this if your LCD can be put to sleep.
   HAVE_LCD_ENABLE should be defined as well. */
#define HAVE_LCD_SLEEP
#define HAVE_LCD_SLEEP_SETTING
#endif

/* define this if you can flip your LCD */
#define HAVE_LCD_FLIP

/* define this if you can invert the colours on your LCD */
#define HAVE_LCD_INVERT

/* #define IRAM_LCDFRAMEBUFFER IDATA_ATTR *//* put the lcd frame buffer in IRAM */

#define CONFIG_KEYPAD PHILIPS_SA9200_PAD

/* Define this to enable morse code input */
#define HAVE_MORSE_INPUT

/* define this if the target has volume keys which can be used in the lists */
#define HAVE_VOLUME_IN_LIST

/* Define this if you do software codec */
#define CONFIG_CODEC SWCODEC

/* There is no hardware tone control */
#define HAVE_SW_TONE_CONTROLS

/* The PP5024 has a built-in AustriaMicrosystems AS3514 */
#define HAVE_AS3514

/* define this if you have a real-time clock */
#ifndef BOOTLOADER
#define CONFIG_RTC RTC_AS3514
#endif

/* Define this if you have a software controlled poweroff */
#define HAVE_SW_POWEROFF

/* Some Sansa E200s seem to be FAT16 formatted */
#define HAVE_FAT16SUPPORT

/* The number of bytes reserved for loadable codecs */
#define CODEC_SIZE 0x100000

/* The number of bytes reserved for loadable plugins */
#define PLUGIN_BUFFER_SIZE 0x80000

#define AB_REPEAT_ENABLE

/* Define this if you can detect headphones */
#define HAVE_HEADPHONE_DETECTION

/* Define this for LCD backlight available */
#define HAVE_BACKLIGHT
#define HAVE_BACKLIGHT_BRIGHTNESS

/* Main LCD backlight brightness range and defaults */
#define MIN_BRIGHTNESS_SETTING      1
#define MAX_BRIGHTNESS_SETTING     12
#define DEFAULT_BRIGHTNESS_SETTING  6

/* Which backlight fading type? */
#define CONFIG_BACKLIGHT_FADING BACKLIGHT_FADING_SW_SETTING

/* define this if you have a light associated with the buttons */
#define HAVE_BUTTON_LIGHT
/* Can't control the brightness on all the buttons */
/* #define HAVE_BUTTONLIGHT_BRIGHTNESS */

/* define this if you have a flash memory storage */
#define HAVE_FLASH_STORAGE

/* define this if the flash memory uses the SecureDigital Memory Card protocol */
#define CONFIG_STORAGE STORAGE_SD

#define BATTERY_CAPACITY_DEFAULT 750    /* default battery capacity */
#define BATTERY_CAPACITY_MIN 750        /* min. capacity selectable */
#define BATTERY_CAPACITY_MAX 750        /* max. capacity selectable */
#define BATTERY_CAPACITY_INC 0          /* capacity increment */
#define BATTERY_TYPES_COUNT  1          /* only one type */

#define CONFIG_BATTERY_MEASURE VOLTAGE_MEASURE

/* Charging implemented in a target-specific algorithm */
#define CONFIG_CHARGING CHARGING_TARGET
#define HAVE_POWEROFF_WHILE_CHARGING

/* define current usage levels (copied from the e200v1 )*/
#define CURRENT_NORMAL     30  /* Toni's measurements in Nov 2008  */
#define CURRENT_BACKLIGHT  40  /* Screen is about 20, blue LEDs are another 20, so 40 if both */
#define CURRENT_RECORD     30  /* flash player, so this is just unboosted current*/

/* define this if the unit can be powered or charged via USB */
#define HAVE_USB_POWER

/* Define this if you have a PortalPlayer PP5024 */
#define CONFIG_CPU PP5024

/* Define this if you want to use the PP5024 i2c interface */
#define CONFIG_I2C I2C_PP5024

/* define this if the hardware can be powered off while charging */
/* Sansa can't be powered off while charging */
/* #define HAVE_POWEROFF_WHILE_CHARGING */

/* The start address index for ROM builds */
#define ROM_START 0x00000000

/* Define this to the CPU frequency */
#define CPU_FREQ 75000000

/* Type of LCD */
#define CONFIG_LCD LCD_SA9200

/* Define this if your LCD can set contrast */
#define HAVE_LCD_CONTRAST

#define MIN_CONTRAST_SETTING        0
#define MAX_CONTRAST_SETTING       31
#define DEFAULT_CONTRAST_SETTING   22 /* Match boot contrast */

/* USB On-the-go */
#define CONFIG_USBOTG USBOTG_ARC

/* enable these for the experimental usb stack */
#define HAVE_USBSTACK
#define USE_ROCKBOX_USB
#define USB_VENDOR_ID 0x0471
#define USB_PRODUCT_ID 0x014f
#define HAVE_USB_HID_MOUSE
#ifdef BOOTLOADER
/* enable bootloader USB mode - ONLY define when also BOOTLOADER */
#define HAVE_BOOTLOADER_USB_MODE
#endif

/* Define this if you have adjustable CPU frequency */
#define HAVE_ADJUSTABLE_CPU_FREQ

#define MI4_FORMAT
#define BOOTFILE_EXT    "mi4"
#define BOOTFILE        "rockbox." BOOTFILE_EXT
#define BOOTDIR "/.rockbox"

/* These values are unused, but need to be defined */
#define FIRMWARE_OFFSET_FILE_CRC    0x0
#define FIRMWARE_OFFSET_FILE_DATA   0x8

#define ICODE_ATTR_TREMOR_NOT_MDCT

#define INCLUDE_TIMEOUT_API

/* Define this if a programmable hotkey is mapped */
#define HAVE_HOTKEY
