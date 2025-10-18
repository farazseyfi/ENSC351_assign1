
#define _POSIX_C_SOURCE 200809L
#include "hal/led.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define GREEN_TRIGGER_PATH     "/sys/class/leds/ACT/trigger"
#define GREEN_BRIGHTNESS_PATH  "/sys/class/leds/ACT/brightness"
#define RED_TRIGGER_PATH       "/sys/class/leds/PWR/trigger"
#define RED_BRIGHTNESS_PATH    "/sys/class/leds/PWR/brightness"

static void writeToFile(const char *path, const char *val)
{
    FILE *f = fopen(path, "w");
    if (!f) { perror("LED fopen"); exit(EXIT_FAILURE); }
    if (fprintf(f, "%s", val) < 1) { perror("LED fprintf"); fclose(f); exit(EXIT_FAILURE); }
    fclose(f);
}

void LED_init(void)
{
    writeToFile(GREEN_TRIGGER_PATH, "none");
    writeToFile(RED_TRIGGER_PATH, "none");
    struct timespec d = {0, 100 * 1000000}; nanosleep(&d, NULL);
    writeToFile(GREEN_BRIGHTNESS_PATH, "0");
    writeToFile(RED_BRIGHTNESS_PATH, "0");
}

void LED_onGreen(void)  { writeToFile(GREEN_BRIGHTNESS_PATH, "1"); }
void LED_offGreen(void) { writeToFile(GREEN_BRIGHTNESS_PATH, "0"); }
void LED_onRed(void)    { writeToFile(RED_BRIGHTNESS_PATH, "1"); }
void LED_offRed(void)   { writeToFile(RED_BRIGHTNESS_PATH, "0"); }

void LED_cleanup(void)
{
    LED_offGreen();
    LED_offRed();
}
