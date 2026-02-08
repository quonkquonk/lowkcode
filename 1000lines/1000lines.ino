#include <LiquidCrystal.h>
#include <Servo.h>





LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

enum menuStates {main, pinMenu, piezoMenu, ledMenu, servoMenu, rangeMenu};
enum pinStates {none, led, servo, piezo};

String rows[] = {"", "", "", "", ""};

int pin0 = none;
int pin1 = none;
int pin2 = none;

int editingPin = 0;
int interactionCooldown = 200;


int rowsCount = 6;
unsigned long lastPressButton = 0;
unsigned long lastPressX = 0;
unsigned long lastPressY = 0;
unsigned long lastBeep = 0;
unsigned long lastSweep = 0;
bool whatdoievencallthis = false;

int cursor = 0;
int currentMenu = main;
int oldMenu;

byte fill[8] = {B11111, B11111,B11111,B11111,B11111,B11111,B11111,B11111};


class Piezo
{
  private:
  int pin;
  int freq = 300;
  bool beeping = false;
  
  public:
  
  Piezo(int m_pin)
  {
    pin = m_pin;
    pinMode(pin, OUTPUT);
  }
  
  void on()
  {
   tone(pin, freq);
   beeping = false;
  }
  
  void off()
  {
   noTone(pin);
   beeping = false;
  }
  
  void setFreq(int m_freq)
  {
   freq = m_freq;
   
   tone(pin, freq);
  } //67 67 67 67 67 67 67 67 67
  
  int freqLevel()
  {
   return freq; 
  }
  
  void toggleBeep()
  {
   beeping = !beeping; 
  }
  
  bool isBeeping()
  {
   return beeping; 
  }
  
  void beep()
  {
   tone(pin, freq, 500); 
  }
  
 
    
};

class Led
{
  private:
  int pin;
  int bright = 255;
  
  public:
  
  Led(int m_pin)
  {
    pin = m_pin;
    pinMode(pin, OUTPUT);
  }
  
  void on() 
  {
   
   analogWrite(pin, bright); 
    
  }
  
  void off()
  {
   digitalWrite(pin, LOW); 
  }
  
  void brightness(int value)
  {
   bright = value;
   analogWrite(pin, bright);
  }
  
  int brightLevel()
  {
   return bright; 
  }
  
  
  
  
  
};


Piezo piezo0(3);
Led led0(3);
Servo servo0;
bool servo0sweep = false;


Piezo piezo1(5);
Led led1(5);
Servo servo1;
bool servo1sweep = false;


Piezo piezo2(6);
Led led2(6);
Servo servo2;
bool servo2sweep = false;


void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(7, INPUT_PULLUP);
  changeScene();
  update();
  

  digitalWrite(0, LOW);
  lcd.createChar(0, fill);
  
  
}

void loop()
{

if (digitalRead(7) == LOW && lastPressButton < millis())
  {
    lastPressButton = millis() + 200;
    Serial.println(cursor);
    buttonPressed();
    changeScene();
    update();
  }
  
  if (analogRead(A0) > 800 && lastPressY < millis())
  {
    lastPressY = millis() + interactionCooldown;
    if (cursor != 0) {
    cursor = (cursor - 1) % rowsCount;
    }
    Serial.println(cursor);
    changeScene();
    update();
  }
  
  if (analogRead(A0) < 400 && lastPressY < millis())
  {
    lastPressY = millis() + interactionCooldown;
    cursor = (cursor + 1) % rowsCount;
    Serial.println(cursor);
    changeScene();
    update();
  }
  if (analogRead(A1) > 800 && lastPressX < millis())
  {
    lastPressX = millis() + interactionCooldown;
    forward();
    changeScene();
    update();
  }
  
  if (analogRead(A1) < 400 && lastPressX < millis())
  {
    lastPressX = millis() + interactionCooldown;
    back();
    changeScene();
    update();
    
  }
  
  if (currentMenu == rangeMenu)
  {
   interactionCooldown = 50; 
  }
  else
  {
    interactionCooldown = 200;
  }
  
  if (millis() > lastBeep)
  {
    lastBeep = millis() + 1000;
    if (piezo0.isBeeping())
    {
      piezo0.beep();
    }
    if (piezo1.isBeeping())
    {
      piezo1.beep();
    }
    if (piezo2.isBeeping())
    {
      piezo2.beep();
    }
  }
  

  if (millis() > lastSweep)
  {
   lastSweep = millis() + 1000;
   if (whatdoievencallthis)
   {
    whatdoievencallthis = false; 
    if (servo0sweep)
    {
      Serial.println("test");
      servo0.write(180); 
    }
     
     if (servo1sweep)
    {
      servo1.write(180); 
    }
     
    if (servo2sweep)
    {
      servo2.write(180); 
    }
   }
    else
    {
      whatdoievencallthis = true;
      
      if (servo0sweep)
    {
      servo0.write(0); 
    }
      if (servo1sweep)
    {
      servo1.write(0); 
    }
      if (servo2sweep)
    {
      servo2.write(0); 
    }
      
    }
  }
  
  
}


