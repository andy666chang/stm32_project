
#include <Wire.h>
#include <VL53L0X.h>

#define SEN_NUM 1

VL53L0X sensors[SEN_NUM];

#define LED_PIN A0
uint8_t led_pin[SEN_NUM] = {A0};
uint32_t time_cnt[SEN_NUM];

void setup()
{
  Serial.begin(115200);

  for (uint8_t i = 0; i < SEN_NUM; i++) {
    pinMode( led_pin[i], OUTPUT );
    digitalWrite( led_pin[i], LOW );

    time_cnt[i] = 0;
  }

  Wire.begin();
  Wire.setClock(200000);

  for (uint8_t i = 0; i < SEN_NUM; i++) {
    sensors[i].setTimeout(50);
    if (!sensors[i].init())
    {
      Serial.println("Failed to detect and initialize sensor!");
      while (1) {}
    }

    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    sensors[i].startContinuous();
  }
}

void loop()
{
  int min = 100, max = 250;
  // Serial.print("min: ");
  Serial.print(min);
  Serial.print(" ");

  // Serial.print("max: ");
  Serial.print(max);
  Serial.print(" ");

  for (uint8_t i = 0; i < SEN_NUM; i++) {
    // Serial.print("Dis: ");
    uint16_t data = sensors[i].readRangeContinuousMillimeters();
    // Serial.print("mm");
    if (sensors[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }

    if (data > 250) {
      data = 250;
    } else if (data < 100) {
      data = 100;
    }

    // Serial.print("data: ");
    Serial.print(data);
    Serial.print(" ");

    // Serial.print("flag: ");
    if (data < 180) {
      Serial.print("200 ");
      time_cnt[i] = millis();
      digitalWrite( led_pin[i], HIGH );
    } else {
      Serial.print("100 ");
    }
  }

  Serial.println();

  uint16_t ms = millis();
  for (uint8_t i = 0; i < SEN_NUM; i++) {
    if (ms - time_cnt[i] > 100) {
      digitalWrite( led_pin[i], LOW );
    }
  }


  _delay_ms(10);
}
