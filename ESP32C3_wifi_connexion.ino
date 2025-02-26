#include <WiFi.h>

const char* ssid     = "TP-Link_6AD3"; // SSID 
const char* password = "48309158"; // Password


void setup()
{
    Serial.begin(921600);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
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
}  
void loop()
{
  }