void buttonPressed()
{
  oldMenu = currentMenu;
	if (currentMenu == main)
    {
      switch (cursor)
      {
       case 0:
        if (pin0 == none){
        currentMenu = pinMenu;
        }
        else if (pin0 == led)
        {
          currentMenu = ledMenu;
        }
        else if (pin0 == piezo)
        {
         currentMenu = piezoMenu; 
        }
        else if (pin0 == servo)
        {
         currentMenu = servoMenu; 
        }
        editingPin = 0;
        break;
       case 1:
        if (pin1 == none){
        currentMenu = pinMenu;
        }
        else if (pin1 == led)
        {
          currentMenu = ledMenu;
        }
        else if (pin1 == piezo)
        {
         currentMenu = piezoMenu; 
        }
        else if (pin1 == servo)
        {
         currentMenu = servoMenu; 
        }
        
        editingPin = 1;
        break;
       case 2:
        if (pin2 == none){
        currentMenu = pinMenu;
        }
        else if (pin2 == led)
        {
          currentMenu = ledMenu;
        }
        else if (pin2 == piezo)
        {
         currentMenu = piezoMenu; 
        }
        else if (pin2 == servo)
        {
         currentMenu = servoMenu; 
        }
        
        editingPin = 2;
        break;
        
        
      }
    }
  else if (currentMenu == pinMenu)
  {
   switch (cursor)
   {
    case 0:
     switch (editingPin)
     {
      case 0: 
       pin0 = led;
       currentMenu = main;
       break;
      case 1:
       pin1 = led;
       currentMenu = main;
       break;
      case 2:
       pin2 = led;
       currentMenu = main;
       break;
     }
     break;
     
     
     case 1:
     	switch (editingPin)
     {
      case 0: 
       pin0 = piezo;
       currentMenu = main;
       break;
      case 1:
       pin1 = piezo;
       currentMenu = main;
       break;
      case 2:
       pin2 = piezo;
       currentMenu = main;
       break;
     }
     break;
    case 2:
     	switch (editingPin)
     {
      case 0: 
       pin0 = servo;
       servo0.attach(3);
       currentMenu = main;
       break;
      case 1:
       pin1 = servo;
       servo1.attach(5);
       currentMenu = main;
       break;
      case 2:
       pin2 = servo;
       servo2.attach(6);
       currentMenu = main;
       break;
     }
     break;
   }
  }
  else if (currentMenu == ledMenu)
  {
    switch (cursor)
    {
     case 0:
      switch (editingPin)
      {
       case 0:
        led0.on();
        break;
        
       case 1:
        led1.on();
        break;
        
       case 2:
        led2.on();
        break;
      }
      break;
     case 1:
      switch (editingPin)
      {
       case 0:
        led0.off();
        break;
        
       case 1:
        led1.off();
        break;
        
       case 2:
        led2.off();
        break;
      }
      break;
      
     case 2:
      currentMenu = rangeMenu;
      break;
      
     case 3:
      switch (editingPin)
      {
       case 0:
        led0.off();
        pin0 = none;
        break;
        
       case 1:
        led1.off();
        pin1 = none;
        break;
        
       case 2:
        led2.off();
        pin2 = none;
        break;
      }
      currentMenu = main;
      break;
    }
  }
  
  else if (currentMenu == piezoMenu)
  {
    switch (cursor)
    {
     case 0:
      switch (editingPin)
      {
       case 0:
        piezo0.on();
        break;
        
       case 1:
        piezo1.on();
        break;
        
       case 2:
        piezo2.on();
        break;
      }
      break;
     case 1:
      switch (editingPin)
      {
       case 0:
        piezo0.off();
        break;
        
       case 1:
        piezo1.off();
        break;
        
       case 2:
        piezo2.off();
        break;
      }
      break;
      
     case 2:
      currentMenu = rangeMenu;
      break;
      
     case 3:
      switch (editingPin)
      {
       case 0:
        piezo0.toggleBeep();
        break;
        
       case 1:
        piezo1.toggleBeep();
        break;
        
       case 2:
        piezo2.toggleBeep();
        break;
      }
      break;
      
     case 4:
      switch (editingPin)
      {
       case 0:
        piezo0.off();
        pin0 = none;
        break;
        
       case 1:
        piezo1.off();
        pin1 = none;
        break;
        
       case 2:
        piezo2.off();
        pin2 = none;
        break;
      }
      currentMenu = main;
      break;
    }
  }
  
  else if (currentMenu == servoMenu)
  {
    switch (cursor)
    {
     case 0:
      currentMenu = rangeMenu;
      break;
      
     case 1:
  		switch (editingPin)
        {
         case 0:
          if (servo0sweep)
          {
           servo0sweep = false; 
          }
          else
          {
           servo0sweep = true; 
          }
          break;
          
          case 1:
          if (servo1sweep)
          {
           servo1sweep = false; 
          }
          else
          {
           servo1sweep = true; 
          }
          break;
          
          case 2:
          if (servo2sweep)
          {
           servo2sweep = false; 
          }
          else
          {
           servo2sweep = true; 
          }
          break;
        }
      break;
      
      case 2:
      	switch (editingPin)
        {
         case 0:
          pin0 = none;
          servo0.detach();
          servo0sweep = false;
          currentMenu = main;
          break;
          
         case 1:
          pin1 = none;
          servo1.detach();
          servo1sweep = false;
          currentMenu = main;
          break;
          
         case 2:
          pin2 = none;
          servo2.detach();
          servo2sweep = false;
          currentMenu = main;
          break;
        }
      
    }
  }
  
  else if (currentMenu == rangeMenu)
  {
    switch (editingPin)
    {
     case 0:
      switch (pin0)
      {
       case led:
        currentMenu = ledMenu;
        break;
       case piezo:
        currentMenu = piezoMenu;
        break;
       case servo:
        currentMenu = servoMenu;
        break;
      }
      break;
      
     case 1:
      switch (pin1)
      {
       case led:
        currentMenu = ledMenu;
        break;
       case piezo:
        currentMenu = piezoMenu;
        break;
       case servo:
        currentMenu = servoMenu;
        break;
      }
      break;
      
      case 2:
      switch (pin2)
      {
       case led:
        currentMenu = ledMenu;
        break;
       case piezo:
        currentMenu = piezoMenu;
        break;
       case servo:
        currentMenu = servoMenu;
        break;
      }
      break;
    }
      
   
  }
  
  if (oldMenu != currentMenu)
  {
   cursor = 0; 
  }
}


