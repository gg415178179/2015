#ifndef _IINCHIP_CONF_H
#define _IINCHIP_CONF_H

//#define __DEF_IINCHIP_DBG__			//获取MR寄存器值
//#define __DEF_IINCHIP_INT__    /**< Involves ISR routine */
//#define __DEF_IINCHIP_PPP__    /**< Involves PPP service routines and md5.h & md5.c */

#define __DEF_IINCHIP_MAP_BASE__  0x6C000000  //硬件地址
#define __DEF_IINCHIP_DIRECT_MODE__     1    //直接模式
#define __DEF_IINCHIP_INDIRECT_MODE__   2    //间接模式

#define SOCKET_REG_SIZE    0x40     // 每通道SOCKET寄存器数

#define MAX_SOCK_NUM 8  //SCOKET通道数

#define __DEF_IINCHIP_ADDRESS_MODE__           __DEF_IINCHIP_DIRECT_MODE__
//#define __DEF_IINCHIP_ADDRESS_MODE__         __DEF_IINCHIP_INDIRECT_MODE__

#if(__DEF_IINCHIP_ADDRESS_MODE__==__DEF_IINCHIP_DIRECT_MODE__)
    #define COMMON_REG_BASE  __DEF_IINCHIP_MAP_BASE__          
    #define SOCKET_REG_BASE  __DEF_IINCHIP_MAP_BASE__+0x200
#else
    #define COMMON_REG_BASE     0                               
    #define SOCKET_REG_BASE     0x0200 
#endif


#define __DEF_C__                  0   /**< Using C code */
#define __DEF_MCU_DEP_INLINE_ASM__ 1   /**< Using inline ASM code */
#define __DEF_MCU_DEP_DMA__        2   /**< Using DMA controller */

//TX/RX寄存器操作方式选择
#define __DEF_IINCHIP_BUF_OP__      __DEF_C__
//#define __DEF_IINCHIP_BUF_OP__    __DEF_MCU_DEP_DMA__

/**
 * Enter a critical section
 */
#define IINCHIP_CRITICAL_SECTION_ENTER() {}

/**
 * Exit a critical section
 */
#define IINCHIP_CRITICAL_SECTION_EXIT() {}
	
#endif

