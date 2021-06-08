
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define LED D0

const char* ssid = "pi.netgear"; 
const char* password = "domnickhunterrl"; 
const char* mqtt_server = "broker.hivemq.com"; 
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(D1, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(MQTTcallback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.subscribe("esp/test");
}

// Function Callback
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;
  for (int i = 0; i < length; i++) 
  {
    message = message + (char)payload[i];
  }
  Serial.print(message);
  if (message == "on") 
  {
    digitalWrite(LED, HIGH);
  }
  else if (message == "off") 
  {
    digitalWrite(LED, LOW);
  }
  Serial.println();
  Serial.println("-----------------------");
}
// ./End Callback Function

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(D1)==0)
  {
  client.publish("esp/test1", "Hello from ESP8266");
  delay(1000);
  }
  else;
  client.loop();
}
