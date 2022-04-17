//Name: Ahmed Kantoush
#include <avr\io.h>
#include <avr\interrupt.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,5,4,3,2);
void setup()
{
  Serial.begin(2000000);
  lcd.begin(16,2);
}

float x = 0;
char flag = 1;
float t1 = millis();
float max = 0;
char change = 0;
float freq = 0;

void loop() {
  for (int i = 0; i<3;i++) //Read 3 values and take avg
    x += analogRead(A0)*5/1023.0;
  x /= 3;
 
  if (x > max) // Calculate peak
    max = x;
    
  if ((x > (max - 0.5)) && (flag)) // If at peak
  {
    flag = 0;
    if (abs((1000/(millis() - t1)) - freq) > 0.05) //Tell if freq changed
      change = 1;
    freq = (1000/(millis() - t1)); //Calculate freq between 2 peaks
    t1 = millis();
  }
  
  if ((x<1.0) && (!flag))// If near min point
    flag = 1;
    
  if (change) //Output freq on lcd 
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Freq:");
    lcd.print(freq, 2);
    lcd.print("Hz");
    change = 0;
  }
  Serial.println(x);
  x = 0;
}
