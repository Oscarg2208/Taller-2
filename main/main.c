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

