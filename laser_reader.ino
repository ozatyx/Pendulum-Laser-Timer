#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);



//Constants
const int pResistor = A0; // Photoresistor at Arduino analog pin A0

//Variables
int value;          //store value from photoresistor (0-1023)
int startTime;      
int stopTime;
int finalTime;
int safety;         //safety value so that it doesn't deal in absolute values
int t1;
int t2;
int max;            //should be the value when photoresistor is receiving max light
float total;
float totalSec;




void setup(){
 pinMode(pResistor, INPUT);
 pinMode(13, OUTPUT);
 digitalWrite(13, HIGH);
 pinMode(2, OUTPUT);
 digitalWrite(2, HIGH);

 Serial.begin(9600);
 safety = 20;           //have to fiddle around with to trigger loops correctly. ambient light messes with things
 max = 900;             //^^
 delay(1000);
 lcd.init();
 lcd.clear();
 lcd.backlight();
}

void is_blocked(){
  value = 999;
  lcd.clear();
  lcd.print("please block lzr");
  lcd.setCursor(0,1);
  lcd.print("with oscillator");
   while(value > max - safety){           //stay in loop until object blocks laser
    value = analogRead(pResistor);

  }
}

void laser_unblocked(){
  value = analogRead(pResistor);
  lcd.clear();
  lcd.print("laser unblocked!");

  while(value > max - safety){           //stay in loop until object blocks laser
    value = analogRead(pResistor);

  }

}

void laser_blocked(){
  lcd.clear();
  lcd.print("laser blocked!");
  while(value < max - safety){       //stay in loop until laser is seen again
    value = analogRead(pResistor);

  }

}

int laser_time(){
  startTime = millis();

  laser_unblocked(); //waits until pendulum crosses laser, should have travelled 1/2 of its period when it does

  stopTime = millis();
  finalTime = stopTime - startTime;
  
  lcd.clear();
  lcd.print("stopped timing!");
  return finalTime;
}

void loop(){
  lcd.clear();
  lcd.print("starting...");
  delay(2000);

  is_blocked(); //starting step 1; pendulum should be blocking laser then lifted once text disappears
  laser_blocked(); //starting step 2; pendulum should be at an extreme point, ready to swing. starts timing once it crosses the laser again.

  lcd.clear();
  lcd.print("timing!");

  t1 = laser_time(); //starts timing right after the pendulum leaves the laser, stops when it comes back, travelling 1/2 of its period.

  lcd.clear();
  lcd.print("your time:");
  total = t1*2 //1/2 period * 2 = full period!
  totalSec = total / 1000;
  
  lcd.clear();
  lcd.print("your period:");
  lcd.setCursor(0, 1);
  lcd.print(totalSec);

  delay(5000); //run it back

}
