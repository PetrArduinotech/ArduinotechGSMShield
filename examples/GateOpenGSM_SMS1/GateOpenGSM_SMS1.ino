#include <ArduinotechGSMShield.h>
#include <SoftwareSerial.h>
#define LED 7
#define RELE1 8

//Vytvoření instance třídy AGS

AGS modul(1);
uint8_t infoStatus;
String number;

//Vstupní parametry a inicializace shieldu

void setup() 
{
  pinMode(RELE1,OUTPUT);
  digitalWrite(RELE1,LOW);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
  modul.begin();
}

//Hlavní smyčka – čeká na příchod SMS s obsahem OPEN, 
//zkontroluje autorizaci - příchozí číslo
//Je-li shoda s tím, na co čeká, zapne relé, 
//dvě sekundy počká, vypne relé a prozvoní zpět
//prozvání 8sekund a pak hovor zavěsí. 
//Není-li shoda, nestane se nic

void loop() 
{
   infoStatus = modul.checkCallAndSMS();
   if (infoStatus == 2)
   {
      number = modul.getNumber();
      Serial.println("SMS from:" + number);
      modul.callEnd();
      
      if (modul.getSMSContent() == "OPEN") 
      {
        digitalWrite(RELE1,HIGH);
        digitalWrite(LED,LOW);
        delay(2000);
        digitalWrite(RELE1,LOW);
        digitalWrite(LED,HIGH);
        modul.makeCall(number);
        delay(8000);
        modul.callEnd();
      }
   }
   delay(1000);
}

