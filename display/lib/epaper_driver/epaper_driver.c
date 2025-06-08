#include "epaper_driver.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/task.h"

#define EPD_WIDTH       800
#define EPD_HEIGHT      480

#define PIN_BUSY 25
#define PIN_RST  26
#define PIN_DC   27
#define PIN_CS   15
#define PIN_CLK  13
#define PIN_MOSI 14

static spi_device_handle_t spi;

static void send_command(uint8_t cmd) {
  printf("CMD: 0x%02X\n", cmd);
  gpio_set_level(PIN_DC, 0);
  spi_transaction_t t = {
    .length = 8, 
    .tx_buffer = &cmd
  };
  spi_device_transmit(spi, &t);
}


static void send_data(uint8_t data) {
  gpio_set_level(PIN_DC, 1);
  spi_transaction_t d = {
    .length = 8, 
    .tx_buffer = &data
  };
  spi_device_transmit(spi, &d);
}

static void wait() {
  printf("Checking BUSY...\n");
  while(gpio_get_level(PIN_BUSY)) {
    printf("BUSY 1...\n");
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  printf("BUSY 0...\n");
  fflush(stdout);
}

static void reset() {
  gpio_set_level(PIN_RST, 0);
  vTaskDelay(pdMS_TO_TICKS(200));
  gpio_set_level(PIN_RST, 1);
  vTaskDelay(pdMS_TO_TICKS(200));
  wait();
}

void epaper_init() {
  gpio_set_direction(PIN_DC, GPIO_MODE_OUTPUT); 
  gpio_set_direction(PIN_RST, GPIO_MODE_OUTPUT); 
  gpio_set_direction(PIN_BUSY, GPIO_MODE_OUTPUT); 
  
  spi_bus_config_t buscfg = {
      .mosi_io_num = PIN_MOSI,
      .miso_io_num = -1,
      .sclk_io_num = PIN_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
  };

  spi_device_interface_config_t devcfg = {
      .clock_speed_hz = 4000000,
      .mode = 0,
      .spics_io_num = PIN_CS,
      .queue_size = 1,
  };
  spi_bus_initialize(HSPI_HOST,&buscfg, SPI_DMA_CH_AUTO);
  spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
  
  // reset device
  reset();
  
  // POWER SETTINGS
  send_command(0x01);
  send_data(0x03);
  send_data(0x00);
  send_data(0x2b);
  send_data(0x2b);
  
  // POWER SETTINGS
  send_command(0x06);
  send_data(0x17);
  send_data(0x17);
  send_data(0x17);
  
  send_command(0x04);
  wait();

  send_command(0x00);
  send_data(0x3F);
  
  // RESOLUTION
  send_command(0x61);
  send_data(0x03);
  send_data(0x20);
  send_data(0x01);
  send_data(0xE0);
  
  
  send_command(0x15);
  send_data(0x00);
}

void epaper_clear() {
  send_command(0x10);
  for (int i = 0; i < EPD_WIDTH * EPD_HEIGHT / 8; i ++) {
    send_data(0x00);
  }
  send_command(0x13);
  for (int i = 0; i < EPD_WIDTH * EPD_HEIGHT / 8; i ++) {
    send_data(0xFF);
  }
  send_command(0x12);
  wait();
}

void epaper_sleep() {
  send_command(0x02);
  wait();
  send_command(0x07);
  send_data(0xA5);
}

void epaper_display_image(const uint8_t *black_image, const uint8_t *red_image) {
  send_command(0x10);
  for (int i = 0; i < EPD_WIDTH * EPD_HEIGHT / 8; i ++) {
    send_data(black_image[i]);
  }
  send_command(0x13);
  for (int i = 0; i < EPD_WIDTH * EPD_HEIGHT / 8; i ++) {
    send_data(red_image[i]);
  }
  send_command(0x12);
  wait();
}

