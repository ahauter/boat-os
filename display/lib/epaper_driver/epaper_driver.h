#pragma once

#include <stdint.h>

void epaper_init(void);
void epaper_clear(void);
void epaper_sleep(void);
void epaper_display_image(const uint8_t *black_image, const uint8_t *red_image);
