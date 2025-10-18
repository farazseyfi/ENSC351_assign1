#ifndef LED_H_
#define LED_H_




// Intiallizing both LEDs 
void LED_init(void);
// Turns the green LED ON
void LED_onGreen(void);
// Turns the Green LED off
void LED_offGreen(void);
// Turns the red LED ON
void LED_onRed(void);
// Turns the red LED off
void LED_offRed(void);
// Turns both LEDs off (to clean up)
void LED_cleanup(void);

#endif