void changeScene() 
{
  
  switch (currentMenu)
  {
   case main:
    rowsCount = 3;
    if (pin0 == none){
    rows[0] = "+pin 3";
    }
    else if (pin0 == led){
    rows[0] = "Led (3)";
    }
    else if (pin0 == piezo){
    rows[0] = "Piezo (3)";
    }
    else if (pin0 == servo){
    rows[0] = "Servo (3)";
    }
      
    if (pin1 == none){
    rows[1] = "+pin 5";
    }
    else if (pin1 == led){
    rows[1] = "Led (5)";
    }
    else if (pin1 == piezo){
    rows[1] = "Piezo (5)";
    }
    else if (pin1 == servo){
    rows[1] = "Servo (5)";
    }
    if (pin2 == none){
    rows[2] = "+pin 6";
    }
    else if (pin2 == led){
    rows[2] = "Led (6)";
    }
    else if (pin2 == piezo){
    rows[2] = "Piezo (6)";
    }
    else if (pin2 == servo){
    rows[2] = "Servo (6)";
    }
    rows[3] = "";
    rows[4] = "";
    break;
    
   case pinMenu:
    rowsCount = 3;
    rows[0] = "LED";
    rows[1] = "Piezo";
    rows[2] = "Servo";
    rows[3] = "";
    rows[4] = "";
    break;
    
    
   case ledMenu:
    rowsCount = 4;
    rows[0] = "On";
    rows[1] = "Off";
    rows[2] = "Brightness";
    rows[3] = "Remove";
    rows[4] = "";
    break;
    
   case piezoMenu:
    rowsCount = 5;
    rows[0] = "On";
    rows[1] = "Off";
    rows[2] = "Pitch Adjust";
    rows[3] = "Beep";
    rows[4] = "Remove";
    break;
    
   case servoMenu:
     rowsCount = 3;
    rows[0] = "Angle Adjust";
    rows[1] = "Sweep";
    rows[2] = "Remove";
    rows[3] = "";
    rows[4] = "";
    break;
   
   case rangeMenu:
    rowsCount = 1;
    switch (editingPin)
    {
     case 0:
      switch (pin0)
      {
       case led:
        rows[0] = led0.brightLevel();
        break;
       case piezo:
        rows[0] = piezo0.freqLevel();
        break;
       case servo:
        rows[0] = servo0.read();
        break;
        
      }
      break;
     case 1:
      switch (pin1)
      {
       case led:
        rows[0] = led1.brightLevel();
        break;
       case piezo:
        rows[0] = piezo1.freqLevel();
        break;
        
        case servo:
        rows[0] = servo1.read();
        break;
      }
      break;
     case 2:
      switch (pin2)
      {
       case led:
        rows[0] = led2.brightLevel();
        break;
       case piezo:
        rows[0] = piezo2.freqLevel();
        break;
       case servo:
        rows[0] = servo2.read();
        break;
      }
      break;
        
    }
      
    
    
    
  }
  
}

