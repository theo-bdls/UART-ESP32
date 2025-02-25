/* Written by Théo Bruneau de la Salle
 *
 * ENSEIRB-MATMECA student, France
 *
 * 2024
 */

#include "DataReception.h"

using namespace std;


// Constructor
socketUDP::socketUDP() // TO DO : use as argument 
{
    m_bufferSize = 1024;
}

// Destructor
socketUDP::~socketUDP()
{
}

/////// Function of the class ///////
/**
 * Open and bind to a socket
 * 
 * IN : 
 * sock : type int
 * addr_sock : physical address of the server
 * client_addr : physical address of the client (ESP32)
 * 
 * OUT : 
 * ID of the socket
 */
int socketUDP::Socket_Initialization(struct sockaddr_in addr_sock, struct sockaddr_in client_addr)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket");
        exit(1);
    }
    memset(&addr_sock, 0, sizeof(addr_sock));
    addr_sock.sin_family = AF_INET;
    addr_sock.sin_port = htons(UDPPORT);
    addr_sock.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&addr_sock, sizeof(addr_sock)) == -1)
    {
        perror("bind");
        exit(1);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(UDPPORT);
    client_addr.sin_addr.s_addr = inet_addr(IPCLIENT);

    if (connect(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1)
    {
        perror("connect");
        exit(1);
    }
    printf("Server UDP ready\n");
    return sock;
}
/**
 * Receive the data from UDP client (ESP32)
 * ESP32S3 : size max 1024 bytes (size of the UART RAM)
 * ESP32C3 : size max 512 bytes  (idem)
 * 
 * IN : 
 * 
 * sock : sock ID : int
 *  
 * data_len : size of the buffer 
 * 
 * OUT : 
 * buffer : data from the radar in uint8_t or char*
 * 
 */
void socketUDP::Receive_Data(int sock, char* buffer){
    ssize_t data_len = recv(sock, buffer, m_bufferSize, 0);
}

/**
 * Close the UDP server 
 * 
 * IN : 
 * 
 * sock : sock ID : int
 */
void socketUDP::Close_Socket(int sock){
    close(sock);
}


/**
 * Set the buffersize
 * 
 * IN : 
 * 
 * bufferSize : int
 */
void socketUDP::setBufferSocket(int bufferSize)
{
    m_bufferSize = bufferSize;
}

///////// Getters ///////
/**
 * Access to the private attribute of the class
 * 
 * OUT : 
 * 
 * struct SocketParameters (useful for IP and Port if necessary)
 * bufferSize : int
 */
socketUDP::SocketParameters socketUDP::getSocketParameters() const {
    SocketParameters Parameters;
    Parameters.bufferSize = m_bufferSize;
    return Parameters;
}
