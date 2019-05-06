#ifndef _SOCKET_H
#define _SOCKET_H
#include "types.h"
#include "w5300.h"


/**********************************
 * define function of SOCKET APIs * 
 **********************************/

/**
 * Open a SOCKET.
 */ 
uint8    socket(SOCKET s, uint8 protocol, uint16 port, uint16 flag);

/**
 * Close a SOCKET.
 */ 
void     close(SOCKET s);                                                           // Close socket

/**
 * It tries to connect a client.
 */
uint8    connect(SOCKET s, uint8 * addr, uint16 port);

/**
 * It tries to disconnect a connection SOCKET with a peer.
 */
void     disconnect(SOCKET s); 

/**
 * It is listening to a connect-request from a client.
 */
uint8    listen(SOCKET s);	    

/**
 * It sends TCP data on a connection SOCKET
 */
uint32   send(SOCKET s, uint8 * buf, uint32 len);


/**
 * It receives TCP data on a connection SOCKET
 */
uint32   recv(SOCKET s, uint8 * buf, uint32 len);

/**
 * It sends UDP, IPRAW, or MACRAW data 
 */
uint32   sendto(SOCKET s, uint8 * buf, uint32 len, uint8 * addr, uint16 port); 

/**
 * It receives UDP, IPRAW, or MACRAW data
 */
uint32   recvfrom(SOCKET s, uint8 * buf, uint32 len, uint8 * addr, uint16  *port);

#endif
