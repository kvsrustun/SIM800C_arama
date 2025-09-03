#include <SoftwareSerial.h>

SoftwareSerial mySerial(D8, D14);

void setup(){
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); 
  delay(1000);
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("ATD+90xxxxxxxxxx;"); 
  delay(1000);

}

void loop(){
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());
  }
}
