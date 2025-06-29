
#include <Wire.h>
#include <VL53L0X.h>

#define SEN_NUM 5
#define LED_KEEP 200

struct dev_t {
  uint8_t en_pin;
  uint8_t led_pin;
  uint8_t check;
  unsigned long time_cnt;
  uint16_t low_pass;
  VL53L0X sensor;
} sens[SEN_NUM] = {
  {.en_pin =  2, .led_pin =  3,},
  {.en_pin =  4, .led_pin =  5,},
  {.en_pin =  6, .led_pin =  7,},
  {.en_pin =  8, .led_pin =  9,},
  {.en_pin = 10, .led_pin = 11,},
};

void(* resetFunc) (void) = 0;

void setup()
{
  Serial.begin(115200);

  for (uint8_t i = 0; i < SEN_NUM; i++) {
    pinMode( sens[i].en_pin, OUTPUT );
    digitalWrite( sens[i].en_pin, LOW );

    pinMode( sens[i].led_pin, OUTPUT );
    digitalWrite( sens[i].led_pin, HIGH );

    sens[i].check = 0;
    sens[i].time_cnt = 0;
  }

  Wire.begin();
  Wire.setClock(200000);

  _delay_ms(1000);

  for (uint8_t i = 0; i < SEN_NUM; i++) {
    digitalWrite( sens[i].en_pin, HIGH );
    sens[i].sensor.setTimeout(100);

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
  int min = 0, max = 1000;
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

    if (data == 65535) {
      Serial.println("\nRead sensor: " + String(i) + " error !!\n");
      _delay_ms(1000);
      resetFunc();
    }

    sens[i].low_pass = (0.85*sens[i].low_pass) + (0.15*data);
    uint16_t hp_data = abs((int32_t)data - (int32_t)sens[i].low_pass);

    // if (data > 250) {
    //   data = 250;
    // } else if (data < 100) {
    //   data = 100;
    // }

    // Serial.print("data: ");
    // Serial.print(data);
    // Serial.print(" ");
    Serial.print(String(data) + " " + String(sens[i].low_pass) + " " + String(hp_data) + " ");

    // Serial.print("flag: ");
    if (hp_data > 30) {
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
