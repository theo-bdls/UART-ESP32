#include "WiFi.h"

int led = D10;

void setup()
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  WiFi.softAP("ESP_AP", "123456789");
}

void loop()
{
  digitalWrite(led, HIGH);   // turn the LED on 
  delay(500);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off
  delay(500);               // wait for a second
  Serial.print("Host Name:");
  Serial.println(WiFi.softAPgetHostname());
  Serial.print("Host IP:");
  Serial.println(WiFi.softAPIP());
  //Serial.print("Host IPV6:");
  //Serial.println(WiFi.softAPIPv6());
  Serial.print("Host SSID:");
  Serial.println(WiFi.SSID());
  Serial.print("Host Broadcast IP:");
  Serial.println(WiFi.softAPBroadcastIP());
  Serial.print("Host mac Address:");
  Serial.println(WiFi.softAPmacAddress());
  Serial.print("Number of Host Connections:");
  Serial.println(WiFi.softAPgetStationNum());
  Serial.print("Host Network ID:");
  Serial.println(WiFi.softAPNetworkID());
  Serial.print("Host Status:");
  Serial.println(WiFi.status());
  delay(1000);
}