#ifndef _TYPES_H
#define _TYPES_H


typedef char int8;                       
typedef volatile char vint8;                          
typedef unsigned char uint8;                                  
typedef volatile unsigned char vuint8;               
typedef short int16;                                              
typedef volatile short vint16;                                                   
typedef unsigned short uint16;                                               
typedef volatile unsigned short vuint16;                                  
typedef long int32;                                          
typedef volatile long vint32;                                           
typedef unsigned long uint32;                                     
typedef volatile unsigned long vuint32;  


typedef uint8 SOCKET;


typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned char	uchar;
typedef unsigned int    uint;

#ifndef __cplusplus
typedef int				bool;
#define	true			1
#define false			0
#endif

typedef enum {
	VAR_LONG=32,
	VAR_SHORT=16,
	VAR_CHAR=8
} VAR_TYPE;

#ifndef NULL
#define NULL (void *)0
#endif

#endif	
