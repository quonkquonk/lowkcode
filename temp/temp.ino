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

enum menus {main, graph, settings, interval, customInterval, confirm, off};
int currentMenu = main;
int cursorPos = 0;
bool cycleButtonLastState = false;
bool enterButtonLastState = false;
bool backButtonLastState = false;
int lastMenu = customInterval;

unsigned long dataSaveFreq = 5000;
unsigned long selectedFreq = 0;
int customIntervalValue = 10; // in minutes

unsigned long nextSave = 0;

float plots[20];
int plotsRecorded = 0;

float h = 0;
float t = 0;


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
  randomSeed(analogRead(32));
 for (int i = 0; i < 20; i++)
 {
  plots[i] = -99;
 }
  
  

  
}

void loop() {

Serial.println(t);


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

    case interval:
      intervalLoop();
      break;

    case confirm:
      confirmLoop();
      break;

    case customInterval:
      customIntervalLoop();
      break;

    case off:
      offLoop();
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

  if (digitalRead(cycle) == 0 && currentMenu == customInterval)
  {
    customIntervalValue = constrain(++customIntervalValue, 1, 300);
    delay(50);
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
   if (digitalRead(back) == 0 && currentMenu == customInterval)
  {
    if (customIntervalValue != 1){
    customIntervalValue = constrain(customIntervalValue--, 1, 300);
    }
    else
    {
      customIntervalValue = 1;
    }
    delay(50);
  }


  if (currentMenu != lastMenu)
  {
    lastMenu = currentMenu;
    cursorPos = 0;

  }
  
 if (nextCheck < millis())
  {
  nextCheck = millis() + 2100;

  
  h = dht.readHumidity();
  
  t = dht.readTemperature();

  
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("C "));

  }

  if (nextSave < millis())
  {
    nextSave = millis() + dataSaveFreq;

    if (plotsRecorded < 20)
    {
      plots[plotsRecorded] = t;
      plotsRecorded++;
    }
    else
    {
      for (int i = 0; i < 20; i++)
      {
        if (i != 19)
        {
          plots[i] = plots[i+1];
        }
        else
        {
          plots[i] = t;
        }
      }
    }
  }

}

void tempLoop()
{
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
  display.println("35C");
  display.setCursor(0, 56);
  display.println("5C");
  

  for (int i = 0; i < plotsRecorded; i++)
  {
    int plotYPos = map(plots[i], 5, 35, 64, 0);
    int plotXPos = (i * 6) + 2;




    if (plots[i + 1] == -99 || i == 19)
    {
      display.fillCircle(plotXPos, plotYPos, 2, WHITE);
    }
    if (i != 0)
    {
      int lastPlotYPos = map(plots[i-1], 5, 35, 64, 0);
      int lastPlotXPos = ((i-1) * 6) + 2;

      display.drawLine(lastPlotXPos, lastPlotYPos, plotXPos, plotYPos, WHITE);
    }

  }

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
      display.println("5 sec");

      display.setCursor(0, 17);
      display.setTextColor(WHITE);
      display.println("10 min");

      display.setCursor(0, 34);
      display.setTextColor(WHITE);
      display.println("30 min");

      display.display();
      break;

    case 1:
      display.setTextSize(2);
      display.clearDisplay();

      display.fillRect(0, 17, 128, 16, WHITE);
      

      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.println("5 sec");

      display.setCursor(0, 17);
      display.setTextColor(BLACK);
      display.println("10 min");

      display.setCursor(0, 34);
      display.setTextColor(WHITE);
      display.println("30 min");

      display.display();
      break;

    case 2:
      display.setTextSize(2);
      display.clearDisplay();

      display.fillRect(0, 34, 128, 16, WHITE);
      

      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.println("5 sec");

      display.setCursor(0, 17);
      display.setTextColor(WHITE);
      display.println("10 min");

      display.setCursor(0, 34);
      display.setTextColor(BLACK);
      display.println("30 min");

      display.display();
      break;

    case 3:
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
      display.println("Custom");

      display.display();
      break;


}
}

void confirmLoop()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.println("Previous data will be cleared");
  display.setTextSize(2);
  display.println(" Continue?");
  

  switch (cursorPos)
  {
    case 0:
      display.setTextSize(2);
      

      display.fillRect(0, 48, 64, 16, WHITE);
      

      display.setCursor(0, 48);
      display.setTextColor(BLACK);
      display.println(" Yes");

      display.setCursor(86, 48);
      display.setTextColor(WHITE);
      display.println("No");

      display.display();
      break;

    case 1:
      display.setTextSize(2);
      

      display.fillRect(64, 48, 64, 16, WHITE);
      

      display.setCursor(0, 48);
      display.setTextColor(WHITE);
      display.println(" Yes");

      display.setCursor(86, 48);
      display.setTextColor(BLACK);
      display.println("No");

      display.display();
      break;
  }
}


void customIntervalLoop()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);


if (customIntervalValue < 60)
{
  display.print(customIntervalValue);
  display.println(" min");
}
else
{ 
  display.print((customIntervalValue - (customIntervalValue % 60)) / 60);
  display.print("h ");
  display.print(customIntervalValue % 60);
  display.println("min");
}
  int barPercent = map(customIntervalValue, 0, 300, 0, 128);
  display.fillRect(0, 50, barPercent, 20, WHITE);
  display.display();
  

  
}

void offLoop()
{
  display.clearDisplay();
  display.display();

  if (digitalRead(cycle) == 0 || digitalRead(back) == 0 || digitalRead(enter) == 0)
  {
    currentMenu = main;
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.println("loading...");
    display.display();
  }
}

void cyclePress()
{
  switch (currentMenu)
  {
    case settings:
      cursorPos = ++cursorPos % 3;
      break;

    case interval:
      cursorPos = ++cursorPos % 4;
      break;

      

    case confirm:
      cursorPos = ++cursorPos % 2;
      break;
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
          currentMenu = off;
          delay(200);
          break;
      }
      break;

    case interval:
      switch (cursorPos)
      {
        case 0:
          selectedFreq = 5000;
          currentMenu = confirm;
          break;

        case 1:
          selectedFreq = 600000;
          currentMenu = confirm;
          break;

        case 2:
          selectedFreq = 1800000;
          currentMenu = confirm;
          break;

        case 3:
          currentMenu = customInterval;
          
          break;
      }
      break;

      case confirm:
        
          switch (cursorPos)
          {
            case 0:
              dataSaveFreq = selectedFreq;
              currentMenu = settings;
              for (int i = 0; i < 20; i++)
              {
                plots[i] = -99;


              }
              plotsRecorded = 0;
              nextSave = 0;
              break;

            case 1:
              currentMenu = interval;
              break;

           
          }
          break;

      case customInterval:
        selectedFreq = customIntervalValue * 60000;
        currentMenu = confirm;
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

    case graph:
    case interval:

      currentMenu = settings;
      break;

    case confirm:
      currentMenu = interval;
      break;

    
  }
}
// lowk just checking if it worked lowk
// lowk lowk still testing lowk



