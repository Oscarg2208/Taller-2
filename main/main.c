#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

int leds[] = {2, 4, 5, 18, 19};

#define NUM_LEDS 5
#define BTN_DIRECCION 21
#define BTN_PAUSA 22

int direccion = 1;
bool pausa_total = false; 

void IRAM_ATTR boton_pausa_isr(void *arg)
{
    pausa_total = true;
}
for (int i = 0; i < NUM_LEDS; i++)
{
    gpio_reset_pin(leds[i]);
    gpio_set_direction(leds[i], GPIO_MODE_OUTPUT);
}
gpio_reset_pin(BTN_DIRECCION);

gpio_set_direction(BTN_DIRECCION, GPIO_MODE_INPUT);

gpio_set_pull_mode(BTN_DIRECCION, GPIO_PULLUP_ONLY);
gpio_set_intr_type(BTN_PAUSA, GPIO_INTR_NEGEDGE);

gpio_install_isr_service(0);

gpio_isr_handler_add(BTN_PAUSA, boton_pausa_isr, NULL);
if (gpio_get_level(BTN_DIRECCION) == 0)
{
    direccion = -direccion;

    printf("Cambio de direccion\n");

    vTaskDelay(pdMS_TO_TICKS(300));
}