/*
  EEE 307 Microprocessors, FALL 2020
  Experimental Project, Lane Assist to Sensetive Traffic Lights;

  Parts required:
  - one Arduino Mega 2560 (Car),
  - one Arduino Uno (Traffic Lights),
  - one Motor Driver Board (L298N),
  - four DC Motor (DC-3-6V),
  - one Infrared Sensor (MZ80),
  - two Tracking Module (YL-70),
  - five Tracking Sensor (YL-73),
  - one Color Sensor (TCS3200/GY-31),
  - one Buzzer,
  - one Robot Car Platform Kit,
  - four Wheel,
  - one Switch,
  - five Red Led,
  - one Yellow Led,
  - one RGB Led (Green Light),
  - one 100 ohm Resistance,
  - four 220 ohm Resistance,
  - one 9V Battery,
  - three 18650 Battery 2000 mAh,
  - one 3S BMS,

  created 13 Nov 2020, by Buğrahan İsmailoğlu & by Ömer Arslan
  modified 13 Nov 2020 to 18 Nov 2020 Nov 2020, by Buğrahan İsmailoğlu & by Ömer Arslan

  Improvements can be made...
*/
#include <LiquidCrystal.h>

LiquidCrystal lcd(9, 10, 11, 12, 13, 34);

// We define the pins that we connect the lane sensors placed on the left side of the car to Arduino. (YL-70)
// Sensor falling on top of the line.
#define SensorLeftOutward 48
#define SensorLeftInward 36

// We define the pins that we connect the lane sensors placed on the right side of the car to Arduino. (YL-70)
// Sensor falling on top of the line.
#define SensorRightOutward 52
#define SensorRightInward 44

// We define the pins that we connect the lane sensors placed on the front side of the car to Arduino. (YL-70)
// Sensor falling on top of the line.
#define SensonFront 40

// We define the pins of the motors that will control the right wheels that we connect to the Arduino. (L298N)
// By connecting the motors on the right, we ensure that they move synchronously.
#define RightMotorForward 4
#define RightMotorBackward 8
#define RightMotorEnable 3

// We define the pins of the motors that will control the left wheels that we connect to the Arduino. (L298N)
// By connecting the motors on the left, we ensure that they move synchronously.
#define LeftMotorForward 6
#define LeftMotorBackward 7
#define LeftMotorEnable 5

// We define the pin that we connect the sensor at the front of the car to the Arduino. (TCS3200)
#define ColorSensorS0 22      
#define ColorSensorS1 24
#define ColorSensorS2 26
#define ColorSensorS3 28
#define ColorSensorOut 30

// We define variables for primary colors. (TCS3200)
int RedFrequency, GreenFrequency, BlueFrequency = 0;

// We define the pin that we connect the sensor on the top of the car to the Arduino. (MZ80)
#define InfraredData 53

// We define a variable in order to be able to read the data from the sensor. (MZ80)
int InfraredValue = 0;

// We define the LEDs that we want to be lit in different numbers in different situations in the back of the car.
#define LEDLeftCorner 46
#define LEDRightCorner 48

// We define the buzzer pin for the car to ring in an emergency. (YL-44)
#define BuzzerHorn 2

