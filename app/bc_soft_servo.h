#ifndef _BC_SOFT_SERVO_H
#define _BC_SOFT_SERVO_H


#include <bc_system.h>
#include <bc_gpio.h>
#include <stm32l0xx.h>
#include <bc_timer.h>

void bc_soft_servo_init(bc_gpio_channel_t gpio);
void bc_soft_servo_start();
void bc_soft_servo_set_pulse_length(bc_gpio_channel_t gpio, uint16_t pulse_length);
void bc_soft_servo_set_angle(bc_gpio_channel_t gpio, uint8_t angle);

uint16_t bc_soft_servo_get_pulse_length(bc_gpio_channel_t gpio);
uint8_t bc_soft_servo_get_angle(bc_gpio_channel_t gpio);

#endif
