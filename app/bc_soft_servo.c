

#include "bc_soft_servo.h"

static struct
{
    uint32_t _channel_enabled;
    uint8_t _current_channel;
    uint16_t pulse_length[20];
    uint16_t angle[20];
    bool _is_initialized;

} _bc_soft_servo;

#define _BC_SOFT_SERVO_PULSE_MIN 544
#define _BC_SOFT_SERVO_PULSE_MAX 2400

static void _bc_soft_servo_init_timer();
void _bc_soft_servo_TIM3_handler(void *param);

void bc_soft_servo_init(bc_gpio_channel_t gpio)
{
    if (!_bc_soft_servo._is_initialized)
    {
        memset(&_bc_soft_servo, 0, sizeof(_bc_soft_servo));
        _bc_soft_servo_init_timer();
        _bc_soft_servo._is_initialized = true;

    }

    bc_gpio_init(gpio);
    bc_gpio_set_mode(gpio, BC_GPIO_MODE_OUTPUT);
    bc_gpio_set_output(gpio, 0);

    _bc_soft_servo._channel_enabled |= 1 << gpio;
}

void bc_soft_servo_set_pulse_length(bc_gpio_channel_t gpio, uint16_t pulse_length)
{
    _bc_soft_servo.pulse_length[gpio] = pulse_length;
}

uint16_t bc_soft_servo_get_pulse_length(bc_gpio_channel_t gpio)
{
    return _bc_soft_servo.pulse_length[gpio];
}

void bc_soft_servo_set_angle(bc_gpio_channel_t gpio, uint8_t angle)
{
    if (angle > 180)
    {
        angle = 180;
    }

    _bc_soft_servo.angle[gpio] = angle;

    uint16_t pwm_value = ((_BC_SOFT_SERVO_PULSE_MAX - _BC_SOFT_SERVO_PULSE_MIN) * ((float) angle / 180.f)) + _BC_SOFT_SERVO_PULSE_MIN;
    bc_soft_servo_set_pulse_length(gpio, pwm_value);
}

uint8_t bc_soft_servo_get_angle(bc_gpio_channel_t gpio)
{
    return _bc_soft_servo.angle[gpio];
}


static void _bc_soft_servo_init_timer()
{
    // Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Errata workaround
    RCC->APB1ENR;

    // Disable counter if it is running
    TIM3->CR1 &= ~TIM_CR1_CEN;

    uint32_t resolution_us = 1;
    uint32_t period_cycles = 3000;

    // Set prescaler to 5 * 32 (5 microseconds resolution)
    TIM3->PSC = resolution_us * 32 - 1;
    TIM3->ARR = period_cycles - 1;

    TIM3->DIER |= TIM_DIER_UIE;
    // Enable TIM3 interrupts
    NVIC_EnableIRQ(TIM3_IRQn);

    bc_timer_set_irq_handler(TIM3, _bc_soft_servo_TIM3_handler, NULL);
}

static void _bc_soft_servo_plan_next()
{
    bool found = false;

    for (int i = _bc_soft_servo._current_channel + 1; i < 32; i++)
    {
        if (_bc_soft_servo._channel_enabled & (1 << i))
        {
            found = true;
            _bc_soft_servo._current_channel = i;
            break;
        }
    }

    if (found)
    {
        bc_gpio_set_output(_bc_soft_servo._current_channel, 1);

        // Stop timer
        TIM3->CR1 &= ~TIM_CR1_CEN;

        TIM3->ARR = _bc_soft_servo.pulse_length[_bc_soft_servo._current_channel] - 1;
        TIM3->CR1 |= TIM_CR1_CEN;
    }
    else
    {
        // no more channels, stop
    }

}

void bc_soft_servo_start()
{
    _bc_soft_servo._current_channel = 0;
    _bc_soft_servo_plan_next();
}

void _bc_soft_servo_TIM3_handler(void *param)
{
    (void) param;
    TIM3->SR = ~TIM_DIER_UIE;
    // Stop timer
    TIM3->CR1 &= ~TIM_CR1_CEN;

    bc_gpio_set_output(_bc_soft_servo._current_channel, 0);

    _bc_soft_servo_plan_next();
}

