//Utilities for ArduinotechGSMShield

#include "Arduino.h"
#include "ArduinotechGSMShield.h"
#include "SoftwareSerial.h"

SoftwareSerial AGS::SIM800(RX,TX);

AGS::AGS(uint8_t debug)
{
	_debug = debug;
}

void AGS::begin()
{
	SIM800.begin(9600);
	Serial.begin(9600);
	//restartSIMHW();
	SIM800Init();
}

//SIM800 initialization procedure for simple SMS and call features
void AGS::SIM800Init()
{

	while (Serial.available() > 0) Serial.read();
	Serial.println F("****************************************");
	while (sendATcommand("AT", "OK", 2000) == 0);
	Serial.println F("SIM800 available!");
	//otestuj registraci do site
	while ((sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
		sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0);
	while (Serial.available() > 0) Serial.read();
	Serial.println F("SIM800 registered to GSM!");
	//parameters to obtain time stamp from GSM network
	while (sendATcommand("AT+CLTS=1", "OK", 500) == 0);
	while (sendATcommand("AT+CENG=3", "OK", 500) == 0);
	while (Serial.available() > 0) Serial.read();
	Serial.print("Time stamp from GSM:" + timeStamp());
	Serial.println("GSM signal quality:" + getQuality());
	Serial.println("GSM provider:" + getProviderName());
	sendATcommand("AT+CMGF=1", "OK", 500);
	//zakaz indikace SMS
	sendATcommand("AT+CNMI=0,0", "OK", 500);
	//CLIP enabled
	sendATcommand("AT+CLIP=1", "OK", 1000);
	//smaz vsechny SMSky
	sendATcommand("AT+CMGD=1,4", "OK", 2000);
	sendATcommand("AT+CMGD=1", "OK", 2000);
	Serial.println F("SIM800 has been configured!");
	Serial.println F("****************************************");
}

//Send AT command to SIM800
int8_t AGS::sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout)
{

	//WDT_Restart(WDT);
	uint8_t x = 0, answer = 0;
	char response[100];
	unsigned long previous;

	memset(response, '\0', 100);    // Initialize the string

	delay(100);

	//clrSIMbuffer();
	SIM800.println(ATcommand);    // Send the AT command
								  //WDT_Restart(WDT);

	x = 0;
	previous = millis();

	// this loop waits for the answer
	do
	{
		if (SIM800.available() != 0) {
			// if there are data in the UART input buffer, reads it and checks for the asnwer
			response[x] = SIM800.read();
			x++;
			// check if the desired answer  is in the response of the module
			if (strstr(response, expected_answer) != NULL)
			{
				answer = 1;
			}
		}
		// Waits for the asnwer with time out
	} while ((answer == 0) && ((millis() - previous) < timeout));

	//clrSIMbuffer();
	return answer;
}


