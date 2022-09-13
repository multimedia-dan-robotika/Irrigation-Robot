#include <SPI.h>
#include <LoRa.h>

#define CS 10
#define RESET 9
#define DIO0 2

#define relay1 22
#define relay2 24

#define FW_IN1 31
#define FW_IN2 33
#define FW_IN3 35
#define FW_IN4 37
#define FW_RPWM 5
#define FW_LPWM 4

#define RW_IN1 28
#define RW_IN2 30
#define RW_IN3 32
#define RW_IN4 34
#define RW_RPWM 7
#define RW_LPWM 6

#define YELLOWLED 38
#define REDLED 40

void setup()
{
  pinMode (FW_IN1, OUTPUT);
  pinMode (FW_IN2, OUTPUT);
  pinMode (FW_IN3, OUTPUT);
  pinMode (FW_IN4, OUTPUT);
  pinMode (FW_RPWM, OUTPUT);
  pinMode (FW_LPWM, OUTPUT);

  pinMode (RW_IN1, OUTPUT);
  pinMode (RW_IN2, OUTPUT);
  pinMode (RW_IN3, OUTPUT);
  pinMode (RW_IN4, OUTPUT);
  pinMode (RW_RPWM, OUTPUT);
  pinMode (RW_LPWM, OUTPUT);

  pinMode (relay1, OUTPUT);
  pinMode (relay2, OUTPUT);

  pinMode (YELLOWLED, OUTPUT);
  pinMode (REDLED, OUTPUT);

  analogWrite (FW_RPWM, 200);
  analogWrite (FW_LPWM, 200);

  analogWrite (RW_RPWM, 200);
  analogWrite (RW_LPWM, 200);

  Serial.begin(9600);

  LoRa.setPins (CS, RESET, DIO0);
  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop()
{
  int moist, currentMoist;
  String ack;
  String packetHeader;
  String packetValue;
  int packetSize;
  
  packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    digitalWrite (REDLED, LOW);
    digitalWrite (YELLOWLED, HIGH);
    
    while (LoRa.available())
    {
      packetHeader = LoRa.readStringUntil ('#');
      packetValue = LoRa.readStringUntil ('#');
      if (packetHeader == "X")
      {
        moist = packetValue.toInt();
        Serial.print ("Header : ");
        Serial.print (packetHeader);
        Serial.print ("\tKelembapan: ");
        Serial.println (moist);
      }
      else
      {
        ack = packetValue;
        Serial.print ("Header : ");
        Serial.print (packetHeader);
        Serial.print ("\tRefill : ");
        Serial.println (ack);
      }
    }
    
    digitalWrite (YELLOWLED, HIGH);
  }
  else
  {
    digitalWrite (REDLED, HIGH);
  }

  if (moist > 600) //&& tankLevel == 1)
  {
    move (0, 1, 1, 0, 150, 150, 0, 1, 1, 0, 150, 150);
    digitalWrite (relay1, HIGH);
  }

  else
  {
    move (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    digitalWrite (relay1, LOW);
  }
}

void move
(
  byte FWin1, byte FWin2, byte FWin3, byte FWin4, int FWrpwm, int FWlpwm,
  byte RWin1, byte RWin2, byte RWin3, byte RWin4, int RWrpwm, int RWlpwm
)
{
  digitalWrite (FW_IN1, FWin1);
  digitalWrite (FW_IN2, FWin2);
  digitalWrite (FW_IN3, FWin3);
  digitalWrite (FW_IN4, FWin4);

  analogWrite (FW_RPWM, FWrpwm);
  analogWrite (FW_LPWM, FWlpwm);

  digitalWrite (RW_IN1, RWin1);
  digitalWrite (RW_IN2, RWin2);
  digitalWrite (RW_IN3, RWin3);
  digitalWrite (RW_IN4, RWin4);

  analogWrite (RW_RPWM, RWrpwm);
  analogWrite (RW_LPWM, RWlpwm);
}
