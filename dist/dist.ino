#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


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
  pinMode(16, OUTPUT);

}

void loop() {

digitalWrite(16, LOW);
delayMicroseconds(10);

digitalWrite(16, HIGH);
delayMicroseconds(2);
digitalWrite(16, LOW);


int distance = pulseIn(17, HIGH) * 0.0343 / 2;

if (distance != 0)
{
  Serial.println(distance);
}

display.clearDisplay();

display.setTextColor(WHITE);
display.setTextSize(4.2);
display.setCursor(0, 0);
display.println(String(distance) + "cm");
display.display();


  



}
