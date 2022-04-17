#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <stdio.h>
static int serial_fputchar(const char ch, FILE *stream) { Serial.write(ch); return ch; }
static FILE *serial_stream = fdevopen(serial_fputchar, NULL);

RF24 radio(8,7);

long X = A7;
long Y = A6;
long BTN = 2;
long EF = 3;
long EB = 4;

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};

byte counter;

long value[5] = {0, 0, 0, 0, 0};
int led_prev = 0;

void setup()
{
  Serial.begin(9600);
  printf("Sizeof - %d\n", sizeof(int));
  pinMode(BTN, INPUT_PULLUP);
  pinMode(X, INPUT_PULLUP);
  pinMode(Y, INPUT_PULLUP);
  pinMode(EF, INPUT_PULLUP);
  pinMode(EB, INPUT_PULLUP);
  radio.begin();
  radio.setAutoAck(1);
  radio.setRetries(0,15);
  radio.openWritingPipe(address[0]);
  radio.setChannel(0x60);
  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_1MBPS);
  radio.powerUp();
  radio.stopListening();
}

void loop(void) 
{
  value[0] = map(analogRead(X) - 512 - 10, -518, 506, -255, 255);
  value[1] = map(analogRead(Y) - 512 - 6, -518, 506, -255, 255) * (-1);
  value[2] = !digitalRead(BTN);
  value[3] = !digitalRead(EF);
  value[4] = !digitalRead(EB);
  printf("x = %ld, y = %ld, SW = %ld, EF = %ld, EB = %ld\n", value[0], value[1], value[2], value[3], value[4]);
  radio.write(&value, sizeof(value));
}
