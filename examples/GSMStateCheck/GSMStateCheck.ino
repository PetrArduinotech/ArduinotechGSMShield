#include <ArduinotechGSMShield.h>
#include <SoftwareSerial.h>
#define LED 7
#define IN1 5
#define IN2 6
#define RELE1 8
#define RELE2 9
#define SERVIS_NUMBER "739822476"


//Vytvoøení instance tøídy AGS

AGS modul(1);
uint8_t infoStatus;
String number;
bool IN1Sent, IN2Sent;


//Vstupní parametry a inicializace shieldu

void setup()
{
	pinMode(IN1, INPUT);
	pinMode(IN2, INPUT);
	pinMode(RELE1, OUTPUT);
	pinMode(RELE2, OUTPUT);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
	modul.begin();
	digitalWrite(LED, HIGH);

	IN1Sent = false;
	IN2Sent = false;
}



void loop()
{
	//klidovy stav = 1, sepnutte rele na vstupu = 0
	//pokud detekuje na vstupu 1 sepnuti kontaktu
	//odesli SMS pouze jednou- obe podminky musi platit zaroven
	if ((digitalRead(IN1) == 0) && (IN1Sent == false))
	{
		modul.sendSMS(SERVIS_NUMBER, "*** Stroj 14 JEDE ***");
		delay(300);
		digitalWrite(RELE1, HIGH);
		delay(300);
		digitalWrite(RELE1, LOW);
		delay(300);
		digitalWrite(RELE1, HIGH);
		delay(300);
		digitalWrite(RELE1, LOW);
		delay(1000);
		IN1Sent = true;
		return;
	}
	//totez na vstupu 2
	if ((digitalRead(IN2) == 0) && (IN2Sent == false))
	{
		modul.sendSMS(SERVIS_NUMBER, "*** Stroj 14 STOJI ***");
		IN2Sent = true;
	}

	//klidovy stav - priprav promenne na odeslani SMS pri zmene stavu
	if (digitalRead(IN1) == 1) IN1Sent = false;
	if (digitalRead(IN2) == 1) IN2Sent = false;
	
	//kontroluj co 0,5s
	delay(500);
}




