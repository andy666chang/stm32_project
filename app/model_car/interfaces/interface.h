#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void led_head_set(uint8_t id, bool en);
bool led_head_get(uint8_t id);
void led_tail_set(uint8_t id, bool en);
bool led_tail_get(uint8_t id);
void led_chasis_set(bool en);
bool led_chasis_get(void);
void led_fire_set(bool en);
bool led_fire_get(void);

int led_init(void);
int rc_init(void);
