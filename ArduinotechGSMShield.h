//knihovna pro obsluhu shieldu Arduinotech GSM
//vytvořeno 15.11.2015
//Update:
//Autor: Ing. Petr Foltýn
//www.arduinotech.cz
#ifndef ArduinotechGSMShield
#define ArduinotechGSMShield

//Includes

#include "Arduino.h"
#include "SoftwareSerial.h"

//Definitions - according GSM shield HW setup
#define GSMReset 4
#define LED 7
#define RELE1 8
#define RELE2 9
#define IN1 5
#define IN2 6
#define RX 2
#define TX 3





class AGS
{
	public:
		String number;
		String SMScontent;
		AGS(uint8_t speed);
		static SoftwareSerial SIM800;
		void SIM800Init();
		void begin();
		void sendSMS(String number, String sms);
		//uint8_t checkSMS();
		//uint8_t checkCall();
		String getNumber();
		String getSMSContent();
		String getQuality();
		String getProviderName();
		
		void checkInput();
		String timeStamp();
		void restartSIMHW();
		void callEnd();
		void makeCall(String callNumber);
		uint8_t checkCallAndSMS();
		bool isConnected();
	
	
	private:
		uint8_t _debug;
		bool callInProgress;
		String providerName;
		String GSMsignal;
		bool rele1Status, rele2Status;
		bool IN1Status, IN2Status;
		uint8_t tempIndex;
		void clrSIMbuffer();
		String sendATcommandResponse(char* ATcommand, char* expected_answer, unsigned int timeout, unsigned int buf);
		int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout);
		
	
};

#endif