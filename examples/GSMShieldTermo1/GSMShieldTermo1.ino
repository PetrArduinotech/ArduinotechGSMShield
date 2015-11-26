#include <ArduinotechGSMShield.h>
#include <SoftwareSerial.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#define LED 7
#define RELE1 8
#define SERVICE_NUMBER "739822476"

//definice OneWire pro pøipojení DS18B20
#define ONE_WIRE_BUS 10
#define TEMPERATURE_PRECISION 9 

//definice pro teplomìr
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
float teplota;
String termIN;

//Vytvoøení instance tøídy AGS

AGS modul(1);
uint8_t infoStatus;
String number;
bool notificationSent;

//Vstupní parametry a inicializace shieldu

void setup()
{
	pinMode(RELE1, OUTPUT);
	digitalWrite(RELE1, LOW);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);
	modul.begin();
	notificationSent = false;

	//teplomìr
	sensors.begin();
	if (sensors.getDeviceCount()>0) Serial.println("Teplomer nalezen!");
	else Serial.println("Teplomer NENALEZEN!!!");
	oneWire.reset_search();
	oneWire.search(insideThermometer);
	//nastav rozliseni
	sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
	sensors.requestTemperatures();
	printAddress(insideThermometer);
	printTemperature(insideThermometer);
	Serial.println("****************************************");

}

//Hlavní smyèka – èeká na pøíchod SMS s obsahem Teplota?
//Jakmile je taková SMS rozpoznána, zmìøí se teplota a odešle
//se SMSka s obsahem Teplota:xx.x st.C
//Smyèka hlídá pøekroèení teploty (zde pevnì 30 st.), jakmile
//je teplota pøekroèená, odešle se SMS

void loop()
{
	//mìø periodicky teplotu
	sensors.requestTemperatures();
	teplota = sensors.getTempC(insideThermometer);
	
	infoStatus = modul.checkCallAndSMS();
	if (infoStatus == 2)
	{
		number = modul.getNumber();
		Serial.println("SMS from:" + number);

		if (modul.getSMSContent() == "Teplota?")
		{
			modul.sendSMS(number,"Teplota=" + String(teplota) + " st.C");
		}
	}
	//zkontroluj teplotu a je-li vyšší než 30 st., pak pošli SMS
	//nastav pøíznak, že SMS byla poslána, a nechodí furt dookola
	if ((teplota > 30.0)&&(notificationSent == false))
	{
		modul.sendSMS(SERVICE_NUMBER, "Pozor, teplota byla prekrocena!!!");
		notificationSent = true;
		Serial.println("Teplota prekrocena!!! Odeslana SMS.");
	}
	if (teplota < 25.0) notificationSent = false;
		
	delay(1000);
}


//pro teplomìr
void printAddress(DeviceAddress deviceAddress)
{
	Serial.print("Adresa teplomeru:");
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
	Serial.println("");
}

void printTemperature(DeviceAddress deviceAddress)
{
	float tempC = sensors.getTempC(deviceAddress);
	Serial.print("Teplota: ");
	Serial.print(tempC);
	Serial.write(176);
	Serial.println("C");
}
