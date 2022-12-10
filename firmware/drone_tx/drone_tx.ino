//#define G433_SPEED 1000   // скорость 100-10000 бит/с, по умолч. 2000 бит/с
#define G433_FAST

#define RADIO_PIN 2
#define JOY_PIN_L 1
#define JOY_PIN_R 3

#include <Gyver433.h>
Gyver433_TX<RADIO_PIN> tx;  // указали пин

#include <GyverJoy.h>
GyverJoy jyL(JOY_PIN_L);
GyverJoy jyR(JOY_PIN_R);

void setup() {
  Serial.begin(115200);
  jyL.calibrate();
  jyR.calibrate();
  jyL.exponent(GJ_SQUARE);
  jyR.exponent(GJ_SQUARE);
  jyR.deadzone(5);
  jyL.deadzone(5);
}

void loop() {
  jyL.tick();
  jyR.tick();
  int data[2];
  data[0] = jyL.value();
  data[1] = jyR.value();
  tx.sendData(data);
  delay(30);
}
