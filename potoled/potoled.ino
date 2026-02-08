#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1   // No reset pin
#define OLED_ADDR  0x3C // Change if needed

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);






void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  Wire.begin();
  Serial.begin(9600);
  display.clearDisplay();
 


  
}

void loop() {
  /*
  if (Serial.available())
  {
    String message = Serial.readString();

    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    display.println(message);
    display.display();
    Serial.println("printed");

  }
  
*/
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println(analogRead(4));
  display.setTextSize(1);
  //display.setCursor(0, 1);
  for (int i = 0; i < map(analogRead(4), 0, 4095, 0, 20); i++)
  {
    display.print("=");
  }
  display.println("");
  for (int i = 0; i < map(analogRead(4), 0, 4095, 0, 20); i++)
  {
    display.print("=");
  }
  display.println("");
  for (int i = 0; i < map(analogRead(4), 0, 4095, 0, 20); i++)
  {
    display.print("=");
  }
  display.display();
  
  Serial.println(analogRead(4));
  //4095

  
}
