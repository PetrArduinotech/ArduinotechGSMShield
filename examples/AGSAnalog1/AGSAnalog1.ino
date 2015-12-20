#include <ArduinotechGSMShield.h>
#include <SoftwareSerial.h>
#define LED 7
#define SERVIS_NUMBER "739822476"

//Vytvoøení instance tøídy AGS

AGS modul(1);
uint8_t infoStatus;
String number;

//definice kombinací kontaktù
String status[8] =
{ 
	"K1=1,K2=1,K3=1",
	"K1=1,K2=1,K3=0",
	"K1=1,K2=0,K3=1",
	"K1=1,K2=0,K3=0",
	"K1=0,K2=1,K3=1",
	"K1=0,K2=1,K3=0",
	"K1=0,K2=0,K3=1",
	"K1=0,K2=0,K3=0" 
};
bool SMSsent;
String SMScontent;
String oldContent;
uint16_t analog;

//Vstupní parametry a inicializace shieldu

void setup()
{
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
	modul.begin();
	digitalWrite(LED, HIGH);
	oldContent = status[0];
}

/*Hlavní smyèka – mìøí napìtí na analogovém vstupu A0 
pøi jeho zmìnì vyhodnotí stav a odešle SMS.
Pak je hlídán pøíchod pøíchozího volání a na SERVIS_NUMBER,
je odeslána SMSka s aktuálním stavem kontaktù - analogového vstupu A0,
pokud se volající èíslo shoduje se SERVIS_NUMBER
*/


void loop()
{
	//mìø A0 a zjišuj kombinace, nachystej obsah SMS
	analog = analogRead(A0);
	if (analog < 340) SMScontent = status[0];
	if ((analog < 400) && (analog > 360)) SMScontent = status[1];
	if ((analog < 435) && (analog > 390)) SMScontent = status[2];
	if ((analog < 540) && (analog > 495)) SMScontent = status[3];
	if ((analog < 490) && (analog > 450)) SMScontent = status[4];
	if ((analog < 620) && (analog > 580)) SMScontent = status[5];
	if ((analog < 720) && (analog > 670)) SMScontent = status[6];
	if (analog > 1000) SMScontent = status[7];
	
	//porovnej obsah pøedešlé SMS a pokud je zmìna, ulo ji a odešli
	if (oldContent != SMScontent)
	{
		oldContent = SMScontent;
		modul.sendSMS(SERVIS_NUMBER, SMScontent);
	}

	
	infoStatus = modul.checkCallAndSMS();
	if (infoStatus == 1)
	{
		number = modul.getNumber();
		Serial.println("Call from:" + number);
		modul.callEnd();

		if (number == SERVIS_NUMBER)
		{
			modul.sendSMS(SERVIS_NUMBER, SMScontent);
		}
		else modul.callEnd();
	}
	delay(500);
}

