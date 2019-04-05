#include <application.h>
#include <bc_servo.h>

#include "smooth.h"

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;

bc_servo_t servo_p0;
bc_servo_t servo_p1;
bc_servo_t servo_p2;
bc_servo_t servo_p3;
bc_servo_t servo_p6;
bc_servo_t servo_p7;
bc_servo_t servo_p8;
bc_servo_t servo_p12;
bc_servo_t servo_p14;

smooth_t sm_p0;
smooth_t sm_p1;
smooth_t sm_p2;
smooth_t sm_p3;
smooth_t sm_p6;
smooth_t sm_p7;
smooth_t sm_p8;
smooth_t sm_p12;
smooth_t sm_p14;


void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{

}

bool at_led_set(bc_atci_param_t *param)
{
    if (param->length != 1)
    {
        return false;
    }

    if (param->txt[0] == '1')
    {
        bc_led_set_mode(&led, BC_LED_MODE_ON);

        return true;
    }

    if (param->txt[0] == '0')
    {
        bc_led_set_mode(&led, BC_LED_MODE_OFF);

        return true;
    }

    return false;
}

bool _servo_set_angle(bc_servo_t *servo, bc_atci_param_t *param)
{
    if (param->length < 1)
    {
        return false;
    }

    uint8_t angle = 0;

    for (size_t i = 0; i < param->length; i++)
    {
        angle *= 10;

        char c = param->txt[param->offset++];

        if (c >= '0' && c <= '9')
        {
            c -= '0';
        }
        else
        {
            return false;
        }

        angle += c;
    }

    bc_servo_set_angle(servo, angle);

    return true;
}


bool _smooth_set(smooth_t *self, bc_atci_param_t *param)
{
    if (param->length < 1)
    {
        return false;
    }

    uint8_t angle = atoi(param->txt);

    int speed = 3000;

    char *comma = strstr(param->txt, ",");
    if(comma != NULL)
    {
        speed = atoi((comma+1));
    }

    self->max_duration = speed;

    smooth_start(self, (float)angle);

    return true;
}


bool servo_p0_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p0, param);
}

bool servo_p1_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p1, param);
}

bool servo_p2_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p2, param);
}

bool servo_p3_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p3, param);
}

bool servo_p6_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p6, param);
}

bool servo_p7_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p7, param);
}

bool servo_p8_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p8, param);
}

bool servo_p12_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p12, param);
}

bool servo_p14_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p14, param);
}

void application_init(void)
{
    // Initialize logging
    bc_log_init(BC_LOG_LEVEL_DUMP, BC_LOG_TIMESTAMP_ABS);

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_pulse(&led, 2000);

    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    bc_servo_init(&servo_p0, BC_PWM_P0);
    bc_servo_init(&servo_p1, BC_PWM_P1);
    bc_servo_init(&servo_p2, BC_PWM_P2);
    bc_servo_init(&servo_p3, BC_PWM_P3);
    bc_servo_init(&servo_p6, BC_PWM_P6);
    bc_servo_init(&servo_p7, BC_PWM_P7);
    bc_servo_init(&servo_p8, BC_PWM_P8);
    bc_servo_init(&servo_p12, BC_PWM_P12);
    bc_servo_init(&servo_p14, BC_PWM_P14);

    // Initialize AT command interface
    static const bc_atci_command_t commands[] = {
            {"$LED", NULL, at_led_set, NULL, NULL, "LED on/off"},
            {"$S0", NULL, servo_p0_set_angle, NULL, NULL, "Set angle on servo on P0"},
            {"$S1", NULL, servo_p1_set_angle, NULL, NULL, "P1"},
            {"$S2", NULL, servo_p2_set_angle, NULL, NULL, "P2"},
            {"$S3", NULL, servo_p3_set_angle, NULL, NULL, "P3"},

            {"$S4", NULL, servo_p6_set_angle, NULL, NULL, "P6"},
            {"$S5", NULL, servo_p7_set_angle, NULL, NULL, "P7"},
            {"$S6", NULL, servo_p8_set_angle, NULL, NULL, "P8"},
            {"$S7", NULL, servo_p12_set_angle, NULL, NULL, "P12"},
            {"$S8", NULL, servo_p14_set_angle, NULL, NULL, "P14"},
            BC_ATCI_COMMAND_CLAC,
            BC_ATCI_COMMAND_HELP
    };
    bc_atci_init(commands, BC_ATCI_COMMANDS_LENGTH(commands));

    smooth_init(&sm_p0);
    smooth_init(&sm_p1);
    smooth_init(&sm_p2);
    smooth_init(&sm_p3);
    smooth_init(&sm_p6);
    smooth_init(&sm_p7);
    smooth_init(&sm_p8);
    smooth_init(&sm_p12);
    smooth_init(&sm_p14);

}

void application_task(void)
{

    bc_servo_set_angle(&servo_p0, smooth_get(&sm_p0));
    bc_servo_set_angle(&servo_p1, smooth_get(&sm_p1));
    bc_servo_set_angle(&servo_p2, smooth_get(&sm_p2));
    bc_servo_set_angle(&servo_p3, smooth_get(&sm_p3));
    bc_servo_set_angle(&servo_p6, smooth_get(&sm_p6));
    bc_servo_set_angle(&servo_p7, smooth_get(&sm_p7));
    bc_servo_set_angle(&servo_p8, smooth_get(&sm_p8));
    bc_servo_set_angle(&servo_p12, smooth_get(&sm_p12));
    bc_servo_set_angle(&servo_p14, smooth_get(&sm_p14));

    bc_scheduler_plan_current_relative(20);
}
