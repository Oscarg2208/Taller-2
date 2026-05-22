#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "esp_adc/adc_oneshot.h"

adc_oneshot_unit_handle_t adc_handle;
volatile bool leer_adc = false;
