Popis knihovny ArduinotechGSMShield
Tato knihobna je urèená pro tento shield:
http://www.arduinotech.cz/produkt/gsm-shield-arduinotech/, podrobnosti naleznete v èlánku:
http://www.arduinotech.cz/inpage/jak-jednoduse-na-gsm-v-dil-arduinotech-gsm-shield/

Tøída AGS
Název tøídy: AGS (zkratka ArduinotechGSMShield)
Vstupní parametry: debug mód -> 1=zapnutý, 0=vypnutý
Výstupní parametry: žádné
Pøíklad: 
vytvoøení instance tøídy ve sketchi:
AGS modul(1); //debug mód povolen
AGS modul(0); //debug mód zakázán

begin();
Popis: inicializuje shield, definuje softwareSerial na portech RX=2, TX=3, rychlost 9600 b/s, inicializuje sériový kanál Arduina na rychlosti 9600b/s, spouští konfiguraèní proces SIM800Init()
Vstupní parametry: žádné
Výstupní parametry: žádné
Pøíklad: 
modul.begin();
	
SIM800Init();
Popis: Konfiguraèní procedura pro SIM800 modul, lze použít samostatnì napø. pro inicializaci v pøípadì zjištìní nekorektního chování v urèitých pasážích kódu. Tato procedura je zahrnuta do funkce begin();
Vstupní parametry: žádné
Výstupní parametry: žádné
Pøíklad: 
modul.SIM800Init();

checkCallAndSMS();
Popis: Provádí otestování, zda je pøítomen pøíchozí hovor nebo nová SMSka. Doporuèuji vložit do hlavní smyèky
Vstupní parametry: žádné
Výstupní parametry: 0 = žádný hovor nebo nová SMS, 1 = nový hovor, 2 = nová SMS
Pøíklad: 
loop()
{
	infoStatus = modul.checkCallAndSMS();
if (infoStatus == 1)
{
	//akce pøi detekci volání
}
if (infoStatus == 2)
{
	//akce pøi detekci SMS
}	
}


callEnd();
Popis: Vyšle pøíkaz do SIM800 pro ukonèení stávajícího hovoru – zavìšení
Vstupní parametry: žádné
Výstupní parametry: žádné
Pøíklad:
modul.callEnd();

makeCall(String callNumber);
Popis: Provede vytoèení èísla – sestaví hlasový hovor
Vstupní parametry: String telefonní èíslo v národním nebo mezinárodním tvaru bez +
Výstupní parametry: žádné
Pøíklad:
modul.makeCall(“739822476“);

getNumber();
Popis: Vrátí øetìzec s telefonním èíslem voljícího nebo úèastníka, který poslal SMS. Obvykle je tento dotaz realizován bezprostøednì po vyhodnocení stavu checkCallAndSMS() funkce.
Vstupní parametry: žádné
Výstupní parametry: String telefonní èíslo
Pøíklad:
infoStatus = modul.checkCallAndSMS();
if (infoStatus == 1) Serial.println(“Volajici ucastnik:” + modul.getNumber());
if (infoStatus == 2) Serial.println(“SMS od:” + modul.getNumber());


getSMSContent();
Popis: Zjištìní obsahu pøijaté SMS
Vstupní parametry: žádné
Výstupní parametry: String obsah SMS
Pøíklad:
if (infoStatus == 2) 
{
Serial.println(“SMS od:” + modul.getNumber());
Serial.println(“Obsah SMS:” + modul.getSMSContent());
}


sendSMS(String number,String sms);
Popis: funkce pro odeslání SMS na dané èíslo s daným obsahem
Vstupní parametry: String èíslo pøíjemce a String obsah SMS
Výstupní parametry: žádné
Pøíklad:
modul.sendSMS(“73982246”,”Ahoj!”);


getProviderName();
Popis: Vrací øetìzec se jménem operátora GSM, které je prezentováno v rámci GSM sítì
Vstupní parametry: žádné
Výstupní parametry: String jméno operátora
Pøíklad:
Serial.println(“GSM operator:” + getProviderName());

getQuality();
Popis: Vrací øetìzec s kvalitou GSM signálu dle momentálního mìøení. Kvalita signálu je parametr vypoètený ze síly, latence a jiných parametrù, není to jen síla signálu! Tento parametr se pohybuje od 1 do 30, 0 = není signál, 31 = pøebuzení signál
Vstupní parametry: žádné
Výstupní parametry: String kvalita signálu
Pøíklad:
Serial.println(“GSM signal quality:” + getQuality());

timeStamp();
Popis: Vrací øetìzec s èasovou znaèkou obdrženou ze sítì GSM, tedy s pomìrnì pøesným èasem
Vstupní parametry: žádné
Výstupní parametry: String èasová znaèka
Pøíklad:
Serial.println(“GSM time:” + timeStamp());
