#include <TinyGPS.h>

TinyGPS gps;
float co, no, pco, pno, debu, pdebu;
float flat, flon;
String indikco, indikno2, indikdebu;

int measurePin = 2;
int ledPower = 13;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

String str; 

void readSensor ()
{ 
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
  
  debu = analogRead (measurePin);
  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  
  co = analogRead (A0); // pembacaan port A0
  no = analogRead (A1); // pembacaan port A1

  
    //CO
     pco= (0.0035)*co-(0.291);
    //NO
     pno= (0.0001)*no-(0.0069);
    //dEBU
    pdebu = (0.0004)*debu+(0.0661); 
}
void setup(){
 Serial.begin(9600);
 Serial1.begin(115200);
 Serial2.begin(9600);
 pinMode(ledPower,OUTPUT);
}
void loop()
{ 
  readSensor(); 
  settingGPS();
  Serial.print("Lat: ");
  Serial.println(flat == TinyGPS::GPS_INVALID_F_ANGLE?0.0 : flat,6); 
  Serial.print(" Long: ");
  Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE?0.0 : flon,6); 
  Serial.print(" CO: ");
  Serial.print(pco); 
  Serial.println("ppm");
  Serial.print(" NO2: ");
  Serial.print(pno); 
  Serial.println("ppm");
  Serial.print(" Debu: ");
  Serial.print(pdebu);
  Serial.println("mg/m3"); 
  str =String('L')+String(flat == TinyGPS::GPS_INVALID_F_ANGLE?0.0 : flat,6)+String('O')+String(flon == TinyGPS::GPS_INVALID_F_ANGLE?0.0 : flon,6)+String('C')+String(pco)+String('N')+String(pno)
  +String('D')+String(pdebu);
  Serial1.println(str);
  delay(5000);
}

void settingGPS ()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial2.available())
    {
      char c = Serial2.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    gps.f_get_position(&flat, &flon);
  }
}
