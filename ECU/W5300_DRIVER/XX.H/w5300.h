#ifndef _W5300_H
#define _W5300_H

#include "iinchip_conf.h"
#include "types.h"

 /**************************************************
*¼Ä´æÆ÷
******************************************************/
#define W5300_MR              (__DEF_IINCHIP_MAP_BASE__) 
#define W5300_IDM_AR          (__DEF_IINCHIP_MAP_BASE__ + 0x02)
#define W5300_IDM_DR          (__DEF_IINCHIP_MAP_BASE__ + 0x04)
#define W5300_IR              (COMMON_REG_BASE + 0x02)
#define W5300_IMR             (COMMON_REG_BASE + 0x04)
#define W5300_SHAR            (COMMON_REG_BASE + 0x08)
#define W5300_SHAR2           (W5300_SHAR + 2)
#define W5300_SHAR4           (W5300_SHAR + 4)
#define W5300_GAR             (COMMON_REG_BASE + 0x10)
#define W5300_GAR2            (W5300_GAR + 2)
#define W5300_SUBR            (COMMON_REG_BASE + 0x14)
#define W5300_SUBR2           (W5300_SUBR + 2)
#define W5300_SIPR            (COMMON_REG_BASE + 0x18)
#define W5300_SIPR2           (W5300_SIPR + 2)
#define W5300_RTR             (COMMON_REG_BASE + 0x1C)
#define W5300_RCR             (COMMON_REG_BASE + 0x1E)
#define W5300_TMS01R          (COMMON_REG_BASE + 0x20)		//TX´æ´¢Æ÷´óÐ¡ÅäÖÃ¼Ä´æÆ÷
#define W5300_TMS23R          (W5300_TMS01R + 2) 
#define W5300_TMS45R          (W5300_TMS01R + 4) 
#define W5300_TMS67R          (W5300_TMS01R + 6) 
#define W5300_RMS01R          (COMMON_REG_BASE + 0x28)      //RX´æ´¢Æ÷´óÐ¡ÅäÖÃ¼Ä´æÆ÷
#define W5300_RMS23R          (W5300_RMS01R + 2)   
#define W5300_RMS45R          (W5300_RMS01R + 4)   
#define W5300_RMS67R          (W5300_RMS01R + 6)   
#define W5300_MTYPER          (COMMON_REG_BASE + 0x30)
#define W5300_PATR            (COMMON_REG_BASE + 0x32)
#define W5300_PTIMER          (COMMON_REG_BASE + 0x36)
#define W5300_PMAGICR         (COMMON_REG_BASE + 0x38)
#define W5300_PSIDR           (COMMON_REG_BASE + 0x3C)
#define W5300_PDHAR           (COMMON_REG_BASE + 0x40)
#define W5300_PDHAR2          (W5300_PDHAR + 2)
#define W5300_PDHAR4          (W5300_PDHAR + 4)
#define W5300_UIPR            (COMMON_REG_BASE + 0x48)
#define W5300_UIPR2           (W5300_UIPR + 2)
#define W5300_UPORTR          (COMMON_REG_BASE + 0x4C)
#define W5300_FMTUR           (COMMON_REG_BASE + 0x4E)
#define W5300_Pn_BRDYR(n)     (COMMON_REG_BASE + 0x60 + n*4)
#define W5300_Pn_BDPTHR(n)    (COMMON_REG_BASE + 0x60 + n*4 + 2)
#define W5300_IDR             (COMMON_REG_BASE + 0xFE)

#define Sn_MR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x00)                     
#define Sn_CR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x02)
#define Sn_IMR(n)       (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x04)
#define Sn_IR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x06)
#define Sn_SSR(n)       (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x08) //200+240+8
#define Sn_PORTR(n)     (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x0A) 	//²Ù×÷º¯Êý»¹Î´Ð´
#define Sn_DHAR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x0C)
#define Sn_DHAR2(n)     (Sn_DHAR(n)+2)
#define Sn_DHAR4(n)     (Sn_DHAR(n)+4)

#define Sn_DPORTR(n)    (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x12)//212+240

#define Sn_DIPR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x14)  //200+14+240 454
#define Sn_DIPR2(n)     (Sn_DIPR(n)+2)

#define Sn_MSSR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x18)

#define Sn_PROTOR(n)		(SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x1A)

#define Sn_KPALVTR(n)   Sn_PROTOR(n)    
#define Sn_PROTOR1(n)   (Sn_PROTOR(n)+1)  

#define Sn_TOSR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x1C)

#define Sn_TTLR(n)      (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x1E)

