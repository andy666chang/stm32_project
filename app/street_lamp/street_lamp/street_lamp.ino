
#include <Wire.h>
#include <VL53L0X.h>

#define SEN_NUM 5
#define LED_KEEP 200

struct dev_t {
  uint8_t en_pin;
  uint8_t led_pin;
  uint8_t check;
  unsigned long time_cnt;
  VL53L0X sensor;
} sens[SEN_NUM] = {
  {.en_pin = 2, .led_pin = 13,},
  {.en_pin = 3, .led_pin = 8,},
  {.en_pin = 4, .led_pin = 9,},
  {.en_pin = 5, .led_pin = 10,},
  {.en_pin = 6, .led_pin = 11,},
};

void setup()
{
  Serial.begin(115200);

  for (uint8_t i = 0; i < SEN_NUM; i++) {
    pinMode( sens[i].en_pin, OUTPUT );
    pinMode( sens[i].en_pin, HIGH );

    pinMode( sens[i].led_pin, OUTPUT );
    pinMode( sens[i].led_pin, HIGH );

    sens[i].check = 0;
    sens[i].time_cnt = 0;
  }

  Wire.begin();
  Wire.setClock(200000);

  _delay_ms(1000);

  for (uint8_t i = 0; i < SEN_NUM; i++) {
    pinMode( sens[i].en_pin, LOW );
    sens[i].sensor.setTimeout(50);

    if (!sens[i].sensor.init()) {
      Serial.print("Failed to detect and initialize sensor: ");
      Serial.print(i);
      Serial.println(" ");
      continue;
    }

    uint8_t addr = sens[i].sensor.getAddress();
    sens[i].sensor.setAddress(addr+i+SEN_NUM);
    sens[i].check = 1;

    Serial.print("addr:");
    Serial.print(addr);
    Serial.print(" -> ");
    Serial.println(addr+i+SEN_NUM);

    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    sens[i].sensor.startContinuous();
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
    if (sens[i].check != 1)
      continue;

    // Serial.print("Dis: ");
    uint16_t data = sens[i].sensor.readRangeContinuousMillimeters();
    // Serial.print("mm");
    if (sens[i].sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

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
      sens[i].time_cnt = millis();
      digitalWrite( sens[i].led_pin, LOW );
    } else {
      Serial.print("100 ");
    }

    unsigned long ms = millis();
    if (ms - sens[i].time_cnt > LED_KEEP) {
      digitalWrite( sens[i].led_pin, HIGH );
    }
  }

  Serial.println();

  _delay_ms(10);
}
