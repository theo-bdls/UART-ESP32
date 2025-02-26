## RotatingInsectRadar : UART-WiFi bridge to send data

## Description
This project takes part of a most important one, which aims to follow an insect with drones. To do so, a radar is needed to follow the movement of the insect. First, the data from the radar was sent to the drone by a UART-USB connection. But, the radar is rotating, so this type of connection can't be used. Therefore, a solution is to used a UART-WiFi connection. The data from the radar are sent with UART wire to the ESP32C3 module which can send the data by WiFi to the drone autopilot. 
In this application, you will see how to handle the ESP32C3 module and then how to receive the data from the module in an approriate output. 

## Setup your computer 

You can use this application with Windows, OS X, Linux or anything else. I did it on MacOs.

You need to install Arduino on your computer. I used the Arduino IDE. 
- [Arduino IDE](https://www.arduino.cc/en/software)

Now, make sure to add the ESP32 library 
- [ESP32 library](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-arduino-ide)

You need to install Matlab (need a license) :

- [Matlab Software](https://au.mathworks.com/products/matlab.html)

I advice you to install the most common library first but be sure to add "Instrument Control Toolbox". We will need it for the UDP function. 

## First step with Arduino and ESP32C3 ? 

The ESP32 used in this project is the XIAO ESP32C3 : [ESP32C3 Doc](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)

The ESP32-S3 is more powerful than the ESP32-C3: for our application S3 will be used: software still the same, the GPIO Rx number and the size of Rx buffer size are the only parameters to change : [ESP32S3 Doc](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/)

Most of the code in the UARTWiFiBridge_Tuto was on this website [ESP32C3 WiFi Use](https://wiki.seeedstudio.com/XIAO_ESP32C3_WiFi_Usage/). Before going further, be sure those code are working. 

Few piece of advice. First, you just need to connect the card to your computer with a USB wire.

Make sure to choose "XIAO ESP32C3" on the Arduino IDE. To do so, Tools>Board>esp32. If esp32 does not appear, that's mean your software is not installed well. Make sure you add esp32 library. 

Then choose the right port. Tools>Port. If you are not sure which Port is the right one, this website should help you : [Finding Port](https://docs.netgate.com/pfsense/en/latest/solutions/sg-1100/connect-to-console.html)

Now, you should be ready to upload your first code into the ESP32C3 module. 
If you can't flash/upload your code into the module, the memory should be full. You should enter into "Download mode". Before flashing (uploading by using the arrow on the top left of the IDE), press the BOOT button and hold it, then press once the RESET button. The name of the board (top left) should blink briefly. Release the BOOT button. 

Make sure you can connect the ESP32 board to a WiFi. Your home WiFi, work WiFi, even by using your phone as a hot spot. This application is really important for our project.

If you don't see any output in the Arduino IDE Serial Monitor, press the RESET button on the ESP32 board. If it's not working, take out the USB wire from your computer and put it again. It can take few seconds before observing an output.

## Usage

Now, let's see how it works to use this application. 
Open your Arduino IDE, connect the ESP32 to your computer after adding the WiFi antenna on the board.

Opened the UARTWiFiBridge_MODE1: this code connect the ESP32 to a WiFi and send packets to a UDP server host by the device which you provide the IP. 
First, you must change the WiFi name (SSID) and the password. Put the parameters that correspond to the WiFi used. Don't forget to connect the receiver (as an example your laptop) to the same WiFi and changed the IP adress in the Arduino code. Put the IP of the receiver = where you want to receive the data sent by WiFi.

You can also use the UARTWifiBridge_Simulation, this code sent data to the UDP server even if you don't have a serial connection with an UART.


## Set up 

First you need to power the radar and connect it to the IF module. 
On the UART radar connection : -black wire = GND
                               -red wire = +5V
                               -white wire = data (output in char)

You need to connect the black wire to the GND pin of the ESP board, red wire with the +5V pin and white wire with the RX pin (GPIO 20 for ESP32C3 and GPIO44 for ESP32S3). 
Connect the board to your computer with a USB. This allows you to watch the data on the Arduino IDE and also your computer power the board. In a real situation, we will use a 5V battery to power the ESP board with the 5V and GND pins. 

When everything is set up, upload the code (UART_WifiBridge_MODE1.ino) into the board. Open the serial monitor. You should see your data written and some messages that precise you are connected to the WiFi, the UDP port in use and the IP of the client (ESP32) which can be useful for the C++ application.

# Software option

You can stream the data and store them into a file with two different applications: 

- Matlab: Online_processing>Online_processing_storage.m
- C++: main.cpp

To compile the C++ you should use the cmake file, please refer to the attached pdf for the whole tutorial.

## Support
If you struggling with anything you can find some clue on the previous website I gave. 
You can also email me : theo.bruneaudelasalle@bordeaux-inp.fr

## Contributing
Anyone can contribute to this project. Please contribute to improve the robustness, the efficiency of the system. 
Make sure to keep the old version please and to make comment when you are adding a new application. 

## Authors and acknowledgment
I want to thank Professor Graeme WOODWARD, who is at the initiative of this development. 

I want to thank Rifat AFROZ, who is working on the rotating radar and helped me by providing useful resources in Matlab.

I want to thank the Wireless Research Centre in the University of Canterbury, who is at the initative of this project. 

## License
LICENSE

## Project status


