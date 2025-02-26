/* This application is using the ESP32C3 to send data by WiFi. 
Those data in our application are received by a radar connected to a UART wire.
Data test are one sequence from the Radar.
The data form is :

Proposed Dataformat based on NMEA data structure
After each scan the Radar will transmit
- 1 x $RT sentence	26 Bytes
- 1 x $RS sentence	24 Bytes
- 25 x $RH sentences	25 * 53  = 1325 Bytes
- 13 x $RL sentences	13 * 53  = 689 Bytes
	Total		2064 Bytes
*/

/*To be used, ssid and password for the wifi connection 
has to be changed each time
 IP of the host (udpAddress) is the IP of the embedded computer
 connected to the same wifi of the ESP32
 If ESP32-C3: RxSize = 512, RxPin = 20
 If ESP32-S3: RxSize = 1024 RxPin = 44
*/


#include <WiFi.h>
#include <WiFiUdp.h>

#define UDP_PORT 3001 
#define SSID "TP-Link_6AD3"
#define PASSWORD "48309158"
#define IPHOST "192.168.0.73"

// Configuration 
WiFiUDP Udp;

void setup() {
  Serial.begin(921600); // PC communication no need if integrated to a system 

  WiFiConnection(SSID,PASSWORD);

  // Server UDP
  Udp.begin(UDP_PORT); //begin sending packet at this port, UDP client side on Arduino
  Serial.println("Server UDP bind to :");
  Serial.println(UDP_PORT);

}

void loop() {

  handleReadUART(IPHOST, UDP_PORT); // GPIO44 on S3 for Rx0, GPIO20 on C3 
  // 1024 Maximum size of UART buffer on S3

}


//////// Function definition ////////
/*
* Data reception and send packet
*/
void handleReadUART(const char* udpAddress, const unsigned int udpPort) {

    char* simulatedData= "YOU\n";
    Udp.beginPacket(udpAddress, UDP_PORT);
    Udp.write((const uint8_t*)simulatedData, strlen(simulatedData)); //need of a cast because of the write() arguments definitions
    Udp.endPacket();
    delay(1000);

}

/*
* Wifi connection and begin UDP 
*/
void WiFiConnection(const char* ssid, const char* password) {

  // WiFi Connexion
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi: ");
  Serial.println("IP address of the ESP32 (client): ");
  Serial.println(WiFi.localIP());
}

