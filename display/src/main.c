#include "epaper_driver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define EPD_WIDTH       800
#define EPD_HEIGHT      480
void app_main() {
    /*int width_bytes = EPD_WIDTH / 8;
    int total_bytes = width_bytes * EPD_HEIGHT;

    // Allocate buffers
    uint8_t black_buffer[total_bytes];
    uint8_t red_buffer[total_bytes];

    // Prepare black buffer: left half black (0), right half white (1)
    for (int y = 0; y < EPD_HEIGHT; y++) {
        for (int x_byte = 0; x_byte < width_bytes; x_byte++) {
            int idx = y * width_bytes + x_byte;
            if (x_byte < width_bytes / 2) {
                black_buffer[idx] = 0x00;  // all black in this byte
            } else {
                black_buffer[idx] = 0xFF;  // all white
            }
        }
    }

    // Prepare red buffer: right half red (0), left half no red (1)
    for (int y = 0; y < EPD_HEIGHT; y++) {
        for (int x_byte = 0; x_byte < width_bytes; x_byte++) {
            int idx = y * width_bytes + x_byte;
            if (x_byte >= width_bytes / 2) {
                red_buffer[idx] = 0x00;  // red pixels
            } else {
                red_buffer[idx] = 0xFF;  // no red pixels
            }
        }
    }

    */ // Initialize display (reset + LUT)
    epaper_init();

    // Display the test pattern
    //epaper_display_image(black_buffer, red_buffer);

    vTaskDelay(pdMS_TO_TICKS(10000));
    epaper_clear();
    epaper_sleep();
    // Optional: loop or sleep
}
