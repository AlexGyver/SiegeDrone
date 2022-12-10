#define RADIO_PIN 2
#define MOT_R_PIN_1 5
#define MOT_R_PIN_2 6
#define MOT_L_PIN_1 9
#define MOT_L_PIN_2 10

//#define G433_SPEED 1000   // скорость 100-10000 бит/с, по умолч. 2000 бит/с
#define G433_FAST
#include <Gyver433.h>
Gyver433_RX<RADIO_PIN, 4> rx;  // указали пин и размер буфера

#include <GyverMotor2.h>
GMotor2<DRIVER2WIRE_PWM> motorL(MOT_L_PIN_1, MOT_L_PIN_2);
GMotor2<DRIVER2WIRE_PWM> motorR(MOT_R_PIN_1, MOT_R_PIN_2);

void setup() {
  //Serial.begin(115200);
  motorR.setMinDuty(10);
  motorL.setMinDuty(10);
}

uint32_t tmr;
void loop() {
  if (tmr && millis() - tmr >= 300) {
    tmr = 0;
    motorL.stop();
    motorR.stop();
  }
  if (rx.tick()) {
    int data[2];
    if (rx.readData(data)) {
      tmr = millis();
      //Serial.println(String(data[0]) + "," + data[1]);
      motorL.setSpeed(data[0]);
      motorR.setSpeed(data[1]);
    }
  }
}