void forward()
{
  if (currentMenu == rangeMenu)
  {
    
    switch (editingPin)
    {
     case 0:
      switch (pin0)
      {
       case led:
       led0.brightness(constrain(led0.brightLevel() + 5, 0, 255));
       
       break;
       
       case piezo:
       piezo0.setFreq(constrain(piezo0.freqLevel() + 10, 0, 3000));
       break;
        
       case servo:
        servo0sweep = false;
        servo0.write(constrain(servo0.read() + 5, 0, 180)); 
        break;
      }
      break;
     case 1:
      switch (pin1)
      {
       case led:
       led1.brightness(constrain(led1.brightLevel() + 5, 0, 255));
       break;
       
       case piezo:
       piezo1.setFreq(constrain(piezo1.freqLevel() + 10, 0, 3000));
       break;
        
       case servo:
        servo1sweep = false;
        servo1.write(constrain(servo1.read() + 5, 0, 180)); 
        break;
      }
      break;
     case 2:
      switch (pin2)
      {
       case led:
       led2.brightness(constrain(led2.brightLevel() + 5, 0, 255));
       break;
       
       case piezo:
       piezo2.setFreq(constrain(piezo2.freqLevel() + 10, 0, 3000));
       break;
        
       case servo:
        servo2sweep = false;
        servo2.write(constrain(servo2.read() + 5, 0, 180)); 
        break;
      }
      break;
    }
  }
  
}
  

