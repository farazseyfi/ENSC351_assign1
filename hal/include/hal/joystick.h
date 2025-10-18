#ifndef JOYSTICK_H_
#define JOYSTICK_H_

// Enumeration for joystick movement directions
typedef enum {
    JOY_NONE, // Not moved
    JOY_UP,
    JOY_DOWN,
    JOY_LEFT,
    JOY_RIGHT
} JoystickDirection;


// Initilaize SPI joystick interface
void Joystick_init(void);
// Read and return the current joystick direction
JoystickDirection Joystick_read(void);
// Cleanup
void Joystick_cleanup(void);

#endif
