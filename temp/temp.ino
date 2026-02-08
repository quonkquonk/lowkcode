#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1   // No reset pin
#define OLED_ADDR  0x3C // Change if needed

unsigned long nextCheck = 0;


#define back 12
#define enter 14
#define cycle 13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(25, DHT11);

enum menus {main, graph, settings, interval, customInterval};
int currentMenu = main;
int cursorPos = 0;
bool cycleButtonLastState = false;
bool enterButtonLastState = false;
bool backButtonLastState = false;
int lastMenu = graph;




void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  Wire.begin();
  Serial.begin(9600);
  display.clearDisplay();
  display.setTextSize(1.6);
  display.setTextColor(WHITE);
  display.setCursor(20, 20);
  display.invertDisplay(false);
  

  dht.begin();

  pinMode(cycle, INPUT_PULLUP);
  pinMode(back, INPUT_PULLUP);
  pinMode(enter, INPUT_PULLUP);

 


  
}

void loop() {




  switch (currentMenu)
  {
    case main:
      tempLoop();
      break;

    case settings:
      settingsLoop();
      break;

    case graph:
      graphLoop();
      break;

  }

  if (cycleButtonLastState != digitalRead(cycle))
  {
    if (cycleButtonLastState == true)
    {
      cyclePress();
    }

    cycleButtonLastState = digitalRead(cycle);
    
  }

  if (enterButtonLastState != digitalRead(enter))
  {
    if (enterButtonLastState == true)
    {
      enterPress();
    }

    enterButtonLastState = digitalRead(enter);
    
  }

  if (backButtonLastState != digitalRead(back))
  {
    if (backButtonLastState == true)
    {
      backPress();
    }

    backButtonLastState = digitalRead(back);
    
  }
  


  if (currentMenu != lastMenu)
  {
    lastMenu = currentMenu;
    cursorPos = 0;

  }
  
 

}

void tempLoop()
{


  if (nextCheck < millis() && currentMenu == main)
  {
  nextCheck = millis() + 2100;

  
  float h = dht.readHumidity();
  
  float t = dht.readTemperature();

   if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("DHT11 disconnected");
    display.display();

    return;
  }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("C "));

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Temp:");
  display.println(String(t) + "C");

  display.println("Humidity: ");
  display.println(String(h) + "%");
  display.display();
  }
}

void settingsLoop()
{
  switch (cursorPos)
  {
    case 0:
      display.setTextSize(2);
      display.clearDisplay();

      display.fillRect(0, 0, 128, 16, WHITE);
      

      display.setCursor(0, 0);
      display.setTextColor(BLACK);
      display.println("Graph");

      display.setCursor(0, 17);
      display.setTextColor(WHITE);
      display.println("Intervals");

      display.setCursor(0, 34);
      display.setTextColor(WHITE);
      display.println("Toggle Scr");

      display.display();
      break;

    case 1:
      display.setTextSize(2);
      display.clearDisplay();

      display.fillRect(0, 17, 128, 16, WHITE);
      

      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.println("Graph");

      display.setCursor(0, 17);
      display.setTextColor(BLACK);
      display.println("Intervals");

      display.setCursor(0, 34);
      display.setTextColor(WHITE);
      display.println("Toggle Scr");

      display.display();
      break;

    case 2:
      display.setTextSize(2);
      display.clearDisplay();

      display.fillRect(0, 34, 128, 16, WHITE);
      

      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.println("Graph");

      display.setCursor(0, 17);
      display.setTextColor(WHITE);
      display.println("Intervals");

      display.setCursor(0, 34);
      display.setTextColor(BLACK);
      display.println("Toggle Scr");

      display.display();
      break;

}
}

void graphLoop()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("[graph here]");
  display.display();
}

void intervalLoop()
{
  switch (cursorPos)
  {
    case 0:
      display.setTextSize(2);
      display.clearDisplay();

      display.fillRect(0, 0, 128, 16, WHITE);
      

      display.setCursor(0, 0);
      display.setTextColor(BLACK);
      display.println("10 min");

      display.setCursor(0, 17);
      display.setTextColor(WHITE);
      display.println("30 min");

      display.setCursor(0, 34);
      display.setTextColor(WHITE);
      display.println("1 hour");

      display.display();
      break;

    case 1:
      display.setTextSize(2);
      display.clearDisplay();

      display.fillRect(0, 17, 128, 16, WHITE);
      

      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.println("10 min");

      display.setCursor(0, 17);
      display.setTextColor(BLACK);
      display.println("30 min");

      display.setCursor(0, 34);
      display.setTextColor(WHITE);
      display.println("1 hour");

      display.display();
      break;

    case 2:
      display.setTextSize(2);
      display.clearDisplay();

      display.fillRect(0, 34, 128, 16, WHITE);
      

      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.println("10 min");

      display.setCursor(0, 17);
      display.setTextColor(WHITE);
      display.println("30 min");

      display.setCursor(0, 34);
      display.setTextColor(BLACK);
      display.println("1 hour");

      display.display();
      break;

    case 3:
      display.setTextSize(2);
      display.clearDisplay();

      display.fillRect(0, 34, 128, 16, WHITE);
      

      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.println("30 min");

      display.setCursor(0, 17);
      display.setTextColor(WHITE);
      display.println("1 hour");

      display.setCursor(0, 34);
      display.setTextColor(BLACK);
      display.println("Custom");

      display.display();
      break;


}
}

void cyclePress()
{
  switch (currentMenu)
  {
    case settings:
      cursorPos = (cursorPos + 1) % 3;
      break;

    case interval:
      cursorPos = (cursorPos + 1) % 4
  }
}

void enterPress()
{
  switch (currentMenu)
  {
    case main:
      currentMenu = settings;
      break;

    case settings:
      switch (cursorPos)
      {
        case 0:
          currentMenu = graph;
          break;

        case 1:
          currentMenu = interval;
          break;

        case 2:
          Serial.println("ill lowk add this later");
          break;
      }
      break;
  }
}

void backPress()
{
  switch (currentMenu)
  {
    case settings:
      currentMenu = main;
      

      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.println("loading...");
      display.display();
      break;
  }
}
// lowk just checking if it worked lowk
// lowk lowk still testing lowk