void setup() {
  lcd.begin(16, 2);
  
  // We are initiating serial communication at 9600 bps because to see results in experimenting with colors.
  Serial.begin(9600);
  
  // We specify the input information of the lane sensors.
  pinMode(SensorLeftInward, INPUT);
  pinMode(SensorRightInward, INPUT);
  pinMode(SensonFront, INPUT);

  // We specify the output information of the motors.
  pinMode(RightMotorForward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  pinMode(RightMotorEnable, OUTPUT);
  
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(LeftMotorEnable, OUTPUT);
  
  // We specify the input-output information of the color sensor.
  pinMode(ColorSensorS0, OUTPUT);
  pinMode(ColorSensorS1, OUTPUT);
  pinMode(ColorSensorS2, OUTPUT);
  pinMode(ColorSensorS3, OUTPUT);
  pinMode(ColorSensorOut, INPUT);

  // We need to set the frequency value to 20% in order to use it on Arduino.
  digitalWrite(ColorSensorS0, HIGH);
  digitalWrite(ColorSensorS1, LOW);

  // We specify the input information of the infrared sensor.
  pinMode(InfraredData, INPUT);

  // We determine the input information of the LEDs.
  pinMode(LEDLeftCorner, INPUT);
  pinMode(LEDRightCorner, INPUT);

  // We determine the input information of the buzzer.
  pinMode(BuzzerHorn, OUTPUT);
  
}

void loop() {
  silencethehorn();

  // YAYA YOLA CIKIYO MU CIKMIYOR MU KONTROL EDİYOR. VE SONRASINDA KOMUTLARI YAPIYOR GENELLLL
  describepedestrian();
}

void describepedestrian(){

  // SENSORDEN DEĞER ALIYOR.
  InfraredValue = digitalRead(InfraredData);

  // EĞER YAYA YOLDAYSA.
  if (InfraredValue == 0){
    
    lcd.setCursor(0, 0);
    lcd.print("There is a");
    lcd.setCursor(0, 1);
    lcd.print("pedestrian.");

    // ARABAYI DURDUR.
    stopspeed();

    // ARKADAKİ DÖRT LEDİ DE YAK. UYARI AMACLI
    digitalWrite(LEDLeftCorner, HIGH);
    digitalWrite(LEDRightCorner, HIGH);

    // İSTERSEK KORNA CALIP UYARI SESİ CIKARTABİLİRİZ
    soundthehorn();

    // EĞER YAYA YOLDA DEĞİLSE.
    lcd.clear();
  }

  else if (InfraredValue == 1){

      // ÖNCELİKLE KORNAYI SUSTURUYORUZ.
      silencethehorn();

      digitalWrite(LEDLeftCorner, LOW);
      digitalWrite(LEDRightCorner, LOW);
      
      // Orta sensör şeridi okursa arabayı çalıştır.
      if(digitalRead(SensonFront) == 0){
        digitalWrite(RightMotorForward, HIGH);
        digitalWrite(RightMotorBackward, LOW);

        digitalWrite(LeftMotorForward, HIGH);
        digitalWrite(LeftMotorBackward, LOW);

        analogWrite(RightMotorEnable, 70);
        analogWrite(LeftMotorEnable, 70);

        moveaccordingtocolors();
      }

      // Sol ve Sağ aynı anda beyazı okursa araba devam etsin
      if (digitalRead(SensorLeftInward) == 0 or digitalRead(SensorRightInward) == 0) {
        
        if (digitalRead(SensorLeftInward) == 0) {
          
          lcd.setCursor(0, 0);
          lcd.print("The car");
          lcd.setCursor(0, 1);
          lcd.print("turns right.");
          
          turnright();
          moveaccordingtocolors();
      
          // sola dönüş işlemini yapıp gücü kesip normale dönmesini beklicez
          if (digitalRead(SensorLeftInward) == 1){
            stopspeed();
          }
        delay(10);
        }
          lcd.clear();
     
        if (digitalRead(SensorRightInward) == 0) {
          
          lcd.setCursor(0, 0);
          lcd.print("The car");
          lcd.setCursor(0, 1);
          lcd.print("turns left.");
         
          turnleft();
          moveaccordingtocolors();
        }
          // sola dönüş işlemini yapıp gücü kesip normale dönmesini beklicez
          if (digitalRead(SensorRightInward) == 1){
            stopspeed();
          }
        }
        lcd.clear();
        // RENKLERİ ALGILAMASI İÇİN KALİBRE ETMESİ GEREKİYOR O YÜZDEN KALİBRE FONK CAGIRIYORUZ.
        //moveaccordingtocolors();
        
  }
  stopspeed();
}


void soundthehorn() {
  tone(BuzzerHorn, 494);
}

void silencethehorn() {
  noTone(BuzzerHorn);
}

void calibrating() {

  // We calibrate the red color.
  digitalWrite(ColorSensorS2, LOW);
  digitalWrite(ColorSensorS3, LOW);

  // Read the output RedFrequency.
  RedFrequency = pulseIn(ColorSensorOut, LOW);

  // Printing red color value.
  Serial.print("Red= ");
  Serial.print(RedFrequency);
  Serial.print("  -  ");
  delay(200);

  // We calibrate the green color.
  digitalWrite(ColorSensorS2, HIGH);
  digitalWrite(ColorSensorS3, HIGH);

  // Read the output GreenFrequency.
  GreenFrequency = pulseIn(ColorSensorOut, LOW);

  // Printing green color value.
  Serial.print("Green= ");
  Serial.print(GreenFrequency);
  Serial.print("  -  ");
  delay(200);

  // We calibrate the blue color.
  digitalWrite(ColorSensorS2, LOW);
  digitalWrite(ColorSensorS3, HIGH);

  // Read the output BlueFrequency.
  BlueFrequency = pulseIn(ColorSensorOut, LOW);

  // Printing blue color value.
  Serial.print("Blue= ");
  Serial.println(BlueFrequency);
  delay(200);

}

void calibrated(){

  // We calibrate the red color.
  digitalWrite(ColorSensorS2, LOW);
  digitalWrite(ColorSensorS3, LOW);

  // Read the output RedFrequency.
  RedFrequency = pulseIn(ColorSensorOut, LOW);

  // We calibrate the green color.
  digitalWrite(ColorSensorS2, HIGH);
  digitalWrite(ColorSensorS3, HIGH);

  // Read the output GreenFrequency.
  GreenFrequency = pulseIn(ColorSensorOut, LOW);

  // We calibrate the blue color.
  digitalWrite(ColorSensorS2, LOW);
  digitalWrite(ColorSensorS3, HIGH);

  // Read the output BlueFrequency.
  BlueFrequency = pulseIn(ColorSensorOut, LOW);

}

void moveaccordingtocolors() {

  calibrated();
   
  if (RedFrequency > BlueFrequency and RedFrequency < GreenFrequency and GreenFrequency > BlueFrequency) {
    if (RedFrequency < 400 and 0 < RedFrequency){
      
      lcd.setCursor(0, 0);
      lcd.print("Red light");
      lcd.setCursor(0, 1);
      lcd.print("!!!!!!STOP!!!!!!");
      
      stopspeed();

      digitalWrite(LEDLeftCorner, HIGH);
      digitalWrite(LEDRightCorner, HIGH);
      
      delay(100);
    }
    lcd.clear();
  }
  if (GreenFrequency < RedFrequency and GreenFrequency > BlueFrequency and BlueFrequency < RedFrequency) {
    if (GreenFrequency < 400 and 0 < GreenFrequency){    
      dontchangespeed();

      lcd.setCursor(0, 0);
      lcd.print("Green light");
      lcd.setCursor(0, 1);
      lcd.print("!!!!!GO ON!!!!!");

      digitalWrite(LEDLeftCorner, LOW);
      digitalWrite(LEDRightCorner, LOW);
    }
    lcd.clear();
  }
}

void stopspeed(){
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);

  analogWrite(RightMotorEnable, 0);
  analogWrite(LeftMotorEnable, 0);
}

void dontchangespeed(){
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);

  analogWrite(RightMotorEnable, 70);
  analogWrite(LeftMotorEnable, 70);
  
}

void turnright(){
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);

  analogWrite(RightMotorEnable, 0);
  analogWrite(LeftMotorEnable, 70); 
}

void turnleft(){
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);

  analogWrite(RightMotorEnable, 70);
  analogWrite(LeftMotorEnable, 0); 
}
