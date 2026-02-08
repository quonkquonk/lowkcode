#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_W (SCREEN_WIDTH - 11)

#define OLED_RESET -1   // No reset pin
#define OLED_ADDR  0x3C // Change if needed

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int poty = 0;
int potx = 0;
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  Wire.begin();
  Serial.begin(9600);

  display.clearDisplay();

  pinMode(5, INPUT_PULLUP);

}

void loop() {

  
  Serial.println((map(analogRead(4), 0, 4095, 0, 100) / 100) * 128);

if (digitalRead(5) == 0)
{
display.clearDisplay();
display.drawCircle(map(analogRead(4), 0, 4095, 100, 0) * 1.28, map(analogRead(15), 0, 4095, 0, 100) * 0.64, 5, WHITE);
display.display();
}
else
{
display.clearDisplay();
display.fillCircle(map(analogRead(4), 0, 4095, 100, 0) * 1.28, map(analogRead(15), 0, 4095, 0, 100) * 0.64, 5, WHITE);
display.display();
}
}
