#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeSans18pt7b.h>
#include "./credentials.h"

#define PROJECT_NAME "EPaper-Weather-Display"
#define TIME_ZONE (+2)
#define DEBUG
#ifdef DEBUG
#define DEBUG_LOG(x) Serial.print(x)
#else
#define DEBUG_LOG(x)
#endif

void helloWorld();
void callback(char *topic, byte *payload, unsigned int length);

GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));

const char *mqtt_server = "192.168.178.41";
const char *clientID = "NodeMCUEPaper";
const char *inTopic1Name = "NodeMCUEPaper/1/Name";
const char *inTopic1Value = "NodeMCUEPaper/1/Value";
const char *inTopic2Name = "NodeMCUEPaper/2/Name";
const char *inTopic2Value = "NodeMCUEPaper/2/Value";
const char *inTopic3Name = "NodeMCUEPaper/3/Name";
const char *inTopic3Value = "NodeMCUEPaper/3/Value";
const char *inTopic4Name = "NodeMCUEPaper/4/Name";
const char *inTopic4Value = "NodeMCUEPaper/4/Value";
String String1 = "Titel1:";
String String2 = "Titel2:";
String String3 = "Titel3:";
String String4 = "Titel4:";
String val1 = "0";
String val2 = "0";
String val3 = "0";
String val4 = "0";

WiFiClient espClient;
PubSubClient client(mqtt_server, 1883, callback, espClient);

char msg[50];

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  DEBUG_LOG("\nConnecting to ");
  DEBUG_LOG(WIFI_SSID);

  WiFi.persistent(false);
  //WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.hostname(PROJECT_NAME);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    DEBUG_LOG(".");
  }

  DEBUG_LOG("\nWiFi connected");
  DEBUG_LOG("\nIP Address: ");
  DEBUG_LOG(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  // Convert the incoming byte array to a string
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char *)payload;
  String Name = (char *)topic;

  DEBUG_LOG("\nMessage arrived on topic: ");
  DEBUG_LOG(topic);
  DEBUG_LOG("\nValue: ");
  DEBUG_LOG(message);
  DEBUG_LOG("\n");

  if (Name == inTopic1Name)
  {
    if (String1 != message)
    {
      String1 = message;
      helloWorld();
    }
  }
  if (Name == inTopic1Value)
  {
    if (val1 != message)
    {
      val1 = message;
      helloWorld();
    }
  }
  if (Name == inTopic2Name)
  {
    if (String2 != message)
    {
      String2 = message;
      helloWorld();
    }
  }
  if (Name == inTopic2Value)
  {
    if (val2 != message)
    {
      val2 = message;
      helloWorld();
    }
  }
  if (Name == inTopic3Name)
  {
    if (String3 != message)
    {
      String3 = message;
      helloWorld();
    }
  }
  if (Name == inTopic3Value)
  {
    if (val3 != message)
    {
      val3 = message;
      helloWorld();
    }
  }
  if (Name == inTopic4Name)
  {
    if (String4 != message)
    {
      String4 = message;
      helloWorld();
    }
  }
  if (Name == inTopic4Value)
  {
    if (val4 != message)
    {
      val4 = message;
      helloWorld();
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    DEBUG_LOG("\nAttempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID, MQTT_USER, MQTT_PASS))
    {
      DEBUG_LOG("\nconnected");
      // ... and resubscribe
      client.subscribe(inTopic1Name);
      client.subscribe(inTopic1Value);
      client.subscribe(inTopic2Name);
      client.subscribe(inTopic2Value);
      client.subscribe(inTopic3Name);
      client.subscribe(inTopic3Value);
      client.subscribe(inTopic4Name);
      client.subscribe(inTopic4Value);
    }
    else
    {
      DEBUG_LOG("\nfailed, rc=");
      DEBUG_LOG(client.state());
      DEBUG_LOG("\n try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  if (client.connect(clientID, MQTT_USER, MQTT_PASS))
  {
    client.subscribe(inTopic1Name);
    client.subscribe(inTopic1Value);
    client.subscribe(inTopic2Name);
    client.subscribe(inTopic2Value);
    client.subscribe(inTopic3Name);
    client.subscribe(inTopic3Value);
    client.subscribe(inTopic4Name);
    client.subscribe(inTopic4Value);
  }

  delay(100);
  display.init(115200);
  // first update should be full refresh
  //helloWorld();
  //delay(1000);
  //display.powerOff();
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

void helloWorld()
{
  display.setRotation(1);
  display.setFont(&FreeSans18pt7b);
  display.setTextColor(GxEPD_BLACK);
  int Col1 = 0;
  int Col2 = 170;
  int Row1 = 30;
  int Row2 = 62;
  int Row3 = 94;
  int Row4 = 126;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    display.setCursor(Col1, Row1);
    display.print(String1);
    display.setCursor(Col2, Row1);
    display.print(val1);

    display.setCursor(Col1, Row2);
    display.print(String2);
    display.setCursor(Col2, Row2);
    display.print(val2);

    display.setCursor(Col1, Row3);
    display.print(String3);
    display.setCursor(Col2, Row3);
    display.print(val3);

    display.setCursor(Col1, Row4);
    display.print(String4);
    display.setCursor(Col2, Row4);
    display.print(val4);
  } while (display.nextPage());
}