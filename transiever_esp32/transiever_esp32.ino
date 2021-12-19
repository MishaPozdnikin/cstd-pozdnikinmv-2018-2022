#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(12,14);
byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};

const char* ssid = "Natka";
const char* password = "380635861072";
const char* serverNameLeftMotor = "http://192.168.0.107:8080/left_motor";
const char* serverNameRightMotor = "http://192.168.0.107:8080/right_motor";
const char* serverNameLed = "http://192.168.0.107:8080/led";

short msg[3] = { 0, 0, 0 };

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

const long interval = 1000; 
unsigned long current_tick = 1;

const char *delim = ";";
unsigned int delims_num = 0;

String httpGETRequest(const char* serverName);
short getValue(String input, const char* delimiters, int pos_to_get);
int getDelimsNum(String input, const char* delimiters);

void setup(){
  SPI.begin(26, 25, 27, 14);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) 
  { 
    delay(500);
    Serial.print(".");
  }
  delims_num = getDelimsNum(httpGETRequest(serverNameLeftMotor), delim);
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  radio.begin();
  radio.setAutoAck(1);
  radio.setRetries(0,15);
  radio.openWritingPipe(address[1]);
  radio.setChannel(0x60);
  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_1MBPS);
  radio.powerUp();
  radio.stopListening();
}

void loop(void) 
{
  if(WiFi.status()== WL_CONNECTED )
  {
    if (current_tick <= delims_num)
    {
      msg[0] = getValue(httpGETRequest(serverNameLeftMotor), delim, current_tick);
      msg[1] = getValue(httpGETRequest(serverNameRightMotor), delim, current_tick);
      msg[2] = getValue(httpGETRequest(serverNameLed), delim, current_tick);
      Serial.println(msg[0]);
      Serial.println(msg[1]);
      Serial.println(msg[2]);
      Serial.println();
      currentMillis = millis();
      previousMillis = currentMillis;
      while (1)
      {
        if (currentMillis - previousMillis <= interval)
        {
          radio.write(&msg, sizeof(msg));
        }
        else
        {
          break;
        }
        currentMillis = millis();
      }
    }
    else 
    {
      Serial.println("Parse complited.");
      while (1) {}
    }
  }
  current_tick++;
}

String httpGETRequest(const char* serverName) 
{
  WiFiClient client;
  HTTPClient http;
    
  http.begin(client, serverName);
  
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) 
  {
    payload = http.getString();
  }
  else 
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}

short getValue(String input, const char* delimiters, int pos_to_get)
{
  char *c_input = strdup(input.c_str());
  char * token = strtok(c_input, delimiters);
  while( token != NULL ) 
  {
    const char *value = token;
    pos_to_get--;
    if (0 == pos_to_get)
    {
      free(c_input);
      return atoi(value);
    }
    token = strtok(NULL, delimiters);
  }
  free(c_input);
  return 0xDEAD;
}

int getDelimsNum(String input, const char* delimiters)
{
  char *c_input = strdup(input.c_str());
  char * token = strtok(c_input, delimiters);
  int delim_counter = 0;
  while( token != NULL ) 
  {
    delim_counter++;
    token = strtok(NULL, delimiters);
  }
  free(c_input);
  return delim_counter;
}