void back()
{
  if (currentMenu != rangeMenu)
  {
  	currentMenu = main;
  }
  else
  {
    switch (editingPin)
    {
     case 0:
      switch (pin0)
      {
       case led:
       led0.brightness(constrain(led0.brightLevel() - 5, 0, 255));
       break;
       
       case piezo:
       piezo0.setFreq(constrain(piezo0.freqLevel() - 10, 0, 3000));
       break;
        
       case servo:
        servo0sweep = false;
        servo0.write(constrain(servo0.read() - 5, 0, 180)); 
        break;
      }
      break;
     case 1:
      switch (pin1)
      {
       case led:
       led1.brightness(constrain(led1.brightLevel() - 5, 0, 255));
       break;
       
       case piezo:
       piezo1.setFreq(constrain(piezo1.freqLevel() - 10, 0, 3000));
       break;
        
       case servo:
        servo1sweep = false;
        servo1.write(constrain(servo1.read() - 5, 0, 180)); 
        break;
      }
      break;
     case 2:
      switch (pin2)
      {
       case led:
       led2.brightness(constrain(led2.brightLevel() - 5, 0, 255));
       break;
       
       case piezo:
       piezo2.setFreq(constrain(piezo2.freqLevel() - 10, 0, 3000));
       break;
        
       case servo:
        servo2sweep = false;
        servo2.write(constrain(servo2.read() - 5, 0, 180)); 
        break;
      }
      break;
    
    }
  }
  
}

void update()
{
  if (currentMenu != rangeMenu){
 switch (cursor)
  {
	case 0:
   		lcd.clear();
   		lcd.setCursor(0, 0);
    	lcd.print(rows[0]);
   		lcd.print(" <");
   		lcd.setCursor(0, 1);
   		lcd.print(rows[1]);
   		break;
    case 1:
   		lcd.clear();
   		lcd.setCursor(0, 0);
    	lcd.print(rows[0]);
   		lcd.setCursor(0, 1);
   		lcd.print(rows[1]);
   		lcd.print(" <");
   		break;
   case 2:
   		lcd.clear();
   		lcd.setCursor(0, 0);
    	lcd.print(rows[1]);
   		lcd.setCursor(0, 1);
   		lcd.print(rows[2]);
   		lcd.print(" <");
   		break;
   case 3:
   		lcd.clear();
   		lcd.setCursor(0, 0);
    	lcd.print(rows[2]);
   		lcd.setCursor(0, 1);
   		lcd.print(rows[3]);
        lcd.print(" <");
   		break;
   case 4:
   		lcd.clear();
   		lcd.setCursor(0, 0);
    	lcd.print(rows[3]);
   		lcd.setCursor(0, 1);
   		lcd.print(rows[4]);
        lcd.print(" <");
   		break;
   case 5:
   		lcd.clear();
   		lcd.setCursor(0, 0);
    	lcd.print(rows[4]);
   		lcd.setCursor(0, 1);
   		lcd.print(rows[5]);
   		lcd.print(" <");
   		break;
  }
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(rows[0]);
    lcd.setCursor(0, 1);
    
    int percent;
    
    switch(editingPin){
     case 0:
     switch(pin0){
      case led:
      percent = map(led0.brightLevel(), 0, 255, 0, 16);
      break;
      
      case piezo:
      percent = map(piezo0.freqLevel(), 0, 3000, 0, 16);
      break;
       
      case servo:
       percent = map(servo0.read(), 0, 180, 0, 16);
       break;
     }
      break;
     case 1:
      switch(pin1){
      case led:
      percent = map(led1.brightLevel(), 0, 255, 0, 16);
      break;
      
      case piezo:
      percent = map(piezo1.freqLevel(), 0, 3000, 0, 16);
      break;
        
      case servo:
       percent = map(servo1.read(), 0, 180, 0, 16);
       break;
     }
      break;
     case 2:
      switch(pin2){
      case led:
      percent = map(led2.brightLevel(), 0, 255, 0, 16);
      break;
      
      case piezo:
      percent = map(piezo2.freqLevel(), 0, 3000, 0, 16);
      break;
        
      case servo:
       percent = map(servo2.read(), 0, 180, 0, 16);
       break;
     }
      break;
    }
    for (int i = 0; i < percent; i++)
    {
      lcd.write(byte(0));
    }
    
  }
}