//Send AT command to SIM800 with response
String AGS::sendATcommandResponse(char* ATcommand, char* expected_answer, unsigned int timeout, unsigned int buf)
{
	//WDT_Restart(WDT);
	uint8_t x = 0, answer = 0;
	char response[150];
	unsigned long previous;
	String rest;

	memset(response, '\0', 100);    // Initialize the string

	delay(100);

	//clrSIMbuffer();
	SIM800.println(ATcommand);    // Send the AT command


	x = 0;
	previous = millis();
	//WDT_Restart(WDT);
	// this loop waits for the answer
	do
	{
		if (SIM800.available() != 0) {
			// if there are data in the UART input buffer, reads it and checks for the asnwer
			response[x] = SIM800.read();
			x++;
			// check if the desired answer  is in the response of the module
			if (strstr(response, expected_answer) != NULL)
			{
				answer = 1;
			}
		}
		// Waits for the asnwer with time out
	} while ((answer == 0) && ((millis() - previous) < timeout));

	//p?e?ti zbytek - max 20 byte
	memset(response, '\0', buf);    // Initialize the string
	delay(100);
	for (x = 0; x < buf; x++) response[x] = SIM800.read();
	//clrSIMbuffer();
	for (x = 0; x < buf; x++) rest += char(response[x]);
	delay(100);
	return rest;

}
//Check SIM800 if call or SMS is present
uint8_t AGS::checkCallAndSMS()
{
	char g;
	String gcmd;
	uint8_t gindex;
	
	//check call presence
	
	while (SIM800.available()>0)
	{
		g = SIM800.read();
		gcmd += g;
		if (g == '\n')
		{
			gcmd = gcmd.substring(0, gcmd.length() - 2);
			gindex = gcmd.indexOf('+');
			if (gcmd.substring(gindex+1,gindex+6) == "CLIP:") 
			{
				

				gindex = gcmd.indexOf(':');
				gcmd = gcmd.substring((gindex + 6), (gindex + 15));
				if(_debug == 1)
				{
					Serial.println("Call received!");
					Serial.println("Number:"+gcmd);
				}
				callInProgress = false;
				number = gcmd;
				return 1;
			}
			else return 0;
		}
	}
	
	gcmd = "";
	
	
	//test SMS presence
	clrSIMbuffer();
	SIM800.println("AT+CMGR=1");
	delay(100);
	//echo surpress
	while (SIM800.available()>0)
	{
		g = SIM800.read();
		gcmd += g;
		if (g == '\n')
		{			
			gcmd = "";
			break;
		}
	}

	//read first line with command response
	while (SIM800.available()>0)
	{
		g = SIM800.read();
		gcmd += g;
		if (g == '\n')
		{

			if (gcmd.substring(0, 2) == "OK") return 0;
			
			if (gcmd.substring(0, 5) == "+CMGR")
			{
				
				if (_debug == 1)
				{
					Serial.println F("****************************************");
					Serial.println F("New SMS alert!");
				}
				
				//first + "+CMGR"
				gindex = gcmd.indexOf('+');
				if (gindex < 0) return 0;
				//cut off first + character
				gcmd = gcmd.substring(gindex + 1);
				//sender number
				gindex = gcmd.indexOf('+');
				number = gcmd.substring(gindex + 4, gindex + 13);
				Serial.println("From:" + number);
				//read SMS content
				gcmd = "";
				delay(50);
				while (SIM800.available()>0)
				{
					g = SIM800.read();
					gcmd += g;
					if (g == '\n')
					{
						gcmd = gcmd.substring(0, gcmd.length() - 2);
						Serial.println("SMS Content:" + gcmd);
						sendATcommand("AT+CMGD=1", "OK", 1000);
						sendATcommand("AT+CMGD=1,4", "OK", 1000);
						if (_debug == 1)
						{
							Serial.println F("SMS deleted.");
							Serial.println F("****************************************");
						}
						SMScontent = gcmd;
						return 2;
					}
				}
				gcmd = "";
				sendATcommand("AT+CMGD=1", "OK", 1000);
				sendATcommand("AT+CMGD=1,4", "OK", 1000);
				return 0;
			}


		}
	}
	return 0;
}

//Call End
void AGS::callEnd()
{
	sendATcommand("ATH","OK",2000);
	return;
}
//Make call
void AGS::makeCall(String callNumber)
{
	SIM800.print("ATD ");
	SIM800.print(callNumber);
	SIM800.println(";");
}


