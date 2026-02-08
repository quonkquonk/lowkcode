#include <ESP32Servo.h>
#include <IRremote.h>

Servo servo1;
Servo servo2;

unsigned long code = 0;



int servo1pos = 0;
int servo2pos = 0;
void setup() {
  Serial.begin(9600);
  servo1.setPeriodHertz(50);
  servo1.attach(26, 500, 2400);
  servo2.setPeriodHertz(50);
  servo2.attach(27, 500, 2400);

  IrReceiver.begin(17);
}

void loop() {
  servo1.write(map(analogRead(35), 0, 4095, 0, 180));

  servo2.write(map(analogRead(32), 0, 4095, 0, 180));

  
}
