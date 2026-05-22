#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "esp_adc/adc_oneshot.h"

// =========================
// ADC
// =========================

adc_oneshot_unit_handle_t adc_handle;

// =========================
// BANDERA TIMER
// =========================

volatile bool leer_adc = false;

// =========================
// ISR TIMER
// =========================

static bool IRAM_ATTR timer_callback(
    gptimer_handle_t timer,
    const gptimer_alarm_event_data_t *edata,
    void *user_ctx)
{
    leer_adc = true;

    return false;
}

void app_main(void)
{
    // =========================
    // CONFIGURAR ADC
    // =========================

    adc_oneshot_unit_init_cfg_t init_config =
        {
            .unit_id = ADC_UNIT_1};

    adc_oneshot_new_unit(
        &init_config,
        &adc_handle);

    adc_oneshot_chan_cfg_t config =
        {
            .bitwidth = ADC_BITWIDTH_DEFAULT,
            .atten = ADC_ATTEN_DB_12};

    adc_oneshot_config_channel(
        adc_handle,
        ADC_CHANNEL_6,
        &config);

    // GPIO34 = ADC_CHANNEL_6

    // =========================
    // CONFIGURAR GPTIMER
    // =========================

    gptimer_handle_t gptimer = NULL;

    gptimer_config_t timer_config =
        {
            .clk_src = GPTIMER_CLK_SRC_DEFAULT,
            .direction = GPTIMER_COUNT_UP,
            .resolution_hz = 1000000};

    gptimer_new_timer(
        &timer_config,
        &gptimer);

    // =========================
    // CALLBACK TIMER
    // =========================

    gptimer_event_callbacks_t cbs =
        {
            .on_alarm = timer_callback};

    gptimer_register_event_callbacks(
        gptimer,
        &cbs,
        NULL);

    // =========================
    // ALARMA CADA 10ms
    // =========================

    gptimer_alarm_config_t alarm_config =
        {
            .alarm_count = 10000,
            .reload_count = 0,
            .flags.auto_reload_on_alarm = true};

    gptimer_set_alarm_action(
        gptimer,
        &alarm_config);

    // =========================
    // INICIAR TIMER
    // =========================

    gptimer_enable(gptimer);

    gptimer_start(gptimer);

    printf("ADC + GPTimer iniciado\n");

    int adc_raw = 0;

    while (1)
    {
        if (leer_adc)
        {
            leer_adc = false;

            adc_oneshot_read(
                adc_handle,
                ADC_CHANNEL_6,
                &adc_raw);

            printf("ADC: %d\n", adc_raw);
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}