//Check if SMS is present in SIM800
/*
uint8_t AGS::checkSMS()
{

	char g;
	String gcmd;
	uint8_t gindex;
	clrSIMbuffer();
	SIM800.println("AT+CMGR=1");
	delay(100);
	//echo surpress
	while (SIM800.available()>0)
	{
		g = SIM800.read();
		gcmd += g;
		if (g == '\n')
		{
			gcmd = "";
			break;
		}
	}

	//read first line with command response
	while (SIM800.available()>0)
	{
		g = SIM800.read();
		gcmd += g;
		if (g == '\n')
		{

			if (gcmd.substring(0, 2) == "OK") return 0;
			
			if (gcmd.substring(0, 5) == "+CMGR")
			{
				
				if (_debug == 1)
				{
					Serial.println F("****************************************");
					Serial.println F("New SMS alert!");
				}
				
				//first + "+CMGR"
				gindex = gcmd.indexOf('+');
				if (gindex < 0) return 0;
				//cut off first + character
				gcmd = gcmd.substring(gindex + 1);
				//sender number
				gindex = gcmd.indexOf('+');
				number = gcmd.substring(gindex + 4, gindex + 13);
				Serial.println("From:" + number);
				//read SMS content
				gcmd = "";
				delay(50);
				while (SIM800.available()>0)
				{
					g = SIM800.read();
					gcmd += g;
					if (g == '\n')
					{
						gcmd = gcmd.substring(0, gcmd.length() - 2);
						Serial.println("SMS content:" + gcmd);
						sendATcommand("AT+CMGD=1", "OK", 1000);
						sendATcommand("AT+CMGD=1,4", "OK", 1000);
						if (_debug == 1)
						{
							Serial.println F("SMS deleted.");
							Serial.println F("****************************************");
						}
						return 2;
					}
				}
				gcmd = "";
				sendATcommand("AT+CMGD=1", "OK", 1000);
				sendATcommand("AT+CMGD=1,4", "OK", 1000);
				return 0;
			}


		}
	}
	return 0;
}
*/
//Get last Sender or A-party number
String AGS::getNumber()
{
	return number;
}
//get SMS content
String AGS::getSMSContent()
{
	return SMScontent;
}

//Get Provider Name
String AGS::getProviderName()
{
	providerName = sendATcommandResponse("AT+COPS?", "+COPS:", 1000, 20);
	tempIndex = providerName.indexOf('"');
	providerName = providerName.substring(tempIndex + 1);
	tempIndex = providerName.indexOf('"');
	providerName = providerName.substring(0, tempIndex);
	return providerName;

}
//Get signal quality
String AGS::getQuality()
{
	GSMsignal = sendATcommandResponse("AT+CSQ", "+CSQ:", 1000, 3);
	return GSMsignal;
}

//Send SMS
void AGS::sendSMS(String number, String sms)
{
	if (_debug==1)
	{
		Serial.println("SMS pro:" + number);
		Serial.println("Obsah:" + sms);
		delay(500);
	}
	
	clrSIMbuffer();
	//number = "+420"+number;
	SIM800.println("AT+CMGS=\"" + number + "\"");
	delay(200);
	//toSerial();
	SIM800.println(sms);        // message to send
	delay(100);
	SIM800.write((char)26);	//CTRL+Z
	delay(100);
	SIM800.println();
	delay(100);
	sendATcommand("AT+CMGD=1", "OK", 2000);
	sendATcommand("AT+CMGD=1,4", "OK", 2000);
	delay(500);
	clrSIMbuffer();
	if (_debug==1)
	{
		Serial.println("SMS odeslana!");
		Serial.println F("****************************************");
	}
	
}

//Time stamp from SIM800 - GSM network time
String AGS::timeStamp()
{
	String ts = "";
	ts = sendATcommandResponse("AT+CCLK?", "+CCLK: \"", 1000, 20);
	//13/11/04,15:23:19+04
	//return ("20"+ts.substring(0,2)+ts.substring(3,5)+ts.substring(6,8)+ts.substring(9,11)+ts.substring(12,14)+ts.substring(15,17));
	return ts;
}

//HW restart of SIM800
void AGS::restartSIMHW()
{
	//HW restart
	digitalWrite(GSMReset, LOW);
	delay(400);
	digitalWrite(GSMReset, HIGH);
	delay(400);
}

//Clear SIM800 buffer
void AGS::clrSIMbuffer()
{
	while (SIM800.available()>0)
	{
		delay(1);
		SIM800.read();
	}
}
//Check if SIM800 is attached to GSM
bool AGS::isConnected()
{
	if ((sendATcommand("AT+CREG?", "+CREG: 0,1", 1000) ||
		sendATcommand("AT+CREG?", "+CREG: 0,5", 1000)) == 1) return true;
	else return false;
}
