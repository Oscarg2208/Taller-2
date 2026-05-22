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