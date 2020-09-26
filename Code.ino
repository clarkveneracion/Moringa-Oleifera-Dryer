#include<avr/io.h>
#include<util/delay.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,20,4);
bool done = false;
bool wet = false;
/*Variables*/
int dryWeight = 0;
int wetWeight = 0;
float Load = 0;
float moistureWeight = 0;
float moistureContent = 0;
float Temp = 0;
float val = 0;
float x;
void RedWeight1();
void RedWeight2();
/*Variables*/
void setup() {
  pinMode(4,INPUT);
  pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(11,OUTPUT);  
  pinMode(12, OUTPUT);
  pinMode(1, OUTPUT);
  lcd.begin();
  lcd.backlight();
}

void loop() {
  //Prompt
  while(digitalRead(6) == 0)
  {
    digitalWrite(1,LOW);
    while(!wet && digitalRead(6) == 0)
    {
      digitalWrite(1,LOW);
      lcd.setCursor(0,0);
      lcd.print("Record wet      ");
      lcd.setCursor(0,1);
      lcd.print("malunggay:      ");
      
      //Load = map(analogRead(A1), 61, 961, 0, 1000); 
      Load = map(analogRead(A1), 341, 1023, 0, 1010); 
      //getWeight();
      
      lcd.setCursor(11,1);
      lcd.print(Load);//931 - 1068
      while(Load > 1000  && digitalRead(6) == 0)
      {
        lcd.setCursor(0,1);
        lcd.print("malunggay:      ");
        digitalWrite(1,HIGH);
        
        Load = map(analogRead(A1), 341, 1023, 0, 1010); 
        //getWeight();
        
        lcd.setCursor(11,1);
        lcd.print(Load);
        lcd.setCursor(0,0);
        lcd.setCursor(0,0);
        lcd.print("Too Heavy       ");
      }
      while(Load < 50  && digitalRead(6) == 0)
      {
        lcd.setCursor(0,1);
        lcd.print("malunggay:      ");
        digitalWrite(1,HIGH);
        
        Load = map(analogRead(A1), 341, 1023, 0, 1010); 
        //getWeight();
        
        lcd.setCursor(11,1);
        lcd.print(Load);
        lcd.setCursor(0,0);
        lcd.setCursor(0,0);
        lcd.print("Too light       ");
      }
      while(digitalRead(5) == 1  && digitalRead(6) == 0)
      {
        wet = true;
        wetWeight = Load;
      }
    }
    
    //process
    if(digitalRead(6) == 0)
    {
      val = analogRead(A0);
      Temp = ((val/1024)*5000)/10;
      
      //Load = map(analogRead(A1), 341, 1023, 0, 1010);
      
      
      if(Temp > 60.5)
        digitalWrite(11,LOW);
      else
      {
        if(moistureContent < 6)
          RedWeight2();
        else if(moistureContent > 6)
          RedWeight1();
        digitalWrite(11,HIGH);
      }
  
      lcd.setCursor(0,0);
      //lcd.print(moistureContent);
      lcd.print("Temperature:");
      lcd.setCursor(12,0);
      lcd.print(Temp);
      lcd.setCursor(0,1);
      lcd.print("Weight:         ");
      lcd.setCursor(8,1);
      lcd.print(Load);
  
      //Computation Wi = wetWeight, Ws = Load
      moistureWeight = wetWeight - Load;
      x = moistureWeight * 100; //26%
      moistureContent = x/Load;
      while(moistureContent >= 8.3 && moistureContent <= 8.6 && digitalRead(6) == 0)
      {
        digitalWrite(11,LOW);
        digitalWrite(1 ,HIGH);
        lcd.setCursor(0,0);
        lcd.print("Done             ");
        lcd.setCursor(0,1);
        lcd.print("Weight:         ");
        lcd.setCursor(8,1);
        lcd.print(Load);
        wet = false;
        if(digitalRead(6) == 1)
          break;
      } 
    }
         
  }
  lcd.setCursor(0,0);
  lcd.print("Stopped          ");
  lcd.setCursor(0,1);
  lcd.print("                 ");
  wet = false;
  delay(500); 
}

void RedWeight1()
{
  Load = Load-0.01;
}
void RedWeight2()
{
  Load--;
  delay(200);
}
