#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define SERVO_PIN 23
#define BTN_PIN 21

// PWM SERVO
#define PWM_FREQUENCY 50
#define PWM_RESOLUTION LEDC_TIMER_13_BIT

// Ángulos
int angulos[] = {0, 45, 90, 135, 180};

int indice = 0;

// Convertir ángulo a duty
uint32_t angle_to_duty(int angle)
{
    // Aproximadamente:
    // 0°   -> 500us
    // 180° -> 2500us

    int min_pulse = 500;
    int max_pulse = 2500;

    int pulse_width = min_pulse +
                      ((max_pulse - min_pulse) * angle / 180);

    // Periodo 20ms para 50Hz
    int duty = (pulse_width * 8191) / 20000;

    return duty;
}

void app_main(void)
{
    // =========================
    // CONFIGURAR BOTÓN
    // =========================

    gpio_reset_pin(BTN_PIN);

    gpio_set_direction(BTN_PIN, GPIO_MODE_INPUT);

    gpio_set_pull_mode(BTN_PIN, GPIO_PULLUP_ONLY);

    // =========================
    // CONFIGURAR TIMER PWM
    // =========================

    ledc_timer_config_t timer_conf =
        {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .duty_resolution = PWM_RESOLUTION,
            .freq_hz = PWM_FREQUENCY,
            .clk_cfg = LEDC_AUTO_CLK};

    ledc_timer_config(&timer_conf);

    // =========================
    // CONFIGURAR CANAL PWM
    // =========================

    ledc_channel_config_t channel_conf =
        {
            .gpio_num = SERVO_PIN,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .channel = LEDC_CHANNEL_0,
            .timer_sel = LEDC_TIMER_0,
            .duty = 0,
            .hpoint = 0};

    ledc_channel_config(&channel_conf);

    printf("Servo iniciado\n");

    while (1)
    {
        // Botón presionado
        if (gpio_get_level(BTN_PIN) == 0)
        {
            int angulo = angulos[indice];

            uint32_t duty = angle_to_duty(angulo);

            ledc_set_duty(
                LEDC_HIGH_SPEED_MODE,
                LEDC_CHANNEL_0,
                duty);

            ledc_update_duty(
                LEDC_HIGH_SPEED_MODE,
                LEDC_CHANNEL_0);

            printf("Angulo: %d grados\n", angulo);

            indice++;

            if (indice >= 5)
            {
                indice = 0;
            }

            // Anti rebote
            vTaskDelay(pdMS_TO_TICKS(400));
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


