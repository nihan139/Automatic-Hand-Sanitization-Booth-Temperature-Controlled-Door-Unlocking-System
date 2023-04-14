#include <Servo.h>
#include <stdio.h>
#include <string.h>

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Servo myServo1L;
Servo myServo1R;
Servo myServo2L;
Servo myServo2R;

#define Sensorpin1 22
#define Sensorpin2 24
#define Sensorpin3 8

#define Servopin1L 10
#define Servopin1R 9
#define Servopin2L 6
#define Servopin2R 7

int k=0;
int i;
int count=0;
float avg=0;
int avg_counter=0;
int initial=1;
float temp[5]={0,0,0,0,0};


void setup() {
  // put your setup code here, to run once:

  myServo1L.attach(Servopin1L);
  myServo1L.write(160);
  myServo1R.attach(Servopin1R);
  myServo1R.write(25);

  myServo2L.attach(Servopin2L);
  myServo2L.write(155);
  myServo2R.attach(Servopin2R);
  myServo2R.write(10);

  pinMode(Sensorpin1, INPUT);
  pinMode(Sensorpin2, INPUT);
  pinMode(Sensorpin3, INPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Welcome to BUET");
  Serial.begin(9600);

  Wire.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  bool value1 = digitalRead(Sensorpin1);
  bool value2 = digitalRead(Sensorpin2);
  bool value3 = digitalRead(Sensorpin3);
  Serial.print("IR Sensor-1 : ");
  Serial.println(value1);
  Serial.print("IR Sensor-2 : ");
  Serial.println(value2);
  Serial.print("IR Sensor-3 : ");
  Serial.println(value3);
  
  float temperature = printTemp('F');//object temperature in F
  //temperature = temperature - 35;

  if(value3==0){
    if(initial == 1){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Person Detected");
      lcd.setCursor(0, 1);
      lcd.print("Processing...");
      initial = 0;    
    }

    
    if(temperature >100){
      temp[avg_counter]=temperature;
      if(avg_counter >= 2){
        if(avg_counter == 2){
          for(i=0;i<3;i++){
          avg = avg + temp[avg_counter];
          }
          avg = avg/3;
          k=2;
          count=0;     
        }
          
        String myString = "Temp: ";
        myString.concat(avg);  
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(myString);
        lcd.setCursor(0, 1);
        lcd.print("Go to Door-2");
      }
      
      avg_counter = avg_counter + 1;
    }
    
    else if(temperature <100 && temperature >90) {
      temp[avg_counter]=temperature;
      if(avg_counter >= 2){
        if(avg_counter == 2){
          for(i=0;i<3;i++){
            avg = avg + temp[avg_counter];
          }
          avg = avg/3;
          k=1;
          count=0;         
        }


        String myString = "Temp: ";
        myString.concat(avg);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(myString);
        lcd.setCursor(0, 1);
        lcd.print("Go to Door-1");
      }
      
      avg_counter = avg_counter + 1;
    }

    }    

  else{
    if(k==1 | k==2){
      String myString = "Time Left: ";
      myString.concat(30-count);
      myString.concat(" sec");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(myString);    
    }
    if (count>30 && (k==1 | k==2)){
      k=0;
      count=0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time Up");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome to BUET");
      
    }
    if(k==0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome to BUET");
    }
    count = count + 1;
    avg=0;
    avg_counter=0;
  }
  
  if(value1==0 && k==1 && count<30){
    myServo1L.write(85);
    myServo1R.write(105);
    k=0;
    count=0;
    //initial =1;
    avg=0;
    avg_counter=0;
    delay(3000);
  }
  
  
  if(value2==0 && k==2 && count<30){
    myServo2L.write(85);
    myServo2R.write(110);
    k=0;
    count=0;
    avg=0;
    avg_counter=0;
    delay(3000);
  }
  
  else{
    myServo1L.write(175);
    myServo1R.write(15);
    myServo2L.write(155);
    myServo2R.write(10);
  }

  delay(1000);

}



float getTemp(char type)
{
   float value;
   float tempObjec = mlx.readObjectTempC();//in C object
   float tempAmbient = mlx.readAmbientTempC();
   if(type =='F')
   {
    value = mlx.readObjectTempF(); //Fah. Object
    value = value+3.5;
   }
   else if(type =='C')
   {
    value = tempObjec;
   }
   else if(type =='D')
   {
    value = tempAmbient;
   }
   return value;
}




float printTemp(char type)
{  
  float tmp = getTemp(type);
   
  if(type =='C')
  {
      Serial.println(tmp);
      Serial.println("C");
              
  }
  else if(type =='D')
  {
      Serial.println(tmp);
      Serial.println("D"); 
  }
  else if(type =='F')
  {
      Serial.println(tmp);
      Serial.println("F");
  }

  return tmp;

}
