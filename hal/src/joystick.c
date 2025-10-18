/*
 * SPI-based Joystick Reader using 12-bit ADC (MCP3204/MCP3008 style)
 * Adapted from SPI ADC sample for ENSC 351 A1 – Faraz Seyfi 2025
 */
#include "hal/joystick.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEV   "/dev/spidev0.0"
#define SPI_SPEED 250000
#define SPI_BITS  8
#define SPI_MODE  0

// --- channel mapping (0:Y,1:X,2/3 optional) ---
#define CH_Y 0
#define CH_X 1

static int fd = -1;

static int read_ch(int ch)
{
    uint8_t tx[3] = { (uint8_t)(0x06 | ((ch & 0x04) >> 2)),
                      (uint8_t)((ch & 0x03) << 6), 0x00 };
    uint8_t rx[3] = {0};
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 3,
        .speed_hz = SPI_SPEED,
        .bits_per_word = SPI_BITS,
        .cs_change = 0
    };
    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        perror("SPI transfer");
        return 0;
    }
    return ((rx[1] & 0x0F) << 8) | rx[2]; // 12-bit result
}

void Joystick_init(void)
{
    fd = open(SPI_DEV, O_RDWR);
    if (fd < 0) { perror("open spidev"); exit(EXIT_FAILURE); }
    uint8_t mode = SPI_MODE, bits = SPI_BITS;
    uint32_t speed = SPI_SPEED;
    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
}

// Threshold detection (auto-centered simple version)
JoystickDirection Joystick_read(void)
{
    int y = read_ch(CH_Y);
    int x = read_ch(CH_X);

    const int center = 2048;
    const int margin = 600;

    if (y > center + margin) return JOY_UP;
    if (y < center - margin) return JOY_DOWN;
    if (x > center + margin) return JOY_RIGHT;
    if (x < center - margin) return JOY_LEFT;
    return JOY_NONE;
}

void Joystick_cleanup(void)
{
    if (fd >= 0) { close(fd); fd = -1; }
}
