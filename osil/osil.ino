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


int plots[SCREEN_W]; 
int value = 0;


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  Wire.begin();
  Serial.begin(9600);
  display.clearDisplay();
 
   
 for (int i = 0; i < SCREEN_W ; i++)
  {
    plots[i] = 5;  
  }
  
}

void loop() {

  for (int i = 0; i < SCREEN_W - 1; i++)
  {
    plots[i] = plots[i+1];  
  }
  int pot = map(analogRead(4), 0, 4095, 64, 0);
  plots[SCREEN_W - 1] = pot;

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  for (int i = 0; i < SCREEN_W ; i++)
  {
    display.setCursor(i, plots[i]);
    display.print(".");
  }

  display.display();

  // delay(10);
}
