#include <BluetoothSerial.h>

String msg;

BluetoothSerial bt;
void setup() {
  bt.begin("wassup");
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
}

void loop() {
  if (bt.available())
  {
    msg = bt.read();
    msg.trim();
    Serial.println(msg);
  }

  if (msg == "49")
  {
    digitalWrite(5, HIGH);
  }
  if (msg == "48")
  {
    digitalWrite(5, LOW);
  }
}
