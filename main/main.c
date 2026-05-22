#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// LEDs
int leds[] = {2, 4, 5, 18, 19};

#define NUM_LEDS 5

// Botones
#define BTN_DIRECCION 21
#define BTN_PAUSA 22

// Variables globales
int direccion = 1;
bool pausa_total = false;

// ISR BOTÓN PAUSA
void IRAM_ATTR boton_pausa_isr(void *arg)
{
    pausa_total = true;
}

void app_main(void)
{
    // =========================
    // CONFIGURAR LEDs
    // =========================

    for (int i = 0; i < NUM_LEDS; i++)
    {
        gpio_reset_pin(leds[i]);
        gpio_set_direction(leds[i], GPIO_MODE_OUTPUT);
    }

    // =========================
    // CONFIGURAR BOTÓN DIRECCIÓN
    // =========================

    gpio_reset_pin(BTN_DIRECCION);

    gpio_set_direction(BTN_DIRECCION, GPIO_MODE_INPUT);

    gpio_set_pull_mode(BTN_DIRECCION, GPIO_PULLUP_ONLY);

    // =========================
    // CONFIGURAR BOTÓN PAUSA
    // =========================

    gpio_reset_pin(BTN_PAUSA);

    gpio_set_direction(BTN_PAUSA, GPIO_MODE_INPUT);

    gpio_set_pull_mode(BTN_PAUSA, GPIO_PULLUP_ONLY);

    // Interrupción
    gpio_set_intr_type(BTN_PAUSA, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);

    gpio_isr_handler_add(BTN_PAUSA, boton_pausa_isr, NULL);

    int posicion = 0;

    while (1)
    {
        // =====================================
        // BOTÓN DIRECCIÓN
        // =====================================

        if (gpio_get_level(BTN_DIRECCION) == 0)
        {
            direccion = -direccion;

            printf("Cambio de direccion\n");

            vTaskDelay(pdMS_TO_TICKS(300));
        }

        // =====================================
        // PAUSA TOTAL
        // =====================================

        if (pausa_total)
        {
            pausa_total = false;

            printf("PAUSA ACTIVADA\n");

            // Encender todos los LEDs
            for (int i = 0; i < NUM_LEDS; i++)
            {
                gpio_set_level(leds[i], 1);
            }

            // Esperar 5 segundos
            vTaskDelay(pdMS_TO_TICKS(5000));

            // Apagar todos
            for (int i = 0; i < NUM_LEDS; i++)
            {
                gpio_set_level(leds[i], 0);
            }
        }

        // =====================================
        // SECUENCIA NORMAL
        // =====================================

        gpio_set_level(leds[posicion], 1);

        printf("LED %d ON\n", posicion + 1);

        vTaskDelay(pdMS_TO_TICKS(200));

        gpio_set_level(leds[posicion], 0);

        posicion += direccion;

        // Rebotes extremos
        if (posicion >= NUM_LEDS)
        {
            posicion = NUM_LEDS - 2;
            direccion = -1;
        }

        if (posicion < 0)
        {
            posicion = 1;
            direccion = 1;
        }
    }
}