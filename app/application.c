#include <application.h>
#include <bc_servo.h>

#include "smooth.h"
#include "bc_soft_servo.h"

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
smooth_t sm_p4;
smooth_t sm_p5;
smooth_t sm_p6;
smooth_t sm_p7;
smooth_t sm_p8;
smooth_t sm_p12;
smooth_t sm_p14;
smooth_t sm_p15;
smooth_t sm_p16;
smooth_t sm_p17;

#define SERVO_COUNT 20

uint8_t ee_servo_pos[SERVO_COUNT];

#define MAGIC_WORD 0x76767678
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

bool servo_p4_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p4, param);
}

bool servo_p5_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p5, param);
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

bool servo_p15_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p15, param);
}

bool servo_p16_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p16, param);
}

bool servo_p17_set_angle(bc_atci_param_t *param)
{
    return _smooth_set(&sm_p17, param);
}

bool servo_save()
{
    ee_servo_pos[0] = bc_servo_get_angle(&servo_p0);
    ee_servo_pos[1] = bc_servo_get_angle(&servo_p1);
    ee_servo_pos[2] = bc_servo_get_angle(&servo_p2);
    ee_servo_pos[3] = bc_servo_get_angle(&servo_p3);
    ee_servo_pos[4] = bc_soft_servo_get_angle(BC_GPIO_P4);
    ee_servo_pos[5] = bc_soft_servo_get_angle(BC_GPIO_P5);
    ee_servo_pos[6] = bc_soft_servo_get_angle(BC_GPIO_P6);
    ee_servo_pos[7] = bc_soft_servo_get_angle(BC_GPIO_P7);
    ee_servo_pos[8] = bc_soft_servo_get_angle(BC_GPIO_P8);
    ee_servo_pos[12] = bc_servo_get_angle(&servo_p12);
    ee_servo_pos[14] = bc_servo_get_angle(&servo_p14);

    ee_servo_pos[15] = bc_soft_servo_get_angle(BC_GPIO_P15);
    ee_servo_pos[16] = bc_soft_servo_get_angle(BC_GPIO_P16);
    ee_servo_pos[17] = bc_soft_servo_get_angle(BC_GPIO_P17);

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
    bc_atci_printf("+S4=%d", bc_soft_servo_get_angle(BC_GPIO_P4));
    bc_atci_printf("+S5=%d", bc_soft_servo_get_angle(BC_GPIO_P5));
    bc_atci_printf("+S6=%d", bc_soft_servo_get_angle(BC_GPIO_P6));
    bc_atci_printf("+S7=%d", bc_soft_servo_get_angle(BC_GPIO_P7));
    bc_atci_printf("+S8=%d", bc_soft_servo_get_angle(BC_GPIO_P8));
    bc_atci_printf("+S12=%d", bc_servo_get_angle(&servo_p12));
    bc_atci_printf("+S14=%d", bc_servo_get_angle(&servo_p14));

    bc_atci_printf("+S15=%d", bc_soft_servo_get_angle(BC_GPIO_P15));
    bc_atci_printf("+S16=%d", bc_soft_servo_get_angle(BC_GPIO_P16));
    bc_atci_printf("+S17=%d", bc_soft_servo_get_angle(BC_GPIO_P17));

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
        for (int i = 0; i < SERVO_COUNT; i++)
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
        smooth_start(&sm_p4, (float)ee_servo_pos[4]);
        smooth_start(&sm_p5, (float)ee_servo_pos[5]);
        smooth_start(&sm_p6, (float)ee_servo_pos[6]);
        smooth_start(&sm_p7, (float)ee_servo_pos[7]);
        smooth_start(&sm_p8, (float)ee_servo_pos[8]);
        smooth_start(&sm_p12, (float)ee_servo_pos[12]);
        smooth_start(&sm_p14, (float)ee_servo_pos[14]);

        smooth_start(&sm_p15, (float)ee_servo_pos[15]);
        smooth_start(&sm_p16, (float)ee_servo_pos[16]);
        smooth_start(&sm_p17, (float)ee_servo_pos[17]);

        bc_led_pulse(&led, 2000);
    }

    if (event == BC_BUTTON_EVENT_CLICK)
    {
        bc_led_pulse(&led, 200);

        bc_soft_servo_start();
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
    //bc_servo_init(&servo_p6, BC_PWM_P6);
    //bc_servo_init(&servo_p7, BC_PWM_P7);
    //bc_servo_init(&servo_p8, BC_PWM_P8);
    bc_servo_init(&servo_p12, BC_PWM_P12);
    bc_servo_init(&servo_p14, BC_PWM_P14);

    bc_soft_servo_init(BC_GPIO_P4);
    bc_soft_servo_init(BC_GPIO_P5);

    // Hardware PWM for P6,7,8 is using TIM3 which collides with soft_servo
    // that's why we need to enable soft PWM on these pins
    bc_soft_servo_init(BC_GPIO_P6);
    bc_soft_servo_init(BC_GPIO_P7);
    bc_soft_servo_init(BC_GPIO_P8);

    bc_soft_servo_init(BC_GPIO_P15);
    bc_soft_servo_init(BC_GPIO_P16);
    bc_soft_servo_init(BC_GPIO_P17);

    servo_load_from_eeprom();

    // Initialize AT command interface
    static const bc_atci_command_t commands[] = {
            {"$LED", NULL, at_led_set, NULL, NULL, "LED on/off"},

            {"$S12", NULL, servo_p12_set_angle, NULL, NULL, "P12"},
            {"$S14", NULL, servo_p14_set_angle, NULL, NULL, "P14"},
            {"$S15", NULL, servo_p15_set_angle, NULL, NULL, "P15"},
            {"$S16", NULL, servo_p16_set_angle, NULL, NULL, "P16"},
            {"$S17", NULL, servo_p17_set_angle, NULL, NULL, "P17"},

            {"$S0", NULL, servo_p0_set_angle, NULL, NULL, "Set angle on servo on P0"},
            {"$S1", NULL, servo_p1_set_angle, NULL, NULL, "P1"},
            {"$S2", NULL, servo_p2_set_angle, NULL, NULL, "P2"},
            {"$S3", NULL, servo_p3_set_angle, NULL, NULL, "P3"},

            {"$S4", NULL, servo_p4_set_angle, NULL, NULL, "P6"},
            {"$S5", NULL, servo_p5_set_angle, NULL, NULL, "P6"},
            {"$S6", NULL, servo_p6_set_angle, NULL, NULL, "P6"},
            {"$S7", NULL, servo_p7_set_angle, NULL, NULL, "P7"},
            {"$S8", NULL, servo_p8_set_angle, NULL, NULL, "P8"},

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
    smooth_init(&sm_p4,  ee_servo_pos[4]);
    smooth_init(&sm_p5,  ee_servo_pos[5]);
    smooth_init(&sm_p6,  ee_servo_pos[6]);
    smooth_init(&sm_p7,  ee_servo_pos[7]);
    smooth_init(&sm_p8,  ee_servo_pos[8]);
    smooth_init(&sm_p12, ee_servo_pos[12]);
    smooth_init(&sm_p14, ee_servo_pos[14]);

    smooth_init(&sm_p15, ee_servo_pos[15]);
    smooth_init(&sm_p16, ee_servo_pos[16]);
    smooth_init(&sm_p17, ee_servo_pos[17]);


}

void application_task(void)
{
    bc_servo_set_angle(&servo_p0, smooth_get(&sm_p0));
    bc_servo_set_angle(&servo_p1, smooth_get(&sm_p1));
    bc_servo_set_angle(&servo_p2, smooth_get(&sm_p2));
    bc_servo_set_angle(&servo_p3, smooth_get(&sm_p3));
    bc_servo_set_angle(&servo_p12, smooth_get(&sm_p12));
    bc_servo_set_angle(&servo_p14, smooth_get(&sm_p14));

    bc_soft_servo_set_angle(BC_GPIO_P4, smooth_get(&sm_p4));
    bc_soft_servo_set_angle(BC_GPIO_P5, smooth_get(&sm_p5));

    bc_soft_servo_set_angle(BC_GPIO_P6, smooth_get(&sm_p6));
    bc_soft_servo_set_angle(BC_GPIO_P7, smooth_get(&sm_p7));
    bc_soft_servo_set_angle(BC_GPIO_P8, smooth_get(&sm_p8));

    bc_soft_servo_set_angle(BC_GPIO_P15, smooth_get(&sm_p15));
    bc_soft_servo_set_angle(BC_GPIO_P16, smooth_get(&sm_p16));
    bc_soft_servo_set_angle(BC_GPIO_P17, smooth_get(&sm_p17));

    bc_soft_servo_start();

    bc_scheduler_plan_current_relative(20);
}
