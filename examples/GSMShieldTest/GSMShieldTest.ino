#include <SoftwareSerial.h>
#include <U8glib.h>

#define RESET 4
#define LED 7
#define RELE1 8
#define RELE2 9
#define IN1 5
#define IN2 6
#define RX 2
#define TX 3

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
//U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_FAST);
uint8_t lineCounter = 0;
String lineTerminal[5];

SoftwareSerial SIM800(RX,TX);
char g,s;


// the setup function runs once when you press reset or power the board
void setup() {

  SIM800.begin(9600);
  Serial.begin(9600);
  lineCounter = 0;
  u8g.setFont(u8g_font_7x14);
  miniPrint("GSM Shield Test");
  pinMode(LED, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(RELE1, OUTPUT);
  pinMode(RELE2, OUTPUT);
  pinMode(IN1,INPUT);
  pinMode(IN2,INPUT);
  digitalWrite(LED,HIGH);
  digitalWrite(RELE1, LOW);
  digitalWrite(RELE2, LOW);
  digitalWrite(RESET, LOW);
  miniPrint("Rele OFF");
  delay(500);
  miniPrint("SIM800 ON");
  digitalWrite(LED,LOW);
  digitalWrite(RESET, HIGH);
  digitalWrite(RELE1, HIGH);
  digitalWrite(RELE2, HIGH);
  miniPrint("Rele ON");
  delay(1000);
  digitalWrite(RELE1, LOW);
  digitalWrite(RELE2, LOW);
  digitalWrite(LED,HIGH);
  miniPrint("Rele OFF");
  
 }

// the loop function runs over and over again forever
void loop() {
  if(Serial.available()>0)
  {
    g = Serial.read();
    SIM800.print(g);
  }
  if (SIM800.available()>0)
  {
    s = SIM800.read();
    Serial.print(s);
  }
  
  
  if ((digitalRead(IN1) == false) || (digitalRead(IN2) == false)) 
    {
        digitalWrite(LED,LOW);
        if ((digitalRead(IN1) == false)) miniPrint("IN1 LOW");
        if ((digitalRead(IN2) == false)) miniPrint("IN2 LOW");
        delay(200);
    }
  
  else digitalWrite(LED,HIGH);
}

void miniPrint(String text)
{
  //cli();//zakaž přerušení
  uint8_t ii;

  if (lineCounter >= 4)
  {
    lineCounter = 3;
    //scroll nahoru
    for (ii = 0; ii<3; ii++)
    {
      lineTerminal[ii] = lineTerminal[ii + 1];
    }
    lineTerminal[3] = text;

  }

  lineTerminal[lineCounter] = "";
  lineTerminal[lineCounter] = text;
  lineCounter++;


  u8g.firstPage();
  do
  {
    for (ii = 0; ii<4; ii++)
    {
      u8g.setPrintPos(5, (ii * 16 + 10));
      u8g.print(lineTerminal[ii]);
    }


  } while (u8g.nextPage());

  //sei();

}
void clearDisplay()
{
  //cli();
  u8g.firstPage();
  do
  {
    u8g.print(" ");
  } while (u8g.nextPage());
  u8g.setFont(u8g_font_7x14);
  for (uint8_t i = 0; i<4; i++)
  {
    lineTerminal[i] = "";
  }
  lineCounter = 0;
  //sei();
}