#define Sn_TX_WRSR(n)		(SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x20) //6C000200
#define Sn_TX_WRSR2(n)		(Sn_TX_WRSR(n) + 2)

#define Sn_TX_FSR(n)       (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x0024)
#define Sn_TX_FSR2(n)      (Sn_TX_FSR(n) + 2)

#define Sn_RX_RSR(n)       (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x0028)
#define Sn_RX_RSR2(n)      (Sn_RX_RSR(n) + 2)

#define Sn_FRAGR(n)        (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x002C)

#define Sn_TX_FIFOR(n)     (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x2E)  //6C000000+0X200+1*40+0X2E

#define Sn_RX_FIFOR(n)     (SOCKET_REG_BASE + n * SOCKET_REG_SIZE + 0x30)



/************************************/
/* The bit of MR regsiter defintion */
/************************************/
#define MR_DBW             (1 << 15)            /**< Data bus width bit of MR. */
#define MR_MPF             (1 << 14)            /**< Mac layer pause frame bit of MR. */
#define MR_WDF(X)          ((X & 0x07) << 11)   /**< Write data fetch time bit of  MR. */
#define MR_RDH             (1 << 10)            /**< Read data hold time bit of MR. */
#define MR_FS              (1 << 8)             /**< FIFO swap bit of MR. */
#define MR_RST             (1 << 7)             /**< S/W reset bit of MR. */
#define MR_MT              (1 << 5)             /**< Memory test bit of MR. */
#define MR_PB              (1 << 4)             /**< Ping block bit of MR. */
#define MR_PPPoE           (1 << 3)             /**< PPPoE bit of MR. */
#define MR_DBS             (1 << 2)             /**< Data bus swap of MR. */
#define MR_IND             (1 << 0)             /**< Indirect mode bit of MR. */


/************************************/ 
/* The bit of IR regsiter defintion */ 
/************************************/ 
#define IR_IPCF            (1 << 7)             /**< IP conflict bit of IR. */
#define IR_DPUR            (1 << 6)             /**< Destination port unreachable bit of IR. */
#define IR_PPPT            (1 << 5)             /**< PPPoE terminate bit of IR. */
#define IR_FMTU            (1 << 4)             /**< Fragment MTU bit of IR. */
#define IR_SnINT(n)        (0x01 << n)          /**< SOCKETn interrupt occurrence bit of IR. */

/*****************************************/ 
/* The bit of Pn_BRDYR regsiter defintion*/ 
/*****************************************/ 
#define Pn_PEN             (1 << 7)             /**< PIN 'BRDYn' enable bit of Pn_BRDYR. */
#define Pn_MT              (1 << 6)             /**< PIN memory type bit of Pn_BRDYR. */
#define Pn_PPL             (1 << 5)             /**< PIN Polarity bit of Pn_BRDYR. */
#define Pn_SN(n)           ((n & 0x07) << 0)    /**< PIN Polarity bit of Pn_BRDYR. */


/***************************************/ 
/* The bit of Sn_MR regsiter defintion */ 
/***************************************/ 
#define Sn_MR_ALIGN        (1 << 8)             /**< Alignment bit of Sn_MR. */
#define Sn_MR_MULTI        (1 << 7)             /**< Multicasting bit of Sn_MR. */
#define Sn_MR_MF           (1 << 6)             /**< MAC filter bit of Sn_MR. */
#define Sn_MR_IGMPv        (1 << 5)             /**< IGMP version bit of Sn_MR. */
#define Sn_MR_ND           (1 << 5)             /**< No delayed ack bit of Sn_MR. */
#define Sn_MR_CLOSE        0x00                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_TCP          0x01                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_UDP          0x02                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_IPRAW        0x03                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_MACRAW       0x04                 /**< Protocol bits of Sn_MR. */
#define Sn_MR_PPPoE        0x05                 /**< Protocol bits of Sn_MR. */


/******************************/ 
/* The values of CR defintion */ 
/******************************/
#define Sn_CR_OPEN         0x01                 /**< OPEN command value of Sn_CR. */
#define Sn_CR_LISTEN       0x02                 /**< LISTEN command value of Sn_CR. */
#define Sn_CR_CONNECT      0x04                 /**< CONNECT command value of Sn_CR. */
#define Sn_CR_DISCON       0x08                 /**< DISCONNECT command value of Sn_CR. */
#define Sn_CR_CLOSE        0x10                 /**< CLOSE command value of Sn_CR. */
#define Sn_CR_SEND         0x20                 /**< SEND command value of Sn_CR. */
#define Sn_CR_SEND_MAC     0x21                 /**< SEND_MAC command value of Sn_CR. */ 
#define Sn_CR_SEND_KEEP    0x22                 /**< SEND_KEEP command value of Sn_CR */
#define Sn_CR_RECV         0x40                 /**< RECV command value of Sn_CR */
#define Sn_CR_PCON         0x23                 /**< PCON command value of Sn_CR */
#define Sn_CR_PDISCON      0x24                 /**< PDISCON command value of Sn_CR */ 
#define Sn_CR_PCR          0x25                 /**< PCR command value of Sn_CR */
#define Sn_CR_PCN          0x26                 /**< PCN command value of Sn_CR */
#define Sn_CR_PCJ          0x27                 /**< PCJ command value of Sn_CR */


