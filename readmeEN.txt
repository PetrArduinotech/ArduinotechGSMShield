ArduinotechGSMShield library description
The library is written for theArduinotech GSM shield, see details here (Sorry, Czech only at this moment):
http://www.arduinotech.cz/inpage/jak-jednoduse-na-gsm-v-dil-arduinotech-gsm-shield/

Class AGS
Class namey: AGS (abbreviation ArduinotechGSMShield)
Input parameteres: debug mód -> 1=on, 0=off
Output parameters: none
Example: 
creation of class instance:
AGS modul(1); //debug mode on
AGS modul(0); //debug mode off

begin();
Description: shield initialization, defines softwareSerial on RX=2, TX=3 pins, baud rate 9600 b/s, initialization of Arduino's serial port at 9600b/s, runs config procedure SIM800Init()
Input parameteres: none
Output parameteres: none
Example: 
modul.begin();
	
SIM800Init();
Description: Config [rocedure for SIM800 module, can be used separately for handling with incorrect status of SIM800 wherever within a code. The procedure is involved in begin();
Input parameteres: none
Output parameteres: none
Example: 
modul.SIM800Init();

checkCallAndSMS();
Description: Tests if call or new SMS is present. I recommend to put it into main loop.
Input parameteres: none
Output parameteresy: 0 = no call, no new SMS, 1 = new call present, 2 = new SMS
Example: 
loop()
{
	infoStatus = modul.checkCallAndSMS();
	if (infoStatus == 1)
	{
		//code for call handling
	}
	if (infoStatus == 2)
	{
		//acode for SMS handling
	}	
}


callEnd();
Description: Sends end call command to SIM800, active call is then terminated
Input parameteres: none
Output parameteres: none
Example:
modul.callEnd();

makeCall(String callNumber);
Description: Makes call set up to given number
Input parameteres: String telephone number in national or international form without + character
Output parameteres: none
Example:
modul.makeCall(“739822476“);

getNumber();
Description: Returns string with telephon number - subscriber who called or sent SMS. Mostly getNumber() is called immediatelly after checkCallAndSMS() to work further with incomming activity.
Input parameteres: none
Output parameteres: String telephone number
Example:
infoStatus = modul.checkCallAndSMS();
if (infoStatus == 1) Serial.println(“Call from:” + modul.getNumber());
if (infoStatus == 2) Serial.println(“SMS from:” + modul.getNumber());


getSMSContent();
Description: Get content of a received SMS
Input parameteres: none
Output parameteresy: String with content of the SMS
Example:
if (infoStatus == 2) 
{
	Serial.println(“SMS from:” + modul.getNumber());
	Serial.println(“SMS content:” + modul.getSMSContent());
}


sendSMS(String number,String sms);
Description: Sends SMS with given nuber and content
Input parameteres: String telephone number of recipient and String content of SMS
Output parameteres: none
Example:
modul.sendSMS(“73982246”,”Hello!”);


getProviderName();
Description: Returns string with name of GSM provider obtainet from the celluar network
Input parameteres: none
Output parameteres: String provider name
Example:
Serial.println(“GSM operator:” + getProviderName());

getQuality();
Description: Returns string with GSM signal quality which is paramter calculated from signal strength, latency and other parameters. This is definitely NOT a signal strength only! Values can be 0 = no signal, 1 to 30 - signal is present, the higher value, the better, 31 = overboosted.
Input parameteres: none
Output parameteres: String GSM signal quality
Pøíklad:
Serial.println(“GSM signal quality:” + getQuality());

timeStamp();
Popis: Returns string with time stamp obtained from GSM network, quite accurate one.
Input parameteres: none
Output parameteres: String time stamp
Example:
Serial.println(“GSM time:” + timeStamp());
