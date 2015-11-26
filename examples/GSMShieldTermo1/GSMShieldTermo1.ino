#include <ArduinotechGSMShield.h>
#include <SoftwareSerial.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#define LED 7
#define VENTILATOR 8
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
bool afterReset;

//Vstupní parametry a inicializace shieldu

void setup()
{
	pinMode(RELE1, OUTPUT);
	digitalWrite(RELE1, LOW);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
	modul.begin();
	notificationSent = true;
	digitalWrite(LED, HIGH);

	//teplomìr
	sensors.begin();
	if (sensors.getDeviceCount()>0) Serial.println F("Teplomer nalezen!");
	else Serial.println F("Teplomer NENALEZEN!!!");
	oneWire.reset_search();
	oneWire.search(insideThermometer);
	//nastav rozliseni
	sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
	sensors.requestTemperatures();
	printAddress(insideThermometer);
	printTemperature(insideThermometer);
	Serial.println F("****************************************");
	while (Serial.available() > 0) Serial.read();

}

//Hlavní smyèka – èeká na pøíchod SMS s obsahem Teplota?
//Jakmile je taková SMS rozpoznána, zmìøí se teplota a odešle
//se SMSka s obsahem Teplota:xx.x st.C
//Smyèka hlídá pøekroèení teploty (zde pevnì 30 st.), jakmile
//je teplota pøekroèená, odešle se SMS, zapne se ventilátor
//pokud se teplota vrátí pod 25 st., vypne se ventilátor
//Ventilátor = RELE1 na D8

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
	//zapni ventilátor
	//vychozi stav odpovídá poslané SMS notification=true - kdyby
	//bylo vychozí false, po resetu by se hned odeslala SMSka pokud by teplota 
	//byla v normálu
	if ((teplota > 30.0)&&(notificationSent == false))
	{
		modul.sendSMS(SERVICE_NUMBER, "Pozor, teplota byla prekrocena!!!");
		notificationSent = true;
		Serial.println F("Teplota prekrocena!!! Odeslana SMS.");
		digitalWrite(VENTILATOR, HIGH);
		Serial.println F("Ventilator byl zapnut");
	}
	//vypni ventilator
	if ((teplota < 24.0) && (notificationSent == false))
	{
		modul.sendSMS(SERVICE_NUMBER, "Teplota v normalu");
		notificationSent = true;
		digitalWrite(VENTILATOR, LOW);
		Serial.println F("Ventilator byl vypnut");
	}
	//smaž pøíznak odeslané SMSky, jakmile se teplota pøiblíží
	//k hranici sepnutí ventiátoru
	if ((teplota > 29.0)&&(teplota < 30.0))
	{
		notificationSent = false;
	}
	//pokud je teplota vyšší jak 35 st. na nic neèekej
	//a zapni ventilátor - ošetøení stavu po restartu
	if (teplota > 35.0) digitalWrite(VENTILATOR, HIGH);
	delay(1000);
	while (Serial.available() > 0) Serial.read();
}


//pro teplomìr
void printAddress(DeviceAddress deviceAddress)
{
	Serial.print F("Adresa teplomeru:");
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
	Serial.print F("Teplota: ");
	Serial.print(tempC);
	Serial.write(176);
	Serial.println("C");
}
