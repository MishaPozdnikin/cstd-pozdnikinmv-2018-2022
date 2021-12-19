#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <stdio.h>

static int serial_fputchar(const char ch, FILE *stream) { Serial.write(ch); return ch; }
static FILE *serial_stream = fdevopen(serial_fputchar, NULL);

RF24 radio(8,7);
byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};

int engineLeftForward = 10;
int engineLeftBackward = 9;

int engineRightForward = 6;
int engineRightBackward = 5;

int LED_PIN = 2;
int led_prev = 0;

long gotByte[3];

int high_prec = 250;
int low_prec = 1;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(engineLeftForward, OUTPUT);
  pinMode(engineLeftBackward, OUTPUT);
  pinMode(engineRightForward, OUTPUT);
  radio.begin();
  radio.setAutoAck(1);        //режим підтвердження прийому
  radio.setRetries(0,15);     //(час між спробами досягти, кількість спроб)
  radio.openReadingPipe(1,address[0]);      //хочемо слухати трубу 0
  radio.setChannel(0x60);  //виберіть канал (в якому немає шуму!)
  radio.setPALevel (RF24_PA_MAX); //рівень потужності передавача. На вибір RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //швидкість обміну. На вибір RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  radio.powerUp(); //Початок роботи
  radio.startListening();  //почати слухати трансляцію, ми приймальний модуль
}

void loop(void) 
{
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
  printf("SW = %d, X = %d, Y = %d\n", gotByte[2], gotByte[0], gotByte[1]);
}
