/*
 * ENSC 351 – Assignment 1 
 * Reaction Timer using BeagleY-AI
 * Author: Faraz Seyfi - #301543610
 * 
 * LEDs indicate direction
 * Player must move the joystick correctly
 * Left/Right exists the game
 * A 5s timeout also ends the loop
 * 
 */
#define _POSIX_C_SOURCE 200809L



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "hal/led.h"
#include "hal/joystick.h"

// A Helper function to get current time in ms
static long long nowMs(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000;
}
// A helper - sleep for given ms
static void sleepMs(long long ms)
{
    struct timespec d = { .tv_sec = ms/1000, .tv_nsec = (ms%1000)*1000000 };
    nanosleep(&d, NULL);
}

int main(void)
{
    printf("Hello embedded world, from Faraz Seyfi!\n");
    printf("When the LEDs light up, press the joystick in that direction!\n");
    printf("(Press left or right to exit)\n\n");

    srand(time(NULL)); // seed random generator
    LED_init(); //setup the LEDs
    Joystick_init(); // Setup the joystick

    long long best = -1; // Store the best (lowest) reaction time

    while (true) {
        // 1) Get ready flashes
        printf("Get ready...\n");
        for (int i=0;i<4;i++){
            LED_onGreen(); sleepMs(250); LED_offGreen();
            LED_onRed();   sleepMs(250); LED_offRed();
        }

        // 2) Wait for release
        if (Joystick_read() != JOY_NONE) {
            printf("Please let go of joystick.\n");
            while (Joystick_read() != JOY_NONE) sleepMs(50);
        }

        // 3) Random delay 0.5–3 s
        long long delay = 500 + rand()%2500;
        sleepMs(delay);

        // 4) Too soon
        if (Joystick_read() != JOY_NONE) { printf("Too soon!\n\n"); continue; }

        // 5) Choose direction
        bool up = rand()%2;
        if (up){ printf("Press UP now!\n"); LED_onGreen(); }
        else   { printf("Press DOWN now!\n"); LED_onRed();  }

        // 6) Measure reaction
        long long start = nowMs();
        const long long TIMEOUT = 5000;
        JoystickDirection in = JOY_NONE;
        while (nowMs() - start < TIMEOUT){
            in = Joystick_read();
            if (in != JOY_NONE) break;
            sleepMs(10);
        }
        long long elapsed = nowMs() - start;

        if (elapsed >= TIMEOUT){ printf("No input within 5 s; quitting!\n"); break; }
        if (in == JOY_LEFT || in == JOY_RIGHT){ printf("User selected to quit.\n"); break; }

        // 7) Evaluate
        if ((up && in==JOY_UP) || (!up && in==JOY_DOWN)){
            printf("Correct! Reaction = %lld ms\n", elapsed);
            if (best < 0 || elapsed < best){ best = elapsed; printf("New best time!\n"); }
            printf("Best so far = %lld ms\n", best);
            for (int i=0;i<5;i++){ LED_onGreen(); sleepMs(100); LED_offGreen(); sleepMs(100); }
        } else {
            printf("Incorrect.\n");
            for (int i=0;i<5;i++){ LED_onRed(); sleepMs(100); LED_offRed(); sleepMs(100); }
        }
        printf("\n");
        LED_offGreen(); LED_offRed();
    }

    // Clean up before existing
    LED_cleanup();
    Joystick_cleanup();
    printf("\nGame over. Thanks for playing!\n");
    return 0;
}
