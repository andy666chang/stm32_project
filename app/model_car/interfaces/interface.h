#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void led_head_set(uint8_t id, bool en);
bool led_head_get(uint8_t id);
void led_tail_set(uint8_t id, bool en);
bool led_tail_get(uint8_t id);
void led_chasis_set(bool en);
bool led_chasis_get(void);
void led_fire_set(bool en);
bool led_fire_get(void);
void led_string_set(bool en);
void thro_led_update(int16_t thro);
void thro_led_brake(void);
void thro_led_set(uint8_t r, uint8_t g, uint8_t b);
uint8_t led_idx_max(void);

int led_init(void);
int rc_init(void);

int flash_erase(uint32_t addr, uint32_t len);
int flash_write(uint32_t addr, uint8_t *data, uint32_t len);
int flash_read(uint32_t addr, uint8_t *data, uint32_t len);