/***********************************************************
* The values of Sn_IR defintion 
***********************************************************/
#define Sn_IR_PRECV        0x80                 /**< PPP receive bit of Sn_IR */
#define Sn_IR_PFAIL        0x40                 /**< PPP fail bit of Sn_IR */
#define Sn_IR_PNEXT        0x20                 /**< PPP next phase bit of Sn_IR */
#define Sn_IR_SENDOK       0x10                 /**< Send OK bit of Sn_IR */
#define Sn_IR_TIMEOUT      0x08                 /**< Timout bit of Sn_IR */
#define Sn_IR_RECV         0x04                 /**< Receive bit of Sn_IR */
#define Sn_IR_DISCON       0x02                 /**< Disconnect bit of Sn_IR */
#define Sn_IR_CON          0x01                 /**< Connect bit of Sn_IR */

/************************************************************
 *The values of Sn_SSR defintion 
************************************************************/
#define SOCK_CLOSED        0x00                 /**< SOCKETn is released */
#define SOCK_ARP           0x01                 /**< ARP-request is transmitted in order to acquire destination hardware address. */
#define SOCK_INIT          0x13                 /**< SOCKETn is open as TCP mode. */
#define SOCK_LISTEN        0x14                 /**< SOCKETn operates as "TCP SERVER" and waits for connection-request (SYN packet) from "TCP CLIENT". */
#define SOCK_SYNSENT       0x15                 /**< Connect-request(SYN packet) is transmitted to "TCP SERVER". */
#define SOCK_SYNRECV       0x16                 /**< Connect-request(SYN packet) is received from "TCP CLIENT". */
#define SOCK_ESTABLISHED   0x17                 /**< TCP connection is established. */
#define SOCK_FIN_WAIT      0x18                 /**< SOCKETn is closing. */
#define SOCK_CLOSING       0x1A                 /**< SOCKETn is closing. */
#define SOCK_TIME_WAIT     0x1B                 /**< SOCKETn is closing. */
#define SOCK_CLOSE_WAIT    0x1C                 /**< Disconnect-request(FIN packet) is received from the peer. */
#define SOCK_LAST_ACK      0x1D                 /**< SOCKETn is closing. */
#define SOCK_UDP           0x22                 /**< SOCKETn is open as UDP mode. */
#define SOCK_IPRAW         0x32                 /**< SOCKETn is open as IPRAW mode. */
#define SOCK_MACRAW        0x42                 /**< SOCKET0 is open as MACRAW mode. */
#define SOCK_PPPoE         0x5F                 /**< SOCKET0 is open as PPPoE mode. */

/* IP PROTOCOL */
#define IPPROTO_IP         0   /* Dummy for IP */
#define IPPROTO_ICMP       1   /* Control message protocol */
#define IPPROTO_IGMP       2   /* Internet group management protocol */
#define IPPROTO_GGP        3   /* Gateway^2 (deprecated) */
#define IPPROTO_TCP        6   /* TCP */
#define IPPROTO_PUP        12  /* PUP */
#define IPPROTO_UDP        17  /* UDP */
#define IPPROTO_IDP        22  /* XNS idp */
#define IPPROTO_ND         77  /* UNOFFICIAL net disk protocol */
#define IPPROTO_RAW        255 /* Raw IP packet */



/***************************************************
 * Basic I/O  Function 
 ***************************************************/
uint16   IINCHIP_READ(uint32 addr);

void     IINCHIP_WRITE(uint32 addr,uint16 data);

uint16   getMR(void);

void     setMR(uint16 val);


/********************************************************
*COMMON Register Access Function 
*********************************************************/
uint16   getIR(void);
void     setIR(uint16 val);
uint16   getIMR(void);
void     setIMR(uint16 mask);
void     getSHAR(uint8 * addr);
void     setSHAR(uint8 * addr);
void     getGAR(uint8 * addr);
void     setGAR(uint8 * addr);    // set gateway address
void     getSUBR(uint8 * addr);
void     setSUBR(uint8 * addr); // set subnet mask address
void	applySUBR(void);
void	clearSUBR(void);// set subnet mask address "0.0.0.0"

