#define RGBred1 3
#define RGBgreen1 5
#define RGBblue1 6

#define RGBred2 9
#define RGBgreen2 10
#define RGBblue2 11

void setup() {

  pinMode(RGBred1,OUTPUT);
  pinMode(RGBgreen1,OUTPUT);
  pinMode(RGBblue1,OUTPUT);
  
  pinMode(RGBred2,OUTPUT);
  pinMode(RGBgreen2,OUTPUT);
  pinMode(RGBblue2,OUTPUT);

}
void loop() {
 
  //  Yeşil ışığı yaktık ve 5 saniye yakılı kaldı
  analogWrite(RGBred1,255);
  analogWrite(RGBgreen1,0);
  analogWrite(RGBblue1,255);
  analogWrite(RGBred2,255);
  analogWrite(RGBgreen2,255);
  analogWrite(RGBblue2,255);
  delay(5000);

  //  Kırmızı ışığı yaktık ve 3 saniye yakılı kaldı
  analogWrite(RGBred1,255);
  analogWrite(RGBgreen1,255);
  analogWrite(RGBblue1,255);
  analogWrite(RGBred2,0);
  analogWrite(RGBgreen2,255);
  analogWrite(RGBblue2,255);
  delay(5000);
}
