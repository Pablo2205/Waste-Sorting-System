#ifndef LEDS_H
#define LEDS_H

#include "config.h"
#include <stdint.h>

void leds_init(void);
void leds_on(uint8_t led);     // led: define por config (LED_METAL, LED_PLASTIC, ...)
void leds_off(uint8_t led);
void leds_off_all(void);

#endif // LEDS_H
