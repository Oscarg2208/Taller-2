#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define SERVO_PIN 23
#define BTN_PIN 21

#define PWM_FREQUENCY 50
#define PWM_RESOLUTION LEDC_TIMER_13_BIT

int angulos[] = {0, 45, 90, 135, 180};

int indice = 0;

uint32_t angle_to_duty(int angle)
int min_pulse = 500;
int max_pulse = 2500;
int pulse_width = min_pulse +
                  ((max_pulse - min_pulse) * angle / 180);
                  int duty = (pulse_width * 8191) / 20000;
                  gpio_reset_pin(BTN_PIN);

gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);

gpio_set_pull_mode(BTN_PIN, GPIO_PULLUP_ONLY);
ledc_timer_config_t timer_conf =
{
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .timer_num = LEDC_TIMER_0,
    .duty_resolution = PWM_RESOLUTION,
    .freq_hz = PWM_FREQUENCY,
    .clk_cfg = LEDC_AUTO_CLK
};
ledc_timer_config(&timer_conf);
ledc_channel_config_t channel_conf =
{
    .gpio_num = SERVO_PIN,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0
};
ledc_channel_config(&channel_conf);


