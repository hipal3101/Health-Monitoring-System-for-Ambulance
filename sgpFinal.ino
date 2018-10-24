
#include "DHT.h"

#include <ESP8266WiFi.h>
#include <MQTT.h>

#define CLIENT_ID "nodemcu"
#define TOPIC "data/temp"

#define DHTPIN 2     // what digital pin we're connected to

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int bpm=0;
int bp;
String payload;

//const char* broker = "test.mosquitto.org";  // Online MQTT Broker
const char* broker = "broker.hivemq.com";  // We can use PC as Broker 
unsigned int port = 1883; // Port number

// create MQTT object
MQTT myMqtt(CLIENT_ID, broker, port);

const char* ssid     = "JJP";
const char* password = "12345678";

void setup() 
{
  Serial.begin(115200);
  delay(1000);
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to MQTT server");  

  // setup callbacks
  myMqtt.onConnected(myConnectedCb);
  myMqtt.onDisconnected(myDisconnectedCb);
  myMqtt.onPublished(myPublishedCb);
  myMqtt.onData(myDataCb);
  
  Serial.println("connect mqtt...");
  myMqtt.connect();

  delay(10);
  dht.begin();
}

void loop() 
{
  float f = dht.readTemperature(true);
  // read the value from the sensor:
  for(int i=0;i<300;i++){
    sensorValue = analogRead(sensorPin);
    if(sensorValue>950)
    bpm++;
    delay(100);
    
  }
  bpm =bpm*2;
  bp=random(120,139);
//  Serial.print("bpm=");
//  Serial.println(bpm);
//  Serial.print("temp=");
//  Serial.println(f);

  if(isnan(f))
  {
    f=0;
  }
  payload = "{\"Temp\":"+(String)f+","+"\"BPM\":"+(String)bpm+","+"\"BP\":"+(String)bp+"}";
  Serial.print(payload);
  bpm=0;
  myMqtt.publish(TOPIC,payload);
  delay(1000);
}

void myConnectedCb() {
  Serial.println("connected to MQTT server");
}

void myDisconnectedCb() {
  Serial.println("disconnected. try to reconnect...");
  delay(500);
  myMqtt.connect();
}

void myPublishedCb() {
  Serial.println("published.");
}

void myDataCb(String& topic, String& data) {
  
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(data);
}
