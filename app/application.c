#include <application.h>
#include <bc_servo.h>

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;

bc_servo_t servo_p0;
bc_servo_t servo_p1;
bc_servo_t servo_p2;
bc_servo_t servo_p3;

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

bool servo_p0_set_angle(bc_atci_param_t *param)
{
    return _servo_set_angle(&servo_p0, param);
}

bool servo_p1_set_angle(bc_atci_param_t *param)
{
    return _servo_set_angle(&servo_p1, param);
}

bool servo_p2_set_angle(bc_atci_param_t *param)
{
    return _servo_set_angle(&servo_p2, param);
}

bool servo_p3_set_angle(bc_atci_param_t *param)
{
    return _servo_set_angle(&servo_p3, param);
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

    // Initialize AT command interface
    static const bc_atci_command_t commands[] = {
            {"$LED", NULL, at_led_set, NULL, NULL, "LED on/off"},
            {"$S0", NULL, servo_p0_set_angle, NULL, NULL, "Set angle on servo on P0"},
            {"$S1", NULL, servo_p1_set_angle, NULL, NULL, "Set angle on servo on P1"},
            {"$S2", NULL, servo_p2_set_angle, NULL, NULL, "Set angle on servo on P2"},
            {"$S3", NULL, servo_p3_set_angle, NULL, NULL, "Set angle on servo on P3"},
            BC_ATCI_COMMAND_CLAC,
            BC_ATCI_COMMAND_HELP
    };
    bc_atci_init(commands, BC_ATCI_COMMANDS_LENGTH(commands));
}

void application_task(void)
{

}