void     getSIPR(uint8 * addr);
void     setSIPR(uint8 * addr); // set local IP address
uint16   getRTR(void); 
void     setRTR(uint16 timeout); // set retry duration for data transmission, connection, closing ...
uint8    getRCR(void);
void     setRCR(uint8 retry); 
uint16   getPATR(void);
uint8    getPTIMER(void);
void     setPTIMER(uint8 time);
uint8    getPMAGICR(void);
void     setPMAGICR(uint8 magic);
uint16   getPSIDR(void);

/**
 *  It gets the PPPoE server hardware address
 */
void     getPDHAR(uint8* addr);

/**
 *  It gets the unreachable destination IP address of ICMP message.
 */
void     getUIPR(uint8* addr);

/**
 *  It gets the unreachable destination port number of ICMP message.
 */
uint16   getUPORTR(void);

/**
 *  It gets the fragmented MTU value of ICMP message.
 */
uint16   getFMTUR(void);

/**
 *  It gets Pn_BRDYR value.
 */
uint8    getPn_BRDYR(uint8 p);

/**
 *  It sets Pn_BRDYR value.
 */
void     setPn_BRDYR(uint8 p, uint8 cfg);

/**
 *  It gets the buffer depth of PIN "BRDYn".
 */
uint16   getPn_BDPTHR(uint8 p);

/**
 *  It sets the buffer depth of PIN "BRDYn".
 */
void     setPn_BDPTHR(uint8 p, uint16 depth);

/**
 *  It gets the ID of W5300.
 */
uint16   getIDR(void);

/**********************************************************
 * SOCKET Register Access Function *
 **********************************************************/
uint16   getSn_MR(SOCKET s);
void     setSn_MR(SOCKET s, uint16 mode); 
uint8    getSn_CR(SOCKET s);
void     setSn_CR(SOCKET s, uint16 com);
uint8    getSn_IMR(SOCKET s);
void     setSn_IMR(SOCKET s, uint8 mask);
uint8    getSn_IR(SOCKET s);
void     setSn_IR(SOCKET s, uint8 ir);
uint8    getSn_SSR(SOCKET s);
void     getSn_DHAR(SOCKET s, uint8* addr);
void     setSn_DHAR(SOCKET s, uint8* addr);
void     setSn_DPORTR(SOCKET s, uint16 port);
void     getSn_DIPR(SOCKET s, uint8* addr);
void     setSn_DIPR(SOCKET s, uint8* addr);
uint16   getSn_MSSR(SOCKET s);
void     setSn_MSSR(SOCKET s, uint16 mss);
uint8    getSn_KPALVTR(SOCKET s);
void     setSn_KPALVTR(SOCKET s, uint8 time);
uint32   getSn_TX_WRSR(SOCKET s);
void     setSn_TX_WRSR(SOCKET s, uint32 size);
uint32   getSn_TX_FSR(SOCKET s);
uint32   getSn_RX_RSR(SOCKET s);
void     setSn_TX_FIFOR(SOCKET s, uint16 data);
uint16   getSn_RX_FIFOR(SOCKET s);
uint8    getSn_PROTOR(SOCKET s);
void     setSn_PROTOR(SOCKET s, uint8 pronum);
uint8    getSn_TOSR(SOCKET s);
void     setSn_TOSR(SOCKET s, uint8 tos);
uint8    getSn_TTLR(SOCKET s);
void     setSn_TTLR(SOCKET s, uint8 ttl);
uint8    getSn_FRAGR(SOCKET s);
void     setSn_FRAGR(SOCKET s, uint8 frag);

/*****************************************************
 * ETC *
 *****************************************************/
void     iinchip_init(void); 

#ifdef __DEF_IINCHIP_INT__
   void     iinchip_irq(void);
#endif

uint8    sysinit(uint8* tx_size, uint8* rx_size); 
uint32   wiz_write_buf(SOCKET s,uint8* buf,uint32 len);
uint32   wiz_read_buf(SOCKET s, uint8* buf,uint32 len);
uint32   getIINCHIP_TxMAX(SOCKET s);
uint32   getIINCHIP_RxMAX(SOCKET s);

#ifdef __DEF_IINCHIP_PPP__
   /**
    *  It initializes to PPPoE of W5300
    */
   uint8    pppinit(uint8 *id, uint8 idlen, uint8 *passwd, uint8 passwdlen);
   /**
    *  It terminates PPPoE connection 
    */   
   void    pppterm(uint8 *mac, uint16 sessionid);
#endif
  void send_keepalive(uint8 s);
#endif





