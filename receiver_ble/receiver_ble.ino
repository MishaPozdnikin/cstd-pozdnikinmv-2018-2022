#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <BTLE.h>

RF24 radio(8,7);
byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};

BTLE btle(&radio);
String DeviceName = "nrfbt"; // Device name

int engineLeftForward = 10;
int engineLeftBackward = 9;

int engineRightForward = 6;
int engineRightBackward = 5;

int LED_PIN = 2;
int led_prev = 0;

int mode = 0;
int modeSelect = 3;
int bleEnable = 0;

int high_prec = 250;
int low_prec = 1;

int gotByte[3];

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(engineLeftForward, OUTPUT);
  pinMode(engineLeftBackward, OUTPUT);
  pinMode(engineRightForward, OUTPUT);
  pinMode(engineRightBackward, OUTPUT);
  pinMode(modeSelect, INPUT_PULLUP);
  mode = !digitalRead(3);
  if (mode) 
  {
    btle.begin("");
  } 
  else 
  {
    radio.begin();
    radio.setAutoAck(1);
    radio.setRetries(0,15);
    radio.openReadingPipe(1,address[0]);
    radio.setChannel(0x60);
    radio.setPALevel (RF24_PA_MAX);
    radio.setDataRate (RF24_1MBPS);
    radio.powerUp();
    radio.startListening();
  }
}

void loop(void) 
{
  if (mode) 
  {
    BLE_part();    
  } 
  else 
  {
    NRF_part();
  }
}

void BLE_part() 
{
  String string="";
  if (btle.listen()) 
  {
    for (uint8_t i = 2; i &&(btle.buffer.pl_size) - 6; i++) 
    {
      string = string + (char(btle.buffer.payload[i]));
    }
    if (string.startsWith(DeviceName)) 
    {
      Serial.println(string);
      string.replace(DeviceName,"");
      Serial.println(string);
      pinMode((string.charAt(2)),1);
      digitalWrite((string.charAt(2)),(string.charAt(3)));
    }
  }
}

int current_millis = 0;
int previous_millis = 0;
int interval = 1000;
void NRF_part() 
{
  current_millis = millis();
  if (radio.available()) 
  {
    radio.read(&gotByte, sizeof(gotByte));
    if (gotByte[1] > 0) 
    {
      if (gotByte[0] > low_prec && gotByte[0] < high_prec) 
      {
        analogWrite(engineLeftForward, gotByte[0]);
        analogWrite(engineRightForward, gotByte[1] - gotByte[0]);
      } 
      else if (gotByte[0] < -low_prec && gotByte[0] > -high_prec) 
      {
        analogWrite(engineLeftForward, gotByte[1] - gotByte[0] * (-1));
        analogWrite(engineRightForward, gotByte[1]);
      }
      else if (gotByte[0] < -low_prec && gotByte[0] > -high_prec) 
      {
        analogWrite(engineLeftForward, 0);
        analogWrite(engineRightForward, gotByte[1]);
      }
      else if (gotByte[0] > low_prec && gotByte[0] < high_prec) 
      {
        analogWrite(engineLeftForward, gotByte[0]);
        analogWrite(engineRightForward, 0);
      } 
      else if (gotByte[0] > -low_prec && gotByte[0] < low_prec)
      {
        analogWrite(engineLeftForward, gotByte[1]);
        analogWrite(engineRightForward, gotByte[1]);
      }
    } 
    else if (gotByte[1] < 0) 
    {
      if (gotByte[0] > low_prec && gotByte[0] < high_prec) 
      {
        analogWrite(engineLeftBackward, gotByte[1] * (-1));
        analogWrite(engineRightBackward, gotByte[1] * (-1) - gotByte[0]);
      } 
      else if (gotByte[0] < -low_prec && gotByte[0] > -high_prec) 
      {
        analogWrite(engineLeftBackward, gotByte[1] * (-1) - gotByte[0] * (-1));
        analogWrite(engineRightBackward, gotByte[1] * (-1));
      } 
      else if (gotByte[0] < -low_prec && gotByte[0] > -high_prec) 
      {
        analogWrite(engineLeftBackward, gotByte[1] * (-1));
        analogWrite(engineRightBackward, gotByte[1] * (-1));
      }
      else if (gotByte[0] > low_prec && gotByte[0] < high_prec) 
      {
        analogWrite(engineLeftBackward, gotByte[1] * (-1));
        analogWrite(engineRightBackward, gotByte[1] * (-1));
      }
      else if (gotByte[0] > -low_prec && gotByte[0] < low_prec) 
      {
        analogWrite(engineLeftBackward, gotByte[1] * (-1));
        analogWrite(engineRightBackward, gotByte[1] * (-1));
      }
    } 
    else 
    {
      if ((led_prev == 0 && gotByte[2] == 1) || (led_prev == 1 && gotByte[2] == 0)) 
      {
        digitalWrite(LED_PIN, gotByte[2]);
        led_prev = gotByte[2];
      }
      analogWrite(engineRightForward, 0);
      analogWrite(engineRightBackward, 0);
      analogWrite(engineLeftForward, 0);
      analogWrite(engineLeftBackward, 0);
    }
  }
  if (current_millis - previous_millis > interval)
  {
    Serial.print(gotByte[0]);
    Serial.print(" ");
    Serial.print(gotByte[1]);
    Serial.print(" ");
    Serial.print(gotByte[2]);
    Serial.println();
    previous_millis = current_millis;
  }
}
