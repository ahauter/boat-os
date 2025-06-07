#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_timer.h"

void app_main(void) {
    printf("Hello, ESP32 with ESP-IDF!\n");

    while (1) {
        printf("Uptime: %lld ms\n", esp_timer_get_time() / 1000);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
