#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "esp_adc/adc_oneshot.h"

adc_oneshot_unit_handle_t adc_handle;
volatile bool leer_adc = false;
static bool IRAM_ATTR timer_callback(
    gptimer_handle_t timer,
    const gptimer_alarm_event_data_t *edata,
    void *user_ctx)
{
    leer_adc = true;

    return false;
}
adc_oneshot_unit_init_cfg_t init_config =
{
    .unit_id = ADC_UNIT_1
};
adc_oneshot_new_unit(
    &init_config,
    &adc_handle);
  adc_oneshot_chan_cfg_t config =
{
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    .atten = ADC_ATTEN_DB_12
};
adc_oneshot_config_channel(
    adc_handle,
    ADC_CHANNEL_6,
    &config);
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config =
{
    .clk_src = GPTIMER_CLK_SRC_DEFAULT,
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 1000000
};
gptimer_new_timer(
    &timer_config,
    &gptimer);
    
