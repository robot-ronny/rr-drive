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

uint8_t ee_servo_pos[9];

#define MAGIC_WORD 0x76767676
#define EE_ADDRESS_MAGIC_WORD 0
#define EE_ADDRESS_SERVO 4


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

bool _smooth_set(smooth_t *self, bc_atci_param_t *param)
{

    bc_led_pulse(&led, 50);

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

bool servo_save()
{
    ee_servo_pos[0] = bc_servo_get_angle(&servo_p0);
    ee_servo_pos[1] = bc_servo_get_angle(&servo_p1);
    ee_servo_pos[2] = bc_servo_get_angle(&servo_p2);
    ee_servo_pos[3] = bc_servo_get_angle(&servo_p3);
    ee_servo_pos[4] = bc_servo_get_angle(&servo_p6);
    ee_servo_pos[5] = bc_servo_get_angle(&servo_p7);
    ee_servo_pos[6] = bc_servo_get_angle(&servo_p8);
    ee_servo_pos[7] = bc_servo_get_angle(&servo_p12);
    ee_servo_pos[8] = bc_servo_get_angle(&servo_p14);

    uint32_t magic_word = MAGIC_WORD;
    bc_eeprom_write(EE_ADDRESS_MAGIC_WORD, &magic_word, sizeof(magic_word));
    bc_eeprom_write(EE_ADDRESS_SERVO, &ee_servo_pos, sizeof(ee_servo_pos));
    return true;
}

bool servo_status()
{

    bc_atci_printf("+S0=%d", bc_servo_get_angle(&servo_p0));
    bc_atci_printf("+S1=%d", bc_servo_get_angle(&servo_p1));
    bc_atci_printf("+S2=%d", bc_servo_get_angle(&servo_p2));
    bc_atci_printf("+S3=%d", bc_servo_get_angle(&servo_p3));
    bc_atci_printf("+S4=%d", bc_servo_get_angle(&servo_p6));
    bc_atci_printf("+S5=%d", bc_servo_get_angle(&servo_p7));
    bc_atci_printf("+S6=%d", bc_servo_get_angle(&servo_p8));
    bc_atci_printf("+S7=%d", bc_servo_get_angle(&servo_p12));
    bc_atci_printf("+S8=%d", bc_servo_get_angle(&servo_p14));

    return true;
}

void servo_load_from_eeprom()
{
    uint32_t magic_word;
    bc_eeprom_read(EE_ADDRESS_MAGIC_WORD, &magic_word, sizeof(magic_word));

    if (magic_word == MAGIC_WORD)
    {
        bc_eeprom_read(EE_ADDRESS_SERVO, &ee_servo_pos, sizeof(ee_servo_pos));
    }
    else
    {
        for (int i = 0; i < 9; i++)
        {
            ee_servo_pos[i] = 10;
        }
    }

}

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event == BC_BUTTON_EVENT_HOLD)
    {
        servo_load_from_eeprom();

        smooth_start(&sm_p0, (float)ee_servo_pos[0]);
        smooth_start(&sm_p1, (float)ee_servo_pos[1]);
        smooth_start(&sm_p2, (float)ee_servo_pos[2]);
        smooth_start(&sm_p3, (float)ee_servo_pos[3]);
        smooth_start(&sm_p6, (float)ee_servo_pos[4]);
        smooth_start(&sm_p7, (float)ee_servo_pos[5]);
        smooth_start(&sm_p8, (float)ee_servo_pos[6]);
        smooth_start(&sm_p12, (float)ee_servo_pos[7]);
        smooth_start(&sm_p14, (float)ee_servo_pos[8]);

        bc_led_pulse(&led, 2000);
    }

    if (event == BC_BUTTON_EVENT_CLICK)
    {
        bc_led_pulse(&led, 200);
    }
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
    bc_button_set_hold_time(&button, 1000);

    bc_servo_init(&servo_p0, BC_PWM_P0);
    bc_servo_init(&servo_p1, BC_PWM_P1);
    bc_servo_init(&servo_p2, BC_PWM_P2);
    bc_servo_init(&servo_p3, BC_PWM_P3);
    bc_servo_init(&servo_p6, BC_PWM_P6);
    bc_servo_init(&servo_p7, BC_PWM_P7);
    bc_servo_init(&servo_p8, BC_PWM_P8);
    bc_servo_init(&servo_p12, BC_PWM_P12);
    bc_servo_init(&servo_p14, BC_PWM_P14);

    servo_load_from_eeprom();

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

            {"$SAVE", servo_save, NULL, NULL, NULL, "Save current servo position as default to the EEPROM"},

            {"$STATUS", servo_status, NULL, NULL, NULL, "Get current servo position"},

            BC_ATCI_COMMAND_CLAC,
            BC_ATCI_COMMAND_HELP
    };
    bc_atci_init(commands, BC_ATCI_COMMANDS_LENGTH(commands));

    smooth_init(&sm_p0,  ee_servo_pos[0]);
    smooth_init(&sm_p1,  ee_servo_pos[1]);
    smooth_init(&sm_p2,  ee_servo_pos[2]);
    smooth_init(&sm_p3,  ee_servo_pos[3]);
    smooth_init(&sm_p6,  ee_servo_pos[4]);
    smooth_init(&sm_p7,  ee_servo_pos[5]);
    smooth_init(&sm_p8,  ee_servo_pos[6]);
    smooth_init(&sm_p12, ee_servo_pos[7]);
    smooth_init(&sm_p14, ee_servo_pos[8]);